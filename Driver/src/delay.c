/********************************************************************************
 * 文件名  ：delay.c
 * 描述    ：延时函数     
 * 库版本  ：V2.0.0
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2012-7-26
**********************************************************************************/
#define DELAY_GLOBAL
#include "delay.h"

//定时器4产生滴答时钟
//输入参数：无
//输出参数：无
//备注：16M时钟时，输出488Hz，近似为2ms一次(2.048)
void T4Tick_Init(void)
{
  TIM4->ARR = 0xff;	//自动重装值
  TIM4->PSCR = (uint8_t)(TIM4_PRESCALER_128);	//128分频
  TIM4->EGR = (uint8_t)TIM4_PSCRELOADMODE_IMMEDIATE;	//立即更新分频值
  TIM4->IER |= (uint8_t)TIM4_IT_UPDATE;	//允许更新中断
  TIM4->CR1 |= TIM4_CR1_CEN;	//运行
  systick=0;
  
}

//软件延时毫秒级时间
//输入参数：毫秒数
//输出参数：无
//备注：自适应晶振频率
void Soft_DelayMs(u16 t)
{
u16 i;
  for(;t;t--)
  {
    for(i=TICK_MS;i;i--);
  }
}

//软件延时微秒级时间
//输入参数：微秒数
//输出参数：无
//备注：自适应晶振频率,不能超过20ms(t溢出）
void Soft_DelayUs(u16 t)
{
  for(t=t*TICK_US/4;t;t--)
  {
  }
  
}

//定时器延时毫秒级时间初始化,只能用TIM1
//输入参数：无
//输出参数：无
//备注：自适应晶振频率
void DelayMs_Init(void)
{
    TIM1_DeInit();
	//向上计数，自动重装值为最大值
  TIM1_TimeBaseInit(DIV_MS,TIM1_COUNTERMODE_UP,0xffff,0);	
	//缓冲写入ARR
  TIM1_ARRPreloadConfig(ENABLE);
  // 软件产生更新事件，用于更新PSCR寄存器
  TIM1->EGR = 0x01;                 

}

//定时器延时毫秒级时间
//输入参数：延时毫秒数
//输出参数：无
//备注：自适应晶振频率
void DelayMs(u16 t)
{
	TIM1_SetCounter(0x0000);	//清零
	TIM1->CR1 |= TIM1_CR1_CEN;	//开
	while(TIM1_GetCounter()<t);	//延时
	TIM1->CR1 &= (uint8_t)(~TIM1_CR1_CEN);	//关
}

#ifdef USE_T1

//定时器延时微秒级时间初始化
//输入参数：无
//输出参数：无
//备注：自适应晶振频率
void DelayUs_Init(void)
{
	TIM1_DeInit();
	//向上计数，自动重装值为最大值
  TIM1_TimeBaseInit(DIV_US,TIM1_COUNTERMODE_UP,0xffff,0);	
	//缓冲写入ARR
  TIM1_ARRPreloadConfig(ENABLE);
    // 软件产生更新事件，用于更新PSCR寄存器
  TIM1->EGR = 0x01;  
}

//定时器延时微秒级时间
//输入参数：延时微秒数
//输出参数：无
//备注：自适应晶振频率
void DelayUs(u16 t)
{
    if(t<9)return;
  else
  {
    t-=8; //除去本身调用所花时间
	TIM1_SetCounter(0x0000);	//清零
	TIM1->CR1 |= TIM1_CR1_CEN;	//开
	while(TIM1_GetCounter()<t);	//延时
	TIM1->CR1 &= (uint8_t)(~TIM1_CR1_CEN);	//关
  }
}

#elif	defined(USE_T2)

//定时器延时微秒级时间初始化
//输入参数：无
//输出参数：无
//备注：自适应晶振频率
void DelayUs_Init(void)
{
	TIM2_DeInit();
	//向上计数，自动重装值为最大值
  TIM2_TimeBaseInit(DIV_US,0xffff);	
	//缓冲写入ARR
  TIM2_ARRPreloadConfig(ENABLE);
    // 软件产生更新事件，用于更新PSCR寄存器
  TIM2->EGR = 0x01;  
}

//定时器延时微秒级时间
//输入参数：延时微秒数
//输出参数：无
//备注：自适应晶振频率
void DelayUs(u16 t)
{
  if(t<9)return;
  else
  {
    t-=8; //除去本身调用所花时间
	TIM2_SetCounter(0x0000);	//清零
	TIM2->CR1 |= TIM2_CR1_CEN;	//开
	while(TIM2_GetCounter()<t);	//延时
        TIM2->CR1 &= (uint8_t)(~TIM2_CR1_CEN);	//关
  }
}

#elif defined(USE_T3)

//定时器延时微秒级时间初始化
//输入参数：无
//输出参数：无
//备注：自适应晶振频率
void DelayUs_Init(void)
{
	TIM3_DeInit();
	//向上计数，自动重装值为最大值
  TIM3_TimeBaseInit(DIV_US,0xffff);	
	//缓冲写入ARR
  TIM3_ARRPreloadConfig(ENABLE);
    // 软件产生更新事件，用于更新PSCR寄存器
  TIM3->EGR = 0x01;  
}

//定时器延时微秒级时间
//输入参数：延时微秒数
//输出参数：无
//备注：自适应晶振频率
void DelayUs(u16 t)
{
    if(t<9)return;
  else
  {
    t-=8; //除去本身调用所花时间
	TIM3_SetCounter(0x0000);	//清零
	TIM3->CR1 |= TIM3_CR1_CEN;	//开
	while(TIM3_GetCounter()<t);	//延时
	TIM3->CR1 &= (uint8_t)(~TIM3_CR1_CEN);	//关
  }
}

#endif

