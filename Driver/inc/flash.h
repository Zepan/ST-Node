/********************************************************************************
 * 文件名  ：flash.h
 * 描述    ：flash操作相关函数头文件    
 * 库版本  ：V2.0.0
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2012-7-27
**********************************************************************************/
//注意使用flash相关功能应去掉stm8s.h中下面的注释
// To enable execution from RAM 
//#if !defined (RAM_EXECUTION)
//#define RAM_EXECUTION  (1)
//#endif /* RAM_EXECUTION */

#ifndef __FLASH_H
#define __FLASH_H

#include "common.h"

//宏定义
#define OPT2_ADDR 0x4803

//函数声明
void Cfg_OPT2(u8 bit_n,u8 opt);

#endif