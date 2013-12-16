/***************************************************
@author:修改 BY oscarhua_zju 原著不详
@date:2011-8-03
@description: 模拟I2C
              移植-延时程序的实现和硬件端口初始化两个函数
******************************************************/
#ifndef __I2C_DRIVER_H
#define __I2C_DRIVER_H

#include "common.h"

/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
//#define GPIO_PORT_I2C	GPIOB			/* GPIO端口 */
//#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
//#define I2C_SCL_PIN		GPIO_Pin_6			/* 连接到SCL时钟线的GPIO */
//#define I2C_SDA_PIN		GPIO_Pin_7			/* 连接到SDA数据线的GPIO */

/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
/*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
#define I2C_SCL_1()  PBset(4)		                /* SCL = 1 */
#define I2C_SCL_0()  PBclr(4)				/* SCL = 0 */

#define I2C_SDA_1()  PBset(5)				/* SDA = 1 */
#define I2C_SDA_0()  PBclr(5)				/* SDA = 0 */

#define I2C_SDA_READ()  (PBin(5)!= 0)	                /* 读SDA口线状态 */

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

//函数声明
void i2c_Cfg(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
u8 i2c_SendByteA(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_ReadByteA(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);

#endif

/********************************************END OF FILE*********************************************************/


