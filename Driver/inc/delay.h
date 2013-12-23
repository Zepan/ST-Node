/********************************************************************************
 * 文件名  ：delay.h
 * 描述    ：延时函数头文件    
 * 库版本  ：V2.0.0
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2012-7-26
**********************************************************************************/

#ifndef __DELAY_H
#define __DELAY_H

#include "common.h"

//选择不同编译器,默认不优化情况
//#define USE_COSMIC
#define USE_IAR

//宏定义
#ifdef USE_COSMIC
  #define TICK_MS (SYSCLK/12000)
  #define TICK_US (SYSCLK/3200000)
#elif defined(USE_IAR)
  #define TICK_MS (SYSCLK/5000)
  #define TICK_US (SYSCLK/1200000)
#endif

#ifdef DELAY_GLOBAL
#define DELAY_EXTERN 
#else
#define DELAY_EXTERN extern
#endif

//毫秒精确延时只能用TIM1
#define	DIV_MS	(SYSCLK/1000-1)

#define USE_T2		//使用定时器2作为精确微秒延时定时器
#ifdef USE_T1
	#define	DIV_US	(SYSCLK/1000000-1)
#elif defined(USE_T2)||defined(USE_T3)
	#define	DIV_US	TIM2_PRESCALER_16	//这里要手动修改
#endif

#define TICK_FREQ (SYSCLK>>15)

//变量声明
DELAY_EXTERN volatile u32 systick;
DELAY_EXTERN u16 d_time;

//函数声明
void T4Tick_Init(void);
void Soft_DelayMs(u16 t);
void Soft_DelayUs(u16 t); 
void DelayMs_Init(void);
void DelayUs_Init(void);
void DelayMs(u16 t);
void DelayUs(u16 t);




#endif
