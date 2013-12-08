/*!Copyright(c) 2012-2013 uclink
 *
 *\file	    bootDownloader.c
 *\brief	download hairboot to stm8 via swim
 *
 *\author	Zepan
 *\version	1.0.0
 *\date	07Dec13
 *
 *\history \arg 1.0.0, 07Dec13, Zepan, Create file. 
 * 				    
 */
//头文件
#include "app.h"

//宏定义
#define PAGE_SIZE 64
#define PAGE_CNT 8

#define VCC_1           PCset(7)
#define VCC_0           PCclr(7)
#define SWIM_1          PCset(6)
#define SWIM_0          PCclr(6)
#define SWIM_IN         PCin(6)
#define SWIM_OUTPUT_H   GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_FAST);
#define SWIM_INPUT_H    GPIOC->DDR &= ~(1<<6);
#define RST_1           PCset(5)
#define RST_0           PCclr(5)
#define LED_1           PCset(4)
#define LED_0           PCclr(4)

#define S_0  do{SWIM_0;\
    __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");\
    __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");\
    __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");\
    __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");\
    __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");\
    SWIM_1;\
    __asm("NOP\nNOP\nNOP\n");\
    }while(0)   //39+1 + 3+1
#define S_1  do{SWIM_0;\
    __asm("NOP\nNOP\nNOP\n");\
    SWIM_1;\
    __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");\
    __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");\
    __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");\
    __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");\
    __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");\
    }while(0)   //3+1 + 39+1     
#define RETRY_CNT 1

//函数声明
void EntrySeq(void);
char ProgramBoot(void);
void LED_Indicate(char flag);

char SWIM_SRST(void);
char SWIM_ROTF(void);
char SWIM_WOTF(void);
char SWIM_WriteByte(char* addr, char data);
char SWIM_WriteBuf(unsigned char n, char* addr, char* buf);
char SWIM_ReadByte(char* addr, char* data);
char SWIM_ReadBuf(unsigned char n, char* addr, char* buf);

//全局变量
char B;
char B_index;
char pb;

int main(void)
{
    //设置初始参数
    Parameter_Init();
    
    RST_0;
    DelayUs(10000);      //首先复位
    SWIM_0;
    DelayUs(20);        //>16us
    EntrySeq();
    while(SWIM_IN);
    while(!SWIM_IN);    //接收 128x HSI;（16us）
    SWIM_OUTPUT_H;
    DelayUs(1000);       //>300ns
    if(SWIM_SRST())     //软复位
    {
        printf("first SWIM_SRST failed!\n");
        LED_Indicate(1);
        return 1;
    }; 
    DelayUs(30000);
    if(SWIM_WriteByte((char*)0x7f80, 0xa0)) //设置debug mode
    {
        printf("set SWIM_CSR failed!\n");
        LED_Indicate(1);
        return 2;
    };
    DelayUs(10000);
    RST_1;
    DelayUs(10000);      //release rst>1ms
    //active now
    if(ProgramBoot())
    {
        printf("set ProgramBoot failed!\n");
        LED_Indicate(1);
        return 3;
    }
    else 
    {
        printf("ProgramBoot ok!\n"); 
        LED_Indicate(0);
    }
    SWIM_SRST();  
    return 0;
}

void Parameter_Init(void)
{    
    Set_HSI(); //使用内部16M晶振  
    DelayUs_Init();//tim1
    GPIO_Init(GPIOC, GPIO_PIN_HNIB, GPIO_MODE_OUT_PP_HIGH_FAST);    //PC7~4
    LED_0;
    Init_UART1(115200);
    //enableInterrupts(); //开全局中断
    DelayUs(50000);
}

void EntrySeq(void)
{
    char i;
    SWIM_OUTPUT_H;
    for(i = 0; i < 4; i++)
    {
        SWIM_1;
        DelayUs(500);
        SWIM_0;
        DelayUs(500);
    }
    for(i = 0; i < 4; i++)
    {
        SWIM_1;
        DelayUs(250);
        SWIM_0;
        DelayUs(250);
    }
    SWIM_INPUT_H;
}

char ProgramBoot(void)
{
    char i;
    SWIM_WriteByte((char*)0x7F99, 0x08);    //STALL CPU
    SWIM_WriteByte((char*)0x5062, 0x56);
    DelayUs(1000);
    SWIM_WriteByte((char*)0x5062, 0xae);    //解锁flash
    DelayUs(10000);
    for(i = 0; i <PAGE_CNT; i++)
    {
        SWIM_WriteByte((char*)0x505B, 0x01);            //标准块编程
        SWIM_WriteByte((char*)0x505C, 0xfe);
        if(SWIM_WriteBuf(PAGE_SIZE, (char*)0x8000+i*PAGE_SIZE, (char*)0x8000+i*PAGE_SIZE)) return 1;
        DelayUs(10000); //写flash延时
    }   
    return 0;
}

void LED_Indicate(char flag)
{
    char i;
    if(flag)    //失败，快闪
    {
        for(i = 0; i < 5; i++)
        {
            LED_1;
            Soft_DelayMs(200);
            LED_0;
            Soft_DelayMs(200);
        }
    }
    else    //成功，常亮
    {
        LED_1; 
    }
}

//input: byte to send
//return: 0,ok; else,err;
char S_TxByte(char data)
{
    char i;
    pb = data;
    pb ^= (pb >> 4);
    pb ^= (pb >> 2);
    pb ^= (pb >> 1);
    pb = pb & 0x01;         //bit xor
    
    for(i = RETRY_CNT; i > 0; i--)
    {
        B = data;
        B_index = 0;
        SWIM_OUTPUT_H;
        __asm(
        "CLR       A\n"             //提前清零
        "BRES      0x500a, #0x6\n"  //header//1
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"  //39+1=40
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"
        "BSET      0x500a, #0x6\n"
        "NOP\nNOP\nNOP\n"           //3+1=4
        "BRES      0x500a, #0x6\n"  //提前置零//1
        "NOP\nNOP\nNOP\nNOP\n" //3//额外1个
        "CheckFinish:\n"
        "CP        A, #0x8\n"       //共8bit//1       
        "JRNC      Finish\n"        //没有借位，即A>=8, 8bit完成//0.5 F
        "LD        A, B\n"          //载入数据//1
        "BCP       A, #0x80\n"      //A and 0x80//1
        "JREQ      SEND0\n"         //=0, send0, 否则 send1//0.5 F
        "SEND1:\n"                  //4低40高
        //"BRES      0x500a, #0x6\n"  //1//8L36H
        //"NOP\n"                     //1
        "BSET      0x500a, #0x6\n"  //1//0已经8个
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"  //22+2+12=36=44-8
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"  //额外3个
        "JRA       NextBit\n"       //为保持时间一致//2 F
        "SEND0:\n"                  //40低4高
        //"BRES      0x500a, #0x6\n"  //1//32L12H
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n" //7+24+1=32=44-12
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"//额外两个                    
        "BSET      0x500a, #0x6\n"  //1
        //"NOP\n"                     //1//额外一个
        "NextBit:\n"                 
        "LD        A, B\n"          //载入数据//1
        "SLL       A\n"             //左移一位//4
        "LD        B, A\n"          //保存数据//1
        "LD        A, B_index\n"    //载入当前位数//1
        "INC       A\n"             //自增//4
        "BRES      0x500a, #0x6\n"  //提前置零//1//H,2+12;L,3+4
        "LD        B_index, A\n"    //保存数据//1
        "JRA       CheckFinish\n"   //循环//2 F
        "Finish:\n"                 //数据发送完成，发送校验位
        "TNZ       pb\n"            //4
        "JREQ      SEND_0\n"        //=0,send_0,否则send_1//0.5 F //至此0共9个
        "SEND_1:\n"                 //4低16高
        //"BRES      0x500a, #0x6\n"  //1
        //"NOP\n"                     //1
        "BSET      0x500a, #0x6\n"  //1//至此0共10个
        /*"NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"                     //1
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"  //32+2=34=44=10*///以后有时间高
        "JRA      SEND_END\n"       //2 F
        "SEND_0:\n"                 //16低4高
        //"BRES      0x500a, #0x6\n"  //1
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"                     //1
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"
        "NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n"
        "NOP\nNOP\nNOP\nNOP\nNOP\n"   //9+30+1=40=44-4//再减一个  
        "BSET      0x500a, #0x6\n"  //1
        //"NOP\nNOP\nNOP\nNOP\n"      //1//以后有时间高
        "SEND_END:\n"               //至此结束
        );
        SWIM_INPUT_H;           //1//wait for ack
        while(SWIM_IN);         //等待下降沿
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");//延时超过半周期
        if(SWIM_IN)break;      //ack, return
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
    }
    SWIM_OUTPUT_H;
    return (i <= 0);
}

char SWIM_SRST(void)
{
    char i;
    for(i = RETRY_CNT; i > 0; i--)
    {
        SWIM_OUTPUT_H;
        S_0;S_0;S_0;S_0;S_0;
        SWIM_INPUT_H;   //1
        while(SWIM_IN);         //等待下降沿
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");//延时超过半周期
        if(SWIM_IN)break;      //ack, return
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
    };
    SWIM_OUTPUT_H;
    return (i <= 0);
}

char SWIM_ROTF(void)
{
    char i;
    for(i = RETRY_CNT; i > 0; i--)
    {
        SWIM_OUTPUT_H;
        S_0;S_0;S_0;S_1;S_1;
        SWIM_INPUT_H;
        while(SWIM_IN);         //等待下降沿
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");//延时超过半周期
        if(SWIM_IN)break;      //ack, return
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
    };
    SWIM_OUTPUT_H;
    return (i <= 0);
}
char SWIM_WOTF(void)
{
    char i;
    for(i = RETRY_CNT; i > 0; i--)
    {
        SWIM_OUTPUT_H;
        S_0;S_0;S_1;S_0;S_1;
        SWIM_INPUT_H;
        while(SWIM_IN);         //等待下降沿
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");//延时超过半周期
        if(SWIM_IN)break;      //ack, return
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
        __asm("NOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\nNOP\n");
    };
    SWIM_OUTPUT_H;
    return (i <= 0);
}

char SWIM_WriteByte(char* addr, char data)
{
    if(SWIM_WOTF()) return 1;    
    if(S_TxByte(1)) return 2;    //N
    if(S_TxByte(0)) return 3;    //@E
    if(S_TxByte((char)((unsigned short)addr>>8))) return 4;      //@H
    if(S_TxByte((char)((unsigned short)addr&0x0ff))) return 5;   //@L
    if(S_TxByte(data)) return 6;
    return 0;
}

//n:1~255
char SWIM_WriteBuf(unsigned char n, char* addr, char* buf)
{
    u16 i;
    
    if(SWIM_WOTF()) return 1;   
    if(S_TxByte(n)) return 2;    //N
    if(S_TxByte(0)) return 3;    //@E
    if(S_TxByte((char)((unsigned short)addr>>8))) return 4;      //@H
    if(S_TxByte((char)((unsigned short)addr&0x0ff))) return 5;   //@L
    for(i = 0; i < n; i++)
    {
        if(S_TxByte(buf[i])) break;
    }   
    if(i < n) return 6;
    else return 0;
}

char SWIM_ReadByte(char* addr, char* data)
{
    return 0;
}

char SWIM_ReadBuf(unsigned char n, char* addr, char* buf)
{
    return 0;
}