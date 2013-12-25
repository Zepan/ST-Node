/********************************************************************************
 * �ļ���  ��common.h
 * ����    �����ú���ͷ�ļ�  
 * ���汾  ��V2.0.0
 * ����    �������޲�  QQ:715805855
 *�޸�ʱ�� ��2012-7-26
**********************************************************************************/

#ifndef __COMMON_H
#define __COMMON_H

#include "stm8s.h"
#include <stdio.h>

//����������ʼ��������
//EXTI
/*
	GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_IN_PU_IT);
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
*/
//ITC,�����ж����ȼ�,ITC_SetSoftwarePriority
/*
        ITC->ISPR6 &= (0x03<<((ITC_IRQ_UART2_RX%4)*2));
        ITC->ISPR6 |= (ITC_PRIORITYLEVEL_2<<((ITC_IRQ_UART2_RX%4)*2));
	    
		ITC->ISPR5 &= (0x03<<((ITC_IRQ_UART1_RX%4)*2));
        ITC->ISPR5 |= (ITC_PRIORITYLEVEL_2<<((ITC_IRQ_UART2_RX%4)*2));
*/

//�궨��
//����ֵ
#define ABS(x)	((x)>=0?(x):(-x))
//������������ֵ
#define MAX(a,b,c)	((a)>(b)?((a)>(c)?(a):(c)):((b)>(c)?(b):(c)))
#define MAX_INDEX(a,b,c)	((a)>(b)?((a)>(c)?0:2):((b)>(c)?1:2))

//ȡ�õ�ַ����Ӧ���������͵�����
#define MEM_U8(x) 		(*((u8 *)(x)))
#define MEM_U16(x) 		(*((u16 *)(x)))
#define MEM_U32(x) 		(*((u32 *)(x)))
#define MEM_FLOAT(x) 	(*((float *)(x)))

#define SYSCLK HSI_VALUE  //ϵͳƵ��

//ע�⣺�Ż��ٶ��ú궨�壬�Ż��ռ��ÿ⺯��
//GPIO����
#define PIN0 GPIO_PIN_0
#define PIN1 GPIO_PIN_1
#define PIN2 GPIO_PIN_2
#define PIN3 GPIO_PIN_3
#define PIN4 GPIO_PIN_4
#define PIN5 GPIO_PIN_5
#define PIN6 GPIO_PIN_6
#define PIN7 GPIO_PIN_7

#define PA_out 	 (GPIOA->ODR)
#define PA_in    (GPIOA->IDR)
#define PAset(n) (GPIOA->ODR|=(1<<n))
#define PAclr(n) (GPIOA->ODR&=~(1<<n))
#define PAneg(n) (GPIOA->ODR^=(1<<n))
#define PAin(n)	 ((BitStatus)(GPIOA->IDR&(1<<n)))

#define PB_out 	 (GPIOB->ODR)
#define PB_in    (GPIOB->IDR)
#define PBset(n) (GPIOB->ODR|=(1<<n))
#define PBclr(n) (GPIOB->ODR&=~(1<<n))
#define PBneg(n) (GPIOB->ODR^=(1<<n))
#define PBin(n)	 ((BitStatus)(GPIOB->IDR&(1<<n)))

#define PC_out 	 (GPIOC->ODR)
#define PC_in    (GPIOC->IDR)
#define PCset(n) (GPIOC->ODR|=(1<<n))
#define PCclr(n) (GPIOC->ODR&=~(1<<n))
#define PCneg(n) (GPIOC->ODR^=(1<<n))
#define PCin(n)	 ((BitStatus)(GPIOC->IDR&(1<<n)))

#define PD_out 	 (GPIOD->ODR)
#define PD_in    (GPIOD->IDR)
#define PDset(n) (GPIOD->ODR|=(1<<n))
#define PDclr(n) (GPIOD->ODR&=~(1<<n))
#define PDneg(n) (GPIOD->ODR^=(1<<n))
#define PDin(n)	 ((BitStatus)(GPIOD->IDR&(1<<n)))

#define PE_out 	 (GPIOE->ODR)
#define PE_in    (GPIOE->IDR)
#define PEset(n) (GPIOE->ODR|=(1<<n))
#define PEclr(n) (GPIOE->ODR&=~(1<<n))
#define PEneg(n) (GPIOE->ODR^=(1<<n))
#define PEin(n)	 ((BitStatus)(GPIOE->IDR&(1<<n)))

#define PF_out 	 (GPIOF->ODR)
#define PF_in    (GPIOF->IDR)
#define PFset(n) (GPIOF->ODR|=(1<<n))
#define PFclr(n) (GPIOF->ODR&=~(1<<n))
#define PFneg(n) (GPIOF->ODR^=(1<<n))
#define PFin(n)	 ((BitStatus)(GPIOF->IDR&(1<<n)))

#ifdef GPIOG
#define PG_out 	 (GPIOG->ODR)
#define PG_in    (GPIOG->IDR)
#define PGset(n) (GPIOG->ODR|=(1<<n))
#define PGclr(n) (GPIOG->ODR&=~(1<<n))
#define PGneg(n) (GPIOG->ODR^=(1<<n))
#define PGin(n)	 ((BitStatus)(GPIOG->IDR&(1<<n)))
#endif

#ifdef GPIOH
#define PH_out 	 (GPIOH->ODR)
#define PH_in    (GPIOH->IDR)
#define PHset(n) (GPIOH->ODR|=(1<<n))
#define PHclr(n) (GPIOH->ODR&=~(1<<n))
#define PHneg(n) (GPIOH->ODR^=(1<<n))
#define PHin(n)	 ((BitStatus)(GPIOH->IDR&(1<<n)))
#endif

#ifdef GPIOI
#define PI_out 	 (GPIOI->ODR)
#define PI_in    (GPIOI->IDR)
#define PIset(n) (GPIOI->ODR|=(1<<n))
#define PIclr(n) (GPIOI->ODR&=~(1<<n))
#define PIneg(n) (GPIOI->ODR^=(1<<n))
#define PIin(n)	 ((BitStatus)(GPIOI->IDR&(1<<n)))
#endif

//λ����
#define BIT0 PIN0
#define BIT1 PIN1
#define BIT2 PIN2
#define BIT3 PIN3
#define BIT4 PIN4

#define BIT5 PIN5
#define BIT6 PIN6
#define BIT7 PIN7


#endif
