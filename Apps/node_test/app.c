#include "app.h"
#include "string.h"
/****************  宏定义  ****************/
#define UID_ADDR 0x4865
#define UID_LEN 12
/**************** 函数声明 ****************/
void Cal_ID(u8 *buf);
/**************** 全局变量 ****************/
const char logo[] = "\
   ___________   _   __          __\n\
  / ___/_  __/  / | / /___  ____/ /__\n\
  \\__ \\ / /    /  |/ / __ \\/ __  / _ \\\n\
 ___/ // /    / /|  / /_/ / /_/ /  __/\n\
/____//_/    /_/ |_/\\____/\\__,_/\\___/\n\
       Welcome to use ST Node!\n";
u8 enterFlag = 0;
/**************** 函数 ****************/
int main(void)
{
    u8 i;
    u8 id[UID_LEN];
    Parameter_Init();               //设置初始参数
    printf("%s\n", logo);           //打印logo
    Cal_ID(id);
    printf("Your ST Node ID is: ");
    for(i = 0; i < UID_LEN; i++) printf("%02X", id[i]);
    printf("\nTry input something :)\n");
    while (1)
    {
        if(enterFlag)
        {
            printf("Enter @ %u.%03u s\n",(int)((systick*2)/1000), (int)((systick*2)%1000));
            enterFlag = 0;
        }
    }
}

//参数初始化
//输入参数：无
//输出参数：无
void Parameter_Init(void)
{
    
    Set_HSI();                      //16M 内部振荡器   
    Init_UART1(115200);	            // 初始化串口
    T4Tick_Init();                  //初始化系统定时器
    enableInterrupts();             //开全局中断
}

//计算节点ID
//输入参数：存储ID的缓存指针
//输出参数：无
void Cal_ID(u8 *buf)
{
    u8 i, j;
    memcpy(buf, (char *)UID_ADDR, UID_LEN);
    for(i =0; i <16; i++)
    {
        for(j = 0; j < UID_LEN; j++)
        {
            buf[j] ^= buf[(buf[j+1])%UID_LEN];
        }
    }
}

/**************** 中断句柄 ****************/
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
   systick++;
   TIM4->SR1 = (uint8_t)(~TIM4_IT_UPDATE);
}

INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
    u8 ch;
    if(UART1->SR & (u8)UART1_FLAG_RXNE)  
    {
        ch = UART1->DR;
		UART1_SendByte(ch);
        if(ch == '\n')
        {
            enterFlag = 1;
        }
    }
}