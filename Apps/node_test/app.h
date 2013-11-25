/********************************************************************************
 * 文件名  ：app.h
 * 描述    ：应用层头文件     
 * 库版本  ：V2.0.0
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2012-7-28
**********************************************************************************/
#ifndef __APP_H
#define __APP_H

#include "stm8s.h"
//#include "stm8_tsl_api.h"
#include "driver.h"

//float格式：31:符号位,23~30,阶码(减127即为2的指数),0~22,底数，为小数部分,前面默认为1,即底数在1~2之间
//stm8为大端存储格式

//宏定义
#ifdef APP_GLOBAL
#define APP_EXTERN
#else
#define APP_EXTERN extern
#endif


//变量声明
APP_EXTERN u8 arg_flag;		//参数锁定标志


//函数声明
void Parameter_Init(void);


#endif



