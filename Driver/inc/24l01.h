#ifndef __24L01_H
#define __24L01_H
	 		  
//CPU相关的头文件
#include "common.h"
#include "spi.h"

//上电10.3ms后进入掉电模式
//设置PWR_UP,1.5ms后进入待机模式I
//TX_FIFO非空，PRIM_RX=0,CE为高10us后进入发射设置状态，再过130us发射
//常规发射模式是，CE持续10us高脉冲，这样发送完后进入待机模式I
//PRIM_RX=1,CE为高进入接收设置状态，之后130us开始接收

//6个通道共用一个频段，但有不同地址，且地址的最低位必须不同，CH1~5高字节必须相同
//增强型猝发模式下，6个发送方把接收通道0作为ACK接收通道，所以RX_ADDR_P0=TX_ADDR

//SPI由低字节到高字节，每个字节先发MSB
//写寄存器前必须进入待机或掉电模式
//待机模式I：CE=0;待机模式II:发送模式时，TX_FIFO为空
//掉电模式：PWR_UP=0

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//NRF24L01寄存器操作命令
#define READ_2401_REG        0x00  //读配置寄存器,低5位为寄存器地址
#define WRITE_2401_REG       0x20  //写配置寄存器,低5位为寄存器地址
#define RD_RX_PLOAD     0x61  //读RX有效数据,1~32字节
#define WR_TX_PLOAD     0xA0  //写TX有效数据,1~32字节
#define FLUSH_TX        0xE1  //清除TX FIFO寄存器.发射模式下用
#define FLUSH_RX        0xE2  //清除RX FIFO寄存器.接收模式下用
#define REUSE_TX_PL     0xE3  //重新使用上一包数据,CE为高,数据包被不断发送.
#define NOP             0xFF  //空操作,可以用来读状态寄存器	
	//读写寄存器可能操作多字节寄存器，首先读/写低字节的高位
 
//SPI(NRF24L01)寄存器地址
#define CONFIG          0x00  //配置寄存器地址;
	#define PRIM_RX		0x01  //bit0:1接收模式,0发射模式;
	#define	PWR_UP		0x02  //bit1:上电选择,1上电，0掉电
	#define CRCO		0x04  //bit2:CRC模式，0,8位CRC，1,16位CRC
	#define EN_CRC		0x08  //bit3:CRC使能，如果EN_AA 中任意一位为高则EN_CRC强迫为高
							  //复位值为1
    #define MASK_MAX_RT 0x10  //bit4:中断MAX_RT(达到最大重发次数中断)使能;1,失能中断;0,MAX_RT中断产生时IRQ 引脚电平为低 
	#define MASK_TX_DS	0x20  //bit5:中断TX_DS使能;1，失能；0，TX_DS中断产生时IRQ 引脚电平为低
	#define MASK_RX_DR	0x40  //bit6:中断RX_DR使能;1，失能；0，RX_RD中断产生时IRQ 引脚电平为低

#define EN_AA           0x01  //使能自动应答功能  bit0~5,对应通道0~5,每个通道复位值均为1
							  //此功能禁止后可与nRF2401 通讯

#define EN_RXADDR       0x02  //接收地址允许,bit0~5,对应通道0~5
							  //0,1通道复位值允许，2~5通道复位值不允许

#define SETUP_AW        0x03  //设置发送/接收 地址宽度(所有数据通道):
	#define AW_3B		0x01  //bit1,0:00,3字节;01,4字节;02,5字节;
	#define AW_4B		0x02  //默认5字节
	#define AW_5B		0x03

#define SETUP_RETR      0x04  //建立自动重发;
							  //bit3:0,自动重发计数器;0,禁止重发；1，自动重发1次；……；15，自动重发15次
							  	//默认自动重发3次
							  //bit7:4,自动重发延时 (250*(x+1)+86)us，默认x=0

#define RF_CH           0x05  //RF通道,bit6:0,工作通道频率;默认为2

#define RF_SETUP        0x06  //RF寄存器;
							  //bit4:PLL_LOCK,PLL_LOCK允许，仅应用于测试模式,默认为0
	#define RF_DR		0x08  //bit3:数据传输速率(0:1Mbps,1:2Mbps);	默认2M
	#define RF_PWR		0x06  //bit2:1,发射功率;00,-18;01,-12;10,-6;11,0;默认0dbm
	#define LNA_HCURR	0x01  //bit0:低噪声放大器增益,默认1

#define STATUS          0x07  //状态寄存器;
	#define TX_FULL		0x01  //bit0:TX FIFO满标志;
	#define RX_P_NO		0x0E  //bit3:1,接收数据通道号(最大:6);
					  			//000~101,数据通道号；111，RX_FIFO为空
	#define MAX_RT		0x10  //bit4,达到最多次重发，必须写1清除后系统才能再通信
    #define TX_DS       0x20  //bit5:数据发送完成中断;写1清除
	#define RX_DR		0x40  //bit6:接收数据中断; 写1清除

#define OBSERVE_TX      0x08  //发送检测寄存器,
							  //bit7:4,数据包丢失计数器;当写RF_CH寄存器时此寄存器复位。当丢失 15个数据包后此寄存器重启
							  //bit3:0,重发计数器;发送新数据包时此寄存器复位 

#define CD              0x09  //载波检测寄存器,bit0,载波检测;

#define RX_ADDR_P0      0x0A  //数据通道0接收地址,最大长度5个字节,先写低字节
								//默认0xE7E7E7E7E7
#define RX_ADDR_P1      0x0B  //数据通道1接收地址,最大长度5个字节,先写低字节
								//默认0xC2C2C2C2C2
#define RX_ADDR_P2      0x0C  //CH2接收地址,默认0xC3，最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P3      0x0D  //CH3接收地址,默认0xC4，最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P4      0x0E  //CH4接收地址,默认0xC5，最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P5      0x0F  //CH5接收地址,默认0xC6，最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;

#define TX_ADDR         0x10  //发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
								//默认0xE7E7E7E7E7

#define RX_PW_P0        0x11  //接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P1        0x12  //接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P2        0x13  //接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P3        0x14  //接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P4        0x15  //接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P5        0x16  //接收数据通道5有效数据宽度(1~32字节),设置为0则非法

#define FIFO_STATUS     0x17  //FIFO状态寄存器;
	#define RX_EMPTY	0x01  //bit0,RX FIFO寄存器空标志;
	#define RX_FULL		0x02  //bit1,RX FIFO满标志;
    #define TX_EMPTY	0x10  //bit4,TX FIFO空标志;
	#define TX_FULL_F		0x20  //bit5,TX FIFO满标志;
	#define TX_REUSE	0x40  //bit6,1,当CE位高电平,循环发送上一数据包;0,不循环;
								//TX_REUSE通过SPI  指令REUSE_TX_PL 设置，通过W_TX_PALOAD 或FLUSH_TX 复位
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//24L01设置及操作线
#define NRF24L01_RF_SETUP	(RF_PWR|LNA_HCURR)	//0db增益,2Mbps,低噪声增益开启
#define NRF24L01_SPI_SPEED	SPI_BAUDRATEPRESCALER_8
#define NRF24L01_CE_PORT	GPIOA
#define NRF24L01_CE_PIN		2
#define NRF24L01_IRQ_PORT	GPIOA
#define NRF24L01_IRQ_PIN	1
#define NRF24L01_CSN_PORT	GPIOA
#define NRF24L01_CSN_PIN	3
#define NRF24L01_CE_HIGH()  PAset(NRF24L01_CE_PIN) //24L01射频使能信号
#define NRF24L01_CE_LOW()   PAclr(NRF24L01_CE_PIN) //24L01射频使能信号
#define NRF24L01_CSN_HIGH() PAset(NRF24L01_CSN_PIN) //SPI片选信号	   
#define NRF24L01_CSN_LOW()  PAclr(NRF24L01_CSN_PIN)  //SPI片选信号	
#define NRF24L01_IRQ  		PAin(NRF24L01_IRQ_PIN)  //IRQ主机数据输入
//24L01发送接收数据宽度定义
#define TX_ADR_WIDTH    5   //5字节的地址宽度
#define RX_ADR_WIDTH    5   //5字节的地址宽度
#define TX_PLOAD_WIDTH  32  //n字节的用户数据宽度
#define RX_PLOAD_WIDTH  32  //n字节的用户数据宽度

//通道参数结构体
typedef struct
{
 	u8 n;	//6通道选择
	u8 addr[TX_ADR_WIDTH];	//通道地址
	u8 f;	//通道频率，0~127
}NRF24L01_CH_TypeDef;

void NRF24L01_Init(void);//初始化
void RX_Mode(NRF24L01_CH_TypeDef *ch);//配置某通道为接收模式
void TX_Mode(NRF24L01_CH_TypeDef *ch);//配置某通道为发送模式
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 u8s);//写数据区
u8 NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 u8s);//读数据区		  
u8 NRF24L01_Read_Reg(u8 reg);			//读寄存器
u8 NRF24L01_Write_Reg(u8 reg, u8 value);//写寄存器
u8 NRF24L01_Check(void);//检查24L01是否存在
u8 NRF24L01_TxPacket(u8 *txbuf);//发送一个包的数据
u8 NRF24L01_RxPacket(u8 *rxbuf);//接收一个包的数据

#endif











