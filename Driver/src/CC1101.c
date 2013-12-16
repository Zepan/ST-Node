/*
================================================================================
Copyright   : Ebyte electronic co.,LTD
Website     : http://yh-ebyte.taobao.com
              http://yiheliyong.cn.alibaba.com
Description : This module contains the low level operations for CC1101
================================================================================
*/
#include "CC1101.H"

#define halRfWriteReg CC1101WriteReg
#define TI_CC_SPIWriteReg CC1101WriteReg
void halRfWriteRfSettings(void) 
{
	TI_CC_SPIWriteReg(CCxxx0_IOCFG2,0x06);  //GDO0 Output Pin Configuration
	TI_CC_SPIWriteReg(CCxxx0_IOCFG0,0x06);  //GDO0 Output Pin Configuration
	TI_CC_SPIWriteReg(CCxxx0_FIFOTHR,0x47); //RX FIFO and TX FIFO Thresholds
	TI_CC_SPIWriteReg(CCxxx0_PKTCTRL0,0x45);//Packet Automation Control
	TI_CC_SPIWriteReg(CCxxx0_FSCTRL1,0x06); //Frequency Synthesizer Control
	TI_CC_SPIWriteReg(CCxxx0_FREQ2,0x10);   //Frequency Control Word, High Byte
	TI_CC_SPIWriteReg(CCxxx0_FREQ1,0xB1);   //Frequency Control Word, Middle Byte
	TI_CC_SPIWriteReg(CCxxx0_FREQ0,0x3B);   //Frequency Control Word, Low Byte
	TI_CC_SPIWriteReg(CCxxx0_MDMCFG4,0xF5); //Modem Configuration
	TI_CC_SPIWriteReg(CCxxx0_MDMCFG3,0x83); //Modem Configuration
	TI_CC_SPIWriteReg(CCxxx0_MDMCFG2,0x1B); //Modem Configuration
	TI_CC_SPIWriteReg(CCxxx0_DEVIATN,0x15); //Modem Deviation Setting
	TI_CC_SPIWriteReg(CCxxx0_MCSM0,0x18);   //Main Radio Control State Machine Configuration
	TI_CC_SPIWriteReg(CCxxx0_FOCCFG,0x16);  //Frequency Offset Compensation Configuration
	TI_CC_SPIWriteReg(CCxxx0_WORCTRL,0xFB); //Wake On Radio Control
	TI_CC_SPIWriteReg(CCxxx0_FSCAL3,0xE9);  //Frequency Synthesizer Calibration
	TI_CC_SPIWriteReg(CCxxx0_FSCAL2,0x2A);  //Frequency Synthesizer Calibration
	TI_CC_SPIWriteReg(CCxxx0_FSCAL1,0x00);  //Frequency Synthesizer Calibration
	TI_CC_SPIWriteReg(CCxxx0_FSCAL0,0x1F);  //Frequency Synthesizer Calibration
	TI_CC_SPIWriteReg(CCxxx0_TEST2,0x81);   //Various Test Settings
	TI_CC_SPIWriteReg(CCxxx0_TEST1,0x35);   //Various Test Settings
	TI_CC_SPIWriteReg(CCxxx0_TEST0,0x09);   //Various Test Settings
//--------------------------------------------------------------------------	
	//
	// Rf settings for CC1101, 433M, 2.4KBPS
	/*
	TI_CC_SPIWriteReg(CCxxx0_IOCFG2,0x06);  //GDO0 Output Pin Configuration
	TI_CC_SPIWriteReg(CCxxx0_IOCFG0,0x06);  //GDO0 Output Pin Configuration
	TI_CC_SPIWriteReg(CCxxx0_FIFOTHR,0x47); //RX FIFO and TX FIFO Thresholds
	TI_CC_SPIWriteReg(CCxxx0_PKTCTRL0,0x05);//Packet Automation Control
	TI_CC_SPIWriteReg(CCxxx0_FSCTRL1,0x06); //Frequency Synthesizer Control
	TI_CC_SPIWriteReg(CCxxx0_FREQ2,0x10);   //Frequency Control Word, High Byte
	TI_CC_SPIWriteReg(CCxxx0_FREQ1,0xB1);   //Frequency Control Word, Middle Byte
	TI_CC_SPIWriteReg(CCxxx0_FREQ0,0x3B);   //Frequency Control Word, Low Byte
	TI_CC_SPIWriteReg(CCxxx0_MDMCFG4,0xF6); //Modem Configuration
	TI_CC_SPIWriteReg(CCxxx0_MDMCFG3,0x83); //Modem Configuration
	TI_CC_SPIWriteReg(CCxxx0_MDMCFG2,0x13); //Modem Configuration
	TI_CC_SPIWriteReg(CCxxx0_DEVIATN,0x15); //Modem Deviation Setting
	TI_CC_SPIWriteReg(CCxxx0_MCSM0,0x18);   //Main Radio Control State Machine Configuration
	TI_CC_SPIWriteReg(CCxxx0_FOCCFG,0x16);  //Frequency Offset Compensation Configuration
	TI_CC_SPIWriteReg(CCxxx0_WORCTRL,0xFB); //Wake On Radio Control
	TI_CC_SPIWriteReg(CCxxx0_FSCAL3,0xE9);  //Frequency Synthesizer Calibration
	TI_CC_SPIWriteReg(CCxxx0_FSCAL2,0x2A);  //Frequency Synthesizer Calibration
	TI_CC_SPIWriteReg(CCxxx0_FSCAL1,0x00);  //Frequency Synthesizer Calibration
	TI_CC_SPIWriteReg(CCxxx0_FSCAL0,0x1F);  //Frequency Synthesizer Calibration
	TI_CC_SPIWriteReg(CCxxx0_TEST2,0x81);   //Various Test Settings
	TI_CC_SPIWriteReg(CCxxx0_TEST1,0x35);   //Various Test Settings
	TI_CC_SPIWriteReg(CCxxx0_TEST0,0x09);   //Various Test Settings
*/
}

/*Write a command byte to the device*/
void CC1101WriteCmd( INT8U command );

/*
================================================================================
Function : CC1101ReadReg( )
    read a byte from the specified register
INPUT    : addr, The address of the register
OUTPUT   : the byte read from the rigister
================================================================================
*/
INT8U CC1101ReadReg( INT8U addr )
{
	INT8U i;
	CC_CSN_LOW( );
	SPI_ReadWriteByte( addr | READ_SINGLE);
	i = SPI_ReadWriteByte( 0xFF );
	CC_CSN_HIGH( );
	return i;
}
/*
================================================================================
Function : CC1101ReadMultiReg( )
    Read some bytes from the rigisters continously
INPUT    : addr, The address of the register
		   buff, The buffer stores the data
		   size, How many bytes should be read
OUTPUT   : None
================================================================================
*/
void CC1101ReadMultiReg( INT8U addr, INT8U *buff, INT8U size )
{
	INT8U i, j;
	CC_CSN_LOW( );
	SPI_ReadWriteByte( addr | READ_BURST);
	for( i = 0; i < size; i ++ )
	{
		for( j = 0; j < 20; j ++ );
		*( buff + i ) = SPI_ReadWriteByte( 0xFF );
	}
	CC_CSN_HIGH( );
}
/*
================================================================================
Function : CC1101ReadStatus( )
    Read a status register
INPUT    : addr, The address of the register
OUTPUT   : the value read from the status register
================================================================================
*/
INT8U CC1101ReadStatus( INT8U addr )
{
	INT8U i;
	CC_CSN_LOW( );
	SPI_ReadWriteByte( addr | READ_BURST);
	i = SPI_ReadWriteByte( 0xFF );
	CC_CSN_HIGH( );
	return i;
}
/*
================================================================================
Function : CC1101SetTRMode( )
    Set the device as TX mode or RX mode
INPUT    : mode selection
OUTPUT   : None
================================================================================
*/
void CC1101SetTRMode( TRMODE mode )
{
	if( mode == TX_MODE ) 		
	{ 
		CC1101WriteCmd( CCxxx0_STX ); 
	}
	else if( mode == RX_MODE )	
	{ 
		CC1101WriteCmd( CCxxx0_SRX ); 
	} 
}
/*
================================================================================
Function : CC1101WriteReg( )
    Write a byte to the specified register
INPUT    : addr, The address of the register
		   value, the byte you want to write
OUTPUT   : None
================================================================================
*/
void CC1101WriteReg( INT8U addr, INT8U value )
{
	CC_CSN_LOW( );
	SPI_ReadWriteByte( addr );
	SPI_ReadWriteByte( value );
	CC_CSN_HIGH( );
}
/*
================================================================================
Function : CC1101WriteMultiReg( )
    Write some bytes to the specified register
INPUT    : addr, The address of the register
		   buff, a buffer stores the values
		   size, How many byte should be written
OUTPUT   : None
================================================================================
*/
void CC1101WriteMultiReg( INT8U addr, INT8U *buff, INT8U size )
{
	INT8U i;
	CC_CSN_LOW( );
	SPI_ReadWriteByte( addr | WRITE_BURST );
	for( i = 0; i < size; i ++ )
	{
		SPI_ReadWriteByte( *( buff + i ) );	
	}
	CC_CSN_HIGH( );
}
/*
================================================================================
Function : CC1101WriteCmd( )
    Write a command byte to the device
INPUT    : command, the byte you want to write
OUTPUT   : None
================================================================================
*/
void CC1101WriteCmd( INT8U command )
{
	CC_CSN_LOW( );
	SPI_ReadWriteByte( command );
	CC_CSN_HIGH( );
}
/*
================================================================================
Function : CC1101Reset( )
    Reset the CC1101 device
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101Reset( void )
{
	INT8U x;
	
	CC_CSN_HIGH( );
	CC_CSN_LOW( );
	CC_CSN_HIGH( );
	for( x = 0; x < 100; x ++ );
	CC1101WriteCmd( CCxxx0_SRES );	
}
/*
================================================================================
Function : CC1101SetIdle( )
    Set the CC1101 into IDLE mode
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101SetIdle( void )
{
    CC1101WriteCmd(CCxxx0_SIDLE);
}
/*
================================================================================
Function : CC1101ClrTXBuff( )
    Flush the TX buffer of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101ClrTXBuff( void )
{
    CC1101SetIdle();//!!蹇呴』鍦↖dle鐘舵�?    CC1101WriteCmd( CCxxx0_SFTX );
}
/*
================================================================================
Function : CC1101ClrRXBuff( )
    Flush the RX buffer of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101ClrRXBuff( void )
{
    CC1101SetIdle();//!!蹇呴』鍦↖dle鐘舵�?    CC1101WriteCmd( CCxxx0_SFRX );
}
/*
================================================================================
Function : CC1101SendPacket( )
    Send a packet
INPUT    : txbuffer, The buffer stores data to be sent
           size, How many bytes should be sent
OUTPUT   : None
================================================================================
*/
void CC2500SendPacket( INT8U *txbuffer, INT8U size )
{
    CC1101ClrTXBuff( );
    CC1101WriteReg( CCxxx0_TXFIFO, size );
    CC1101WriteMultiReg( CCxxx0_TXFIFO, txbuffer, size );
    CC1101SetTRMode( TX_MODE );
    //while( ...... );
    while( CC1101_GDO0 == 0 );
    while( CC1101_GDO0 != 0 );

    CC1101ClrTXBuff( );
}
/*
================================================================================
Function : CC1101GetRXCnt( )
    Get received count of CC1101
INPUT    : None
OUTPUT   : How many bytes hae been received
================================================================================
*/
INT8U CC1101GetRXCnt( void )
{
    return ( CC1101ReadStatus( CCxxx0_RXBYTES )  & BYTES_IN_RXFIFO );
}
/*
================================================================================
Function : CC1101RecPacket( )
    Receive a packet
INPUT    : rxBuffer, A buffer store the received data
           size, How many bytes should be received
OUTPUT   : 1:received, 0:no data 
================================================================================
*/
INT8U CC1101RecPacket( INT8U *rxBuffer, INT8U *length )
{
	char status[2];
	char pktLen;
	INT16U x , j = 0;

	//CC1101SetTRMode( RX_MODE );
	while( CC1101_GDO0 != 0 )
	{
		for( x = 0; x < 10; x ++ );
		if( ++j >= 20 )  return 0;
	}
	if ( CC1101GetRXCnt( ) != 0 )
	{
		for( x = 0; x < 200; x ++ );
		pktLen = CC1101ReadReg(CCxxx0_RXFIFO);   // Read length byte

		if (pktLen <= *length)                    // If pktLen size <= rxBuffer
		{
			CC1101ReadMultiReg(CCxxx0_RXFIFO, rxBuffer, pktLen); // Pull data
			*length = pktLen;                     // Return the actual size
			CC1101ReadMultiReg(CCxxx0_RXFIFO, status, 2);// Read  status bytes
					
			CC1101ClrRXBuff( );
			if( status[1]&CRC_OK ) { return 1; }
			else {   return 0; }
		}                                       // Return CRC_OK bit
		else
		{
			*length = pktLen;                   // Return the large size
			CC1101ClrRXBuff( );      	        // Flush RXFIFO
			return 0;                           // Error
		}
	}
	else   {  return 0; }                       // Error
}

void CC1101Init( void )
{
	INT8U PaTabel[] = {0xc0};   //10dBm   
	volatile INT8U i;  

        //初始化SPI接口
      	SPI_Init(SPI_FIRSTBIT_MSB, CC1101_SPI_SPEED, SPI_MODE_MASTER,\
          SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, \
          SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
   	SPI_Cmd(ENABLE);
	//CSN推挽输出
   	GPIO_Init(CC1101_CSN_PORT , 1<<CC1101_CSN_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
   	CC1101_CSN_HIGH();
	//GDO0上拉输入
	GPIO_Init(CC1101_GDO0_PORT , 1<<CC1101_GDO0_PIN, GPIO_MODE_IN_PU_NO_IT);
	//GDO2上拉输入
	GPIO_Init(CC1101_GDO2_PORT , 1<<CC1101_GDO2_PIN, GPIO_MODE_IN_PU_NO_IT);
        
	CC1101Reset( );
	halRfWriteRfSettings( );
	CC1101WriteMultiReg(CCxxx0_PATABLE, PaTabel, 1);
	
	i = CC1101ReadStatus( CCxxx0_PARTNUM );//for test, must be 0x80
	i = CC1101ReadStatus( CCxxx0_VERSION );//for test, refer to the datasheet
}

/*
================================================================================
Function : CC1101RSSI( )
  get RSSI
INPUT    : none
OUTPUT   : RSSI dbm
================================================================================
*/
s16 CC1101RSSI(void)
{
#define RSSI_OFFSET 74
  s16 rssi_reg;  //有符号数  
  rssi_reg=(signed char)CC1101ReadStatus(CCxxx0_RSSI);
  return rssi_reg/2-RSSI_OFFSET;
}


/*
================================================================================
------------------------------------THE END-------------------------------------
================================================================================
*/
