/*
================================================================================
Copyright   : Ebyte electronic co.,LTD
Website     : http://yh-ebyte.taobao.com
              http://yiheliyong.cn.alibaba.com
Description : This module contains the low level operations for CC1101
================================================================================
*/
#ifndef _CC1101_H_
#define _CC1101_H_

#include "CC1101_REG.h"		//DO NOT modify
#include "common.h"
#include "spi.h"     //import SPI_ReadWriteByte

typedef unsigned char  INT8U;
typedef signed   char  INT8S;
typedef unsigned int   INT16U;
typedef signed   int   INT16S;
typedef unsigned long  INT32U;
typedef signed   long  INT32S;

//CC1101设置及操作线
#define CC1101_SPI_SPEED  SPI_BAUDRATEPRESCALER_2
#define CC1101_GDO0_PORT  GPIOC
#define CC1101_GDO0_PIN	  4
#define CC1101_GDO0       PCin(CC1101_GDO0_PIN)	
#define CC1101_GDO2_PORT  GPIOC
#define CC1101_GDO2_PIN	  3
#define CC1101_GDO2       PCin(CC1101_GDO2_PIN)	
#define CC1101_CSN_PORT	  GPIOA
#define CC1101_CSN_PIN	  3
#define CC1101_CSN_HIGH() PAset(CC1101_CSN_PIN) //SPI片选信号	   
#define CC1101_CSN_LOW()  PAclr(CC1101_CSN_PIN)  //SPI片选信号	

/*
================================================================================
------------------------------Internal IMPORT functions-------------------------
you must offer the following functions for this module
1. INT8U SPI_ExchangeByte( INT8U input ); //SPI Send and Receive function
2. CC_CSN_LOW( );                        //Pull down the CSN line
3. CC_CSN_HIGH( );                       //Pull up the CSN Line
================================================================================
*/

#define CC_CSN_LOW( )   PAclr(CC1101_CSN_PIN);while( PBin(7)!=0);  //
#define CC_CSN_HIGH( )  PAset(CC1101_CSN_PIN);


/*
================================================================================
-----------------------------------macro definitions----------------------------
================================================================================
*/
typedef enum { TX_MODE, RX_MODE }TRMODE;


/*read a byte from the specified register*/
INT8U CC1101ReadReg( INT8U addr );

/*Read some bytes from the rigisters continously*/
void CC1101ReadMultiReg( INT8U addr, INT8U *buff, INT8U size );

/*Read a status register*/
INT8U CC1101ReadStatus( INT8U addr );

/*Set the device as TX mode or RX mode*/
void CC1101SetTRMode( TRMODE mode );

/*Write a byte to the specified register*/
void CC1101WriteReg( INT8U addr, INT8U value );

/*Write some bytes to the specified register*/
void CC1101WriteMultiReg( INT8U addr, INT8U *buff, INT8U size );

/*Write a command byte to the device*/
void CC1101WriteCmd( INT8U command );

/*Reset the CC1101 device*/
void CC1101Reset( void );

/*Set the CC1101 into IDLE mode*/
void CC1101SetIdle( void );

/*Flush the TX buffer of CC1101*/
void CC1101ClrTXBuff( void );

/*Flush the RX buffer of CC1101*/
void CC1101ClrRXBuff( void );

/*Send a packet*/
void CC2500SendPacket( INT8U *txbuffer, INT8U size );

/*Get received count of CC1101*/
INT8U CC1101GetRXCnt( void );

/*Receive a packet*/
INT8U CC1101RecPacket( INT8U *rxBuffer, INT8U *length );


void CC1101Init( void );

s16 CC1101RSSI(void);

#endif // _CC1101_H_
/*
================================================================================
------------------------------------THE END-------------------------------------
================================================================================
*/
