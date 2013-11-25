/********************************************************************************
 * 文件名  ：uart.c
 * 描述    ：串口应用相关函数    
 * 库版本  ：V2.0.0
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2012-7-26
 *注：	USART1：RX-PA4,TX-PA5   
				USART2：RX-PD6,TX-PD5   
****************************************************************************************/
#define UART_GLOBAL
#include "uart.h"

//如果是UART1
#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S103) ||\
    defined(STM8S003) || defined(STM8S903) || defined (STM8AF52Ax) || defined (STM8AF62Ax)

void Init_UART1(u32 baud)
{
  //以下是压缩代码   
    uint32_t BaudRate_Mantissa = 0, BaudRate_Mantissa100 = 0;
    UART1->CR1 |= (uint8_t)UART1_WORDLENGTH_8D;
    UART1->CR3 |= (uint8_t)UART1_STOPBITS_1;   
    UART1->CR1 |= (uint8_t)UART1_PARITY_NO;   
    BaudRate_Mantissa    = ((uint32_t)SYSCLK / (baud << 4));
    BaudRate_Mantissa100 = (((uint32_t)SYSCLK * 100) / (baud << 4));
    UART1->BRR2 |= (uint8_t)((uint8_t)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100)) << 4) / 100) & (uint8_t)0x0F); 
    UART1->BRR2 |= (uint8_t)((BaudRate_Mantissa >> 4) & (uint8_t)0xF0); 
    UART1->BRR1 |= (uint8_t)BaudRate_Mantissa;           
    UART1->CR2 &= (uint8_t)~(UART1_CR2_TEN | UART1_CR2_REN); 
    UART1->CR3 |= (uint8_t)((uint8_t)UART1_SYNCMODE_CLOCK_DISABLE & (uint8_t)(UART1_CR3_CPOL | 
                                              UART1_CR3_CPHA | UART1_CR3_LBCL));  
    UART1->CR2 |= (uint8_t)UART1_CR2_TEN;  
    UART1->CR2 |= (uint8_t)UART1_CR2_REN;  
    UART1->CR3 &= (uint8_t)(~UART1_CR3_CKEN); 
	
    UART1->CR2 |= ((uint8_t)1<<5);	//接收中断使能
	
	UART1->CR1 &= (uint8_t)(~UART1_CR1_UARTD);	//使能UART
  
  //这里可以再设置下IO输入输出模式
//    UART1_DeInit();
//    UART1_Init(baud, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, \
//    UART1_PARITY_NO , UART1_SYNCMODE_CLOCK_DISABLE , UART1_MODE_TXRX_ENABLE);
//    UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE  );
//    UART1_Cmd(ENABLE );  
}

void UART1_SendString(u8* Data)
{
  u16 i=0;
  for(;Data[i];i++)
    UART1_SendByte(Data[i]);
}

void UART1_SendData(u8* Data,u8 len)
{
  u16 i=0;
  for(;i<len;i++)
    UART1_SendByte(Data[i]);
}

u8 UART1_ReceiveByte(void)
{
     while(!(UART1->SR & (u8)UART1_FLAG_RXNE));
     return ((uint8_t)UART1->DR);
}


//重定向c库函数printf到USART1
int putchar(int ch)
{  
  UART1_SendByte(ch);
  return (ch);
}
/*
int fgetc(FILE *f)
{
     while(!(UART1->SR & (u8)UART1_FLAG_RXNE));
     return ((uint8_t)UART1->DR);  
}
*/

/************************************************************************
 * 函数名：UART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART1_printf( "\r\n this is a demo \r\n" );
 *            		 USART1_printf(  "\r\n %d \r\n", i );
 *            		 USART1_printf(  "\r\n %s \r\n", j );
 ***************************************************************************/
void UART1_printf( uint8_t *Data,...)
{
  const char *s;
  int d;   
  char buf[16];
  va_list ap;
  va_start(ap, Data);

  while ( *Data != 0)     // 判断是否到达字符串结束符
  {				                          
      if ( *Data == 0x5c )  //'\'
      {									  
              switch ( *++Data )
              {
                      case 'r':							          //回车符
                              UART1_SendData8(0x0d);
                              Data ++;
                              break;

                      case 'n':							          //换行符
                              UART1_SendData8(0x0a);	
                              Data ++;
                              break;
                      
                      default:
                              Data ++;
                          break;
              }			 
      }
      else if ( *Data == '%')
      {					//
	switch ( *++Data )
	  {				
	    case 's':						//字符串
		s = va_arg(ap, const char *);
          for ( ; *s; s++) 
	  {
	    UART1_SendData8(*s);
	    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
          }
	  Data++;
          break;

          case 'd':	//十进制
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
	{
	  UART1_SendData8(*s);
	  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
        }
	Data++;
        break;
	default:
	Data++;
	break;
	}		 
     } /* end of else if */
    else UART1_SendData8(*Data++);
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
  }
}

#endif

//串口2
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
void Init_UART2(u32 baud)
{
    UART2_DeInit();
    UART2_Init(baud, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, \
    UART2_PARITY_NO , UART2_SYNCMODE_CLOCK_DISABLE , UART2_MODE_TXRX_ENABLE);
    UART2_ITConfig(UART2_IT_RXNE_OR,ENABLE  );
    UART2_Cmd(ENABLE );  
}

void UART2_SendString(u8* Data)
{
  u16 i=0;
  for(;Data[i];i++)
    UART2_SendByte(Data[i]);
}

void UART2_SendData(u8* Data,u8 len)
{
  u16 i=0;
  for(;i<len;i++)
    UART2_SendByte(Data[i]);
}

u8 UART2_ReceiveByte(void)
{
     while(!(UART2->SR & (u8)UART2_FLAG_RXNE));
     return ((uint8_t)UART2->DR);    
}

/*
//重定向c库函数printf到USART2
int fputc(int ch, FILE *f)
{  
  UART2_SendByte(ch);
  return (ch);
}
int fgetc(FILE *f)
{
     while(!(UART2->SR & (u8)UART2_FLAG_RXNE));
     return ((uint8_t)UART2->DR);  
}
*/

/************************************************************************
 * 函数名：UART2_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART2
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART1_printf( "\r\n this is a demo \r\n" );
 *            		 USART1_printf(  "\r\n %d \r\n", i );
 *            		 USART1_printf(  "\r\n %s \r\n", j );
 ***************************************************************************/
void UART2_printf( uint8_t *Data,...)
{
  const char *s;
  int d;   
  char buf[16];
  va_list ap;
  va_start(ap, Data);

  while ( *Data != 0)     // 判断是否到达字符串结束符
  {				                          
      if ( *Data == 0x5c )  //'\'
      {									  
              switch ( *++Data )
              {
                      case 'r':							          //回车符
                              UART2_SendData8(0x0d);
                              Data ++;
                              break;

                      case 'n':							          //换行符
                              UART2_SendData8(0x0a);	
                              Data ++;
                              break;
                      
                      default:
                              Data ++;
                          break;
              }			 
      }
      else if ( *Data == '%')
      {					//
	switch ( *++Data )
	  {				
	    case 's':						//字符串
		s = va_arg(ap, const char *);
          for ( ; *s; s++) 
	  {
	    UART2_SendData8(*s);
	    while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
          }
	  Data++;
          break;

          case 'd':	//十进制
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
	{
	  UART2_SendData8(*s);
	  while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
        }
	Data++;
        break;
	default:
	Data++;
	break;
	}		 
     } /* end of else if */
    else UART2_SendData8(*Data++);
    while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
  }
}

#endif




/********************************************************
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被UARTx_printf()调用
 *******************************************************/
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */




/*****END OF FILE*****/