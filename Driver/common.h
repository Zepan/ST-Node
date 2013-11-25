/********************************************************************************
 * 文件名  ：common.h
 * 描述    ：公用函数头文件  
 * 库版本  ：V2.0.0
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2012-7-26
**********************************************************************************/

#ifndef __COMMON_H
#define __COMMON_H

#include "stm8s.h"
#include <stdio.h>

//部分外设初始化样例：
//EXTI
/*
	GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_IN_PU_IT);
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
*/
//ITC,设置中断优先级,ITC_SetSoftwarePriority
/*
        ITC->ISPR6 &= (0x03<<((ITC_IRQ_UART2_RX%4)*2));
        ITC->ISPR6 |= (ITC_PRIORITYLEVEL_2<<((ITC_IRQ_UART2_RX%4)*2));
	    
		ITC->ISPR5 &= (0x03<<((ITC_IRQ_UART1_RX%4)*2));
        ITC->ISPR5 |= (ITC_PRIORITYLEVEL_2<<((ITC_IRQ_UART2_RX%4)*2));
*/

//宏定义
//绝对值
#define ABS(x)	((x)>=0?(x):(-x))
//三个数找最大值
#define MAX(a,b,c)	((a)>(b)?((a)>(c)?(a):(c)):((b)>(c)?(b):(c)))
#define MAX_INDEX(a,b,c)	((a)>(b)?((a)>(c)?0:2):((b)>(c)?1:2))

//取该地址上相应的数据类型的数据
#define MEM_U8(x) 		(*((u8 *)(x)))
#define MEM_U16(x) 		(*((u16 *)(x)))
#define MEM_U32(x) 		(*((u32 *)(x)))
#define MEM_FLOAT(x) 	(*((float *)(x)))

#define SYSCLK HSI_VALUE  //系统频率

//注意：优化速度用宏定义，优化空间用库函数
//GPIO操作
#define PIN0 GPIO_PIN_0
#define PIN1 GPIO_PIN_1
#define PIN2 GPIO_PIN_2
#define PIN3 GPIO_PIN_3
#define PIN4 GPIO_PIN_4
#define PIN5 GPIO_PIN_5
#define PIN6 GPIO_PIN_6
#define PIN7 GPIO_PIN_7

#define PAout 	 (GPIOA->ODR)
#define PAin		 (GPIOA->IDR)
#define PAset(n) (GPIOA->ODR|=(n));
#define PAclr(n) (GPIOA->ODR&=~(n));
#define PAneg(n) (GPIOA->ODR^=(n));
#define PAin_Pin(n)	 ((BitStatus)(GPIOA->IDR&(n))

#define PBout 	 (GPIOB->ODR)
#define PBin		 (GPIOB->IDR)
#define PBset(n) (GPIOB->ODR|=(n));
#define PBclr(n) (GPIOB->ODR&=~(n));
#define PBneg(n) (GPIOB->ODR^=(n));
#define PBin_Pin(n)	 ((BitStatus)(GPIOB->IDR&(n))

#define PCout 	 (GPIOC->ODR)
#define PCin		 (GPIOC->IDR)
#define PCset(n) (GPIOC->ODR|=(n));
#define PCclr(n) (GPIOC->ODR&=~(n));
#define PCneg(n) (GPIOC->ODR^=(n));
#define PCin_Pin(n)	 ((BitStatus)(GPIOC->IDR&(n))

#define PDout 	 (GPIOD->ODR)
#define PDin		 (GPIOD->IDR)
#define PDset(n) (GPIOD->ODR|=(n));
#define PDclr(n) (GPIOD->ODR&=~(n));
#define PDneg(n) (GPIOD->ODR^=(n));
#define PDin_Pin(n)	 ((BitStatus)(GPIOD->IDR&(n))

#define PEout 	 (GPIOE->ODR)
#define PEin		 (GPIOE->IDR)
#define PEset(n) (GPIOE->ODR|=(n));
#define PEclr(n) (GPIOE->ODR&=~(n));
#define PEneg(n) (GPIOE->ODR^=(n));
#define PEin_Pin(n)	 ((BitStatus)(GPIOE->IDR&(n))

#define PFout 	 (GPIOF->ODR)
#define PFin		 (GPIOF->IDR)
#define PFset(n) (GPIOF->ODR|=(n));
#define PFclr(n) (GPIOF->ODR&=~(n));
#define PFneg(n) (GPIOF->ODR^=(n));
#define PFin_Pin(n)	 ((BitStatus)(GPIOF->IDR&(n))

#ifdef GPIOG
#define PGout 	 (GPIOG->ODR)
#define PGin		 (GPIOG->IDR)
#define PGset(n) (GPIOG->ODR|=(n));
#define PGclr(n) (GPIOG->ODR&=~(n));
#define PGneg(n) (GPIOG->ODR^=(n));
#define PGin_Pin(n)	 ((BitStatus)(GPIOG->IDR&(n))
#endif

#ifdef GPIOH
#define PHout 	 (GPIOH->ODR)
#define PHin		 (GPIOH->IDR)
#define PHset(n) (GPIOH->ODR|=(n));
#define PHclr(n) (GPIOH->ODR&=~(n));
#define PHneg(n) (GPIOH->ODR^=(n));
#define PHin_Pin(n)	 ((BitStatus)(GPIOH->IDR&(n))
#endif

#ifdef GPIOI
#define PIout 	 (GPIOI->ODR)
#define PIin		 (GPIOI->IDR)
#define PIset(n) (GPIOI->ODR|=(n));
#define PIclr(n) (GPIOI->ODR&=~(n));
#define PIneg(n) (GPIOI->ODR^=(n));
#define PIin_Pin(n)	 ((BitStatus)(GPIOI->IDR&(n))
#endif

//位定义
#define BIT0 PIN0
#define BIT1 PIN1
#define BIT2 PIN2
#define BIT3 PIN3
#define BIT4 PIN4

#define BIT5 PIN5
#define BIT6 PIN6
#define BIT7 PIN7


#endif
