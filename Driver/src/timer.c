/********************************************************************************
 * 文件名  ：timer.c
 * 描述    ：定时器操作相关函数     
 * 库版本  ：V2.0.0
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2012-7-27,28，29
**********************************************************************************/

#include "timer.h"

//定时器1的PWM初始化,不包含引脚重映射，需在之前设置选项字
//输入参数：PWM结构体指针
//输出参数：无
void T1_PWM_Init(PWM_Typedef *PWMx)
{
//    //初始化IO口为推挽输出
//    GPIO_Init(PWMx->GPIOx,PWMx->Pinx, GPIO_MODE_OUT_PP_LOW_FAST); 
  u16 ps ;	//预分频值
  u16 preload,dutytick;
  //根据要产生的频率计算分频值,除最高档(4K以上)外其它档保证12位以上的精度
  if(PWMx->f>((SYSCLK>>16)+1))	//16M时245HZ以上	
    ps=1-1;
  else if(PWMx->f>((SYSCLK>>20)+1))	//16~245
    ps=16-1;
  else   //0.95~16,低于0.9Hz则输出不正确
    ps=256-1;
  preload=(u16)(SYSCLK/(PWMx->f)/(ps+1));
  dutytick=(u16)(preload*(PWMx->duty)+0.5);
  //时基初始化
  TIM1_TimeBaseInit(ps, TIM1_COUNTERMODE_UP, \
      (u16)(preload-0.5), 0x00);
  //PWM输出初始化
  switch(PWMx->ch)
  {
    case 1:
      TIM1_OC1Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_DISABLE,\
        dutytick,TIM1_OCPOLARITY_HIGH,\
        TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_SET);
      //预装载使能
      TIM1_OC1PreloadConfig(ENABLE);  
      break;
    case 2:
      TIM1_OC2Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_DISABLE,\
        dutytick,TIM1_OCPOLARITY_HIGH,\
        TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_SET);
      //预装载使能
      TIM1_OC2PreloadConfig(ENABLE);   
      break;
    case 3:
      TIM1_OC3Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_DISABLE,\
        dutytick,TIM1_OCPOLARITY_HIGH,\
        TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_SET);
      //预装载使能
      TIM1_OC3PreloadConfig(ENABLE);  
      break;
    case 4:
      TIM1_OC4Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, \
        dutytick, \
          TIM1_OCPOLARITY_HIGH, TIM1_OCIDLESTATE_SET);
      //预装载使能
      TIM1_OC4PreloadConfig(ENABLE);  
      break;
  default:
    break;
  }
  //缓冲写入ARR
  TIM1_ARRPreloadConfig(ENABLE);
  //输出
  TIM1_CtrlPWMOutputs(ENABLE);
  //立即更新预分频值
  TIM1_PrescalerConfig(ps, TIM1_PSCRELOADMODE_IMMEDIATE);
  //开定时器
  TIM1_Cmd(ENABLE);

}

//定时器1的PWM占空比改变
//输入参数：PWM结构体指针
//输出参数：无
void T1_PWM_Duty(PWM_Typedef *PWMx)
{
u16 cmp;
  //计算新比较值
  cmp=(u16)(((((u16)(TIM1->ARRH))<<8)+(TIM1->ARRL)+1)*(PWMx->duty)+0.5);
  //根据不同通道设置占空比
  switch(PWMx->ch)
  {
    case 1:
      TIM1->CCR1H = (uint8_t)(cmp >> 8);
      TIM1->CCR1L = (uint8_t)(cmp);
      break;
    case 2:
      TIM1->CCR2H = (uint8_t)(cmp >> 8);
      TIM1->CCR2L = (uint8_t)(cmp);
      break;
    case 3:
      TIM1->CCR3H = (uint8_t)(cmp >> 8);
      TIM1->CCR3L = (uint8_t)(cmp);
      break;
    default:
      break;
  } 
}

//定时器2的PWM初始化,不包含引脚重映射，需在之前设置选项字
//输入参数：PWM结构体指针
//输出参数：无
void T2_PWM_Init(PWM_Typedef *PWMx)
{
    u8 ps ;	//预分频值
    u16 preload,dutytick;
  //根据要产生的频率计算分频值,除最高档(4K以上)外其它档保证12位以上的精度
  if(PWMx->f>((SYSCLK>>16)+1))	//16M时245HZ以上	
    ps=0;
  else if(PWMx->f>((SYSCLK>>20)+1))	//16~245
    ps=4;
  else   //0.95~16,低于0.95Hz则输出不正确
    ps=8;
  preload=(u16)((SYSCLK>>ps)/(PWMx->f));
  dutytick=(u16)(preload*(PWMx->duty)+0.5);
  //初始化时基
  TIM2_TimeBaseInit(ps, (u16)(preload-0.5));
  //PWM输出初始化
  switch(PWMx->ch)
  {
    case 1:
      TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,\
        dutytick, TIM2_OCPOLARITY_HIGH);
      TIM2_OC1PreloadConfig(ENABLE); 
      break;
    case 2:
      TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,\
        dutytick, TIM2_OCPOLARITY_HIGH);
      TIM2_OC2PreloadConfig(ENABLE);  
      break;
    case 3:
      TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE,\
        dutytick, TIM2_OCPOLARITY_HIGH);
      TIM2_OC3PreloadConfig(ENABLE); 
    break;
    default:
    break;
  }   
  TIM2_Cmd(ENABLE);
}

//定时器2的PWM占空比改变
//输入参数：PWM结构体指针
//输出参数：无
void T2_PWM_Duty(PWM_Typedef *PWMx)
{
u16 cmp;
  //计算新比较值
  cmp=(u16)(((((u16)(TIM2->ARRH))<<8)+(TIM2->ARRL)+1)*(PWMx->duty)+0.5);
  //根据不同通道设置占空比
  switch(PWMx->ch)
  {
    case 1:
      TIM2->CCR1H = (uint8_t)(cmp >> 8);
      TIM2->CCR1L = (uint8_t)(cmp);
      break;
    case 2:
      TIM2->CCR2H = (uint8_t)(cmp >> 8);
      TIM2->CCR2L = (uint8_t)(cmp);
      break;
    case 3:
      TIM2->CCR3H = (uint8_t)(cmp >> 8);
      TIM2->CCR3L = (uint8_t)(cmp);
      break;
    default:
      break;
  } 
}


#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) ||defined(STM8S105) ||\
    defined(STM8S005) ||  defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8AF626x)
//定时器3的PWM初始化,不包含引脚重映射，需在之前设置选项字
//输入参数：PWM结构体指针
//输出参数：无
void T3_PWM_Init(PWM_Typedef *PWMx)
{
    u8 ps ;	//预分频值
    u16 preload,dutytick;
  //根据要产生的频率计算分频值,除最高档(4K以上)外其它档保证12位以上的精度
  if(PWMx->f>((SYSCLK>>16)+1))	//16M时245HZ以上	
    ps=0;
  else if(PWMx->f>((SYSCLK>>20)+1))	//16~245
    ps=4;
  else   //0.95~16,低于0.95Hz则输出不正确
    ps=8;
  preload=(u16)((SYSCLK>>ps)/(PWMx->f));
  dutytick=(u16)(preload*(PWMx->duty)+0.5);
  //初始化时基
  TIM3_TimeBaseInit(ps, (u16)(preload-0.5));
  //PWM输出初始化
  switch(PWMx->ch)
  {
    case 1:
      TIM3_OC1Init(TIM3_OCMODE_PWM1, TIM3_OUTPUTSTATE_ENABLE,\
        dutytick, TIM3_OCPOLARITY_HIGH);
      TIM3_OC1PreloadConfig(ENABLE); 
      break;
    case 2:
      TIM3_OC2Init(TIM3_OCMODE_PWM1, TIM3_OUTPUTSTATE_ENABLE,\
        dutytick, TIM3_OCPOLARITY_HIGH);
      TIM3_OC2PreloadConfig(ENABLE);  
      break;
    default:
    break;
  }   
  TIM3_Cmd(ENABLE);
}

//定时器3的PWM占空比改变
//输入参数：PWM结构体指针
//输出参数：无
void T3_PWM_Duty(PWM_Typedef *PWMx)
{
u16 cmp;
  //计算新比较值
  cmp=(u16)(((((u16)(TIM3->ARRH))<<8)+(TIM3->ARRL)+1)*(PWMx->duty)+0.5);
  //根据不同通道设置占空比
  switch(PWMx->ch)
  {
    case 1:
      TIM3->CCR1H = (uint8_t)(cmp >> 8);
      TIM3->CCR1L = (uint8_t)(cmp);
      break;
    case 2:
      TIM3->CCR2H = (uint8_t)(cmp >> 8);
      TIM3->CCR2L = (uint8_t)(cmp);
      break;
    default:
      break;
  } 
}

#endif
