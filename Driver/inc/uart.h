/********************************************************************************
 * 文件名  ：uart.h
 * 描述    ：串口应用头文件    
 * 库版本  ：V2.0.0
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2012-7-26
 *注：	USART1：RX-PA4,TX-PA5   
				USART2：RX-PD6,TX-PD5   
****************************************************************************************/
#ifndef __UART_H
#define __UART_H
#include "common.h"
#include <stdio.h>
#include <stdarg.h>

#ifdef UART_GLOBAL
	#define UART_EXTERN
#else
	#define UART_EXTERN extern
#endif

#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define RxBufferSize 64

//串口中断接收数据相关
//使用方法如下：
/*
		if(UART_REC)
		{
			UART2_SendData(RxBuffer,UART_NUM);
			UART2_SendByte('\r\n');
			UART_RX_NUM=0;
		}
*/
UART_EXTERN u8 RxBuffer[RxBufferSize];
//低6位表示接收到的字节数
//bit7表示接收到0x0d,bit8表示接收到0x0a,一行接收完成
UART_EXTERN u8 UART_RX_NUM;
#define UART_REC	(UART_RX_NUM&0x80)	//串口中断接收完成标志
#define UART_NUM	(UART_RX_NUM&0x3f)	//串口缓冲区的字节数


//字符串整形
static char *itoa(int value, char *string, int radix);

//如果是UART1
#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S103) ||\
    defined(STM8S003) || defined(STM8S903) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
		
//先等待非空再发送
#define UART1_SendByte(x)	\
  {while (!(UART1->SR & (u8)UART1_FLAG_TXE));UART1->DR = (x);}	
	
	void Init_UART1(u32 baud);
//	int fputc(int ch, FILE *f);
//	int fgetc(FILE *f);
	void UART1_SendString(u8* Data);
	void UART1_SendData(u8* Data,u8 len);
	u8 UART1_ReceiveByte(void);
	void UART1_printf( uint8_t *Data,...);
#endif
	
//串口2
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
//先等待非空再发送
#define UART2_SendByte(x)	\
  {while (!(UART2->SR & (u8)UART2_FLAG_TXE));UART2->DR = (x);}	
	
	void Init_UART2(u32 baud);
//	int fputc(int ch, FILE *f);
//	int fgetc(FILE *f);
	void UART2_SendString(u8* Data);
	void UART2_SendData(u8* Data,u8 len);
	u8 UART2_ReceiveByte(void);
	void UART2_printf( uint8_t *Data,...);
#endif


#endif