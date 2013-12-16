/***************************************************
@author:吴才泽
@date:2012-6-3
@description:spi驱动
******************************************************/
#include "spi.h"
  
/**
  * @brief  SPI波特率设置
  * @param  SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_256 
  * @retval None
  */
void SPI_SetSpeed(u16  SPI_BaudRatePrescaler)
{
	//改变SPI的波特率
	SPI->CR1&=~0x38;	//波特率控制位的掩码
	SPI->CR1|=SPI_BaudRatePrescaler;
	  
} 

//SPI 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI_ReadWriteByte(u8 TxData)
{	
  u8 retry=200;
  
	while((SPI->SR&1<<1)==0);//等待发送区空
		  
	SPI->DR=TxData;	 	  //发送一个byte 
	while((SPI->SR&1<<0)==0&&retry)//等待接收完一个byte   
	{
	 	retry--; 
	}
	if(!retry)return 0;
	else return SPI->DR;          //返回收到的数据				    
}




