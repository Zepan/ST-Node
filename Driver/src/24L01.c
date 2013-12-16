#include "24l01.h"
#include "delay.h"
							    
//初始化24L01的IO口
void NRF24L01_Init(void)
{  
	SPI_Init(SPI_FIRSTBIT_MSB, NRF24L01_SPI_SPEED, SPI_MODE_MASTER,\
            SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, \
            SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
   	SPI_Cmd(ENABLE);
	//CSN推挽输出
   	GPIO_Init(NRF24L01_CSN_PORT , 1<<NRF24L01_CSN_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
   	NRF24L01_CSN_HIGH();
	//CE推挽输出
	GPIO_Init(NRF24L01_CE_PORT , 1<<NRF24L01_CE_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
	//IRQ上拉输入
	GPIO_Init(NRF24L01_IRQ_PORT , 1<<NRF24L01_IRQ_PIN, GPIO_MODE_IN_PU_NO_IT);

	NRF24L01_CE_LOW(); 					//失能24L01	  		 		  
}

//检测24L01是否存在(读写寄存器测试)
//返回值:0，成功;1，失败	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	NRF24L01_Write_Buf(WRITE_2401_REG|TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//检测24L01错误	
	return 0;		 //检测到24L01
}	 
	 
//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN_LOW();                 //使能SPI传输
  	status =SPI_ReadWriteByte(reg);//发送寄存器号 
  	SPI_ReadWriteByte(value);      //写入寄存器的值
  	NRF24L01_CSN_HIGH();                 //禁止SPI传输	   
  	return(status);       			//返回状态值
}

//读取SPI寄存器值
//reg:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 	NRF24L01_CSN_LOW();          //使能SPI传输		
  	SPI_ReadWriteByte(reg);   //发送寄存器号
  	reg_val=SPI_ReadWriteByte(NOP);//读取寄存器内容
  	NRF24L01_CSN_HIGH();          //禁止SPI传输		    
  	return(reg_val);           //返回状态值
}
	
//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN_LOW();           //使能SPI传输
  	status=SPI_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI_ReadWriteByte(NOP);//读出数据
  	NRF24L01_CSN_HIGH();       //关闭SPI传输
  	return status;        //返回读到的状态值
}

//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
 	NRF24L01_CSN_LOW();          //使能SPI传输
  	status = SPI_ReadWriteByte(reg);//发送寄存器值(位置)
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI_ReadWriteByte(*pBuf++); //写入数据	 
  	NRF24L01_CSN_HIGH();       //关闭SPI传输
  	return status;          //返回读到的状态值
}	
			   
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	//（24L01的最大SPI时钟为10Mhz）
	//SPIx_SetSpeed(&NRF24L01_SPIx,NRF24L01_SPI_SPEED);
	NRF24L01_CE_LOW();	   //待机模式I
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	NRF24L01_CE_HIGH();//启动发送	   
	while(NRF24L01_IRQ);//等待发送完成
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值	   
	NRF24L01_Write_Reg(WRITE_2401_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_RT)//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,NOP);//清除TX FIFO寄存器 
		return MAX_RT; 
	}
	if(sta&TX_DS)//发送完成
	{
		return TX_DS;
	}
	return 0xff;//其他原因发送失败
}

//启动NRF24L01接收一次数据
//rxbuf:待接收数据首地址
//返回值:0，接收完成；其他，错误代码
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	//（24L01的最大SPI时钟为10Mhz）
	//SPIx_SetSpeed(&NRF24L01_SPIx,NRF24L01_SPI_SPEED);   
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(WRITE_2401_REG|STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_DR)//接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,NOP);//清除RX FIFO寄存器
		return 0; 
	}	   
	return 1;//没收到任何数据
}	
				    
//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void RX_Mode(NRF24L01_CH_TypeDef *ch)
{
	NRF24L01_CE_LOW();	//待机模式I	  
  	NRF24L01_Write_Buf(WRITE_2401_REG|(RX_ADDR_P0+ch->n),(u8*)ch->addr,RX_ADR_WIDTH);//写RX节点地址	  
  	NRF24L01_Write_Reg(WRITE_2401_REG|EN_AA,(0x01<<(ch->n)));    //使能通道的自动应答    
  	NRF24L01_Write_Reg(WRITE_2401_REG|EN_RXADDR,(0x01<<(ch->n)));//使能通道的接收地址  	 
  	NRF24L01_Write_Reg(WRITE_2401_REG|RF_CH,ch->f);	     //设置RF通信频率,0~127		  
  	NRF24L01_Write_Reg(WRITE_2401_REG|(RX_PW_P0+ch->n),RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  	NRF24L01_Write_Reg(WRITE_2401_REG|RF_SETUP,NRF24L01_RF_SETUP);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(WRITE_2401_REG|CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
													//使能中断
  	NRF24L01_CE_HIGH(); //CE为高,进入接收模式 
        Soft_DelayUs(200);
}	
					 
//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void TX_Mode(NRF24L01_CH_TypeDef *ch)
{														 
	NRF24L01_CE_LOW();	    
  	NRF24L01_Write_Buf(WRITE_2401_REG|TX_ADDR,ch->addr,TX_ADR_WIDTH);//写TX节点地址 
  	NRF24L01_Write_Buf(WRITE_2401_REG|RX_ADDR_P0,ch->addr,RX_ADR_WIDTH); //设置RX节点地址,自动ACK需要	  

  	NRF24L01_Write_Reg(WRITE_2401_REG|EN_AA,(0x01<<(ch->n)));     //使能通道0的自动应答    
  	NRF24L01_Write_Reg(WRITE_2401_REG|EN_RXADDR,(0x01<<(ch->n))); //使能通道0的接收地址  
  	NRF24L01_Write_Reg(WRITE_2401_REG|SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	NRF24L01_Write_Reg(WRITE_2401_REG|RF_CH,ch->f);       //设置RF通道为40
  	NRF24L01_Write_Reg(WRITE_2401_REG|RF_SETUP,NRF24L01_RF_SETUP);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(WRITE_2401_REG|CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
													//使能中断
	NRF24L01_CE_HIGH();//CE为高,10us后启动发送
        Soft_DelayUs(100);

}		  




