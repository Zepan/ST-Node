/********************************************************************************
 * 文件名  ：timer.h
 * 描述    ：定时器相关函数的头文件    
 * 库版本  ：V2.0.0
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2012-7-27
**********************************************************************************/
#ifndef __TIMER_H
#define __TIMER_H

#include "common.h"

//输入捕获初始化样例
/*
  //通道1，上升沿，
  //TIM1_ICSELECTION_INDIRECTTI表示相邻通道，
  //  如CH1<->CH2,CH3<->CH4,DIRECTTI则为本通道，见P130
  //TIM1_ICPSC_DIV1表示捕捉预分频，捕捉到几次才触发一次
  //最后一个参数为输入滤波器参数，0时无滤波器，详见P154
  TIM1_ICInit(TIM1_CHANNEL_1, TIM1_ICPOLARITY_RISING, \
    TIM1_ICSELECTION_INDIRECTTI, TIM1_ICPSC_DIV1, 0x00);
  TIM1_ITConfig( TIM1_IT_CC1 , ENABLE);   //输入捕获1中断使能
  TIM1_ClearFlag(TIM1_FLAG_CC1);  //清中断标志
  TIM1_Cmd(ENABLE); //运行定时器
*/

//结构体
typedef struct {
	float f;	//PWM的频率
	float duty;	//PWM的占空比
	u8 ch;	//使用的通道 
//	GPIO_TypeDef *  GPIOx;	//该通道所在GPIO口
//	GPIO_Pin_TypeDef  Pinx;	//该通道所用引脚，如GPIO_PIN_0
}PWM_Typedef;

//函数声明
void T1_PWM_Init(PWM_Typedef *PWMx);
void T1_PWM_Duty(PWM_Typedef *PWMx);
void T2_PWM_Init(PWM_Typedef *PWMx);
void T2_PWM_Duty(PWM_Typedef *PWMx);

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) ||defined(STM8S105) ||\
    defined(STM8S005) ||  defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8AF626x)
void T3_PWM_Init(PWM_Typedef *PWMx);
void T3_PWM_Duty(PWM_Typedef *PWMx);
#endif

#endif