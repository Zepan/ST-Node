/***************************************************
@author:吴才泽
@date:2012-6-3
@description:spi头文件
******************************************************/
#ifndef __SPI_H
#define __SPI_H

#include "common.h"

//函数声明
void SPI_SetSpeed(u16  SPI_BaudRatePrescaler);
u8 SPI_ReadWriteByte(u8 TxData);


#endif

