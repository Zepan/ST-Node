/********************************************************************************
 * 文件名  ：adc.c
 * 描述    ：adc相关函数    
 * 库版本  ：V2.0.0
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2012-9-10
**********************************************************************************/
#define ADC_GLOBAL
#include "adc.h"

//ADC1单次转换
//输入参数：转换通道(0~15)
//输出参数：无
//备注：默认时钟8分频，数据右对齐
void ADC1_Init_Single(u8 ch)
{
    //设置数据右对齐
    ADC1->CR2 |= (uint8_t)(ADC1_ALIGN_RIGHT);
	//设置为单次转换模式
    ADC1->CR1 &= (uint8_t)(~ADC1_CR1_CONT);
	//选择要转换的通道
    ADC1->CSR &= (uint8_t)(~ADC1_CSR_CH);
    ADC1->CSR |= (uint8_t)(ch);
	//设置时钟预分频，默认8分频
    ADC1->CR1 &= (uint8_t)(~ADC1_CR1_SPSEL);
   	ADC1->CR1 |= (uint8_t)(ADC1_PRESSEL_FCPU_D8);
	//失能外部触发
	ADC1->CR2 &= (uint8_t)(~ADC1_CR2_EXTTRIG);
	//失能斯密特触发器，降低功耗
	if(ch<8)ADC1->TDRL |=(u8)(0x01)<<ch;
	else ADC1->TDRH |=(u8)(0x01)<<(ch-8);
	//使能ADC1
	ADC1->CR1 |= ADC1_CR1_ADON;  
}

//ADC1取得单次转换的结果
//输入参数：存储转换结果的地址
//输出参数：无
//备注：默认右对齐
void ADC1_Get_Single(u16 *data)
{
  u8 tmp;
  	//启动一次转换
	ADC1->CR1 |= ADC1_CR1_ADON;
	//等待转换结束
	while(!(ADC1->CSR&ADC1_FLAG_EOC));
	//清标志	
	ADC1->CSR&=~ADC1_FLAG_EOC;
	tmp = ADC1->DRL;
	*data =(ADC1->DRH);
	*data =(*data<<8)|tmp;
}

//ADC1单次扫描模式转换
//输入参数：转换截止通道(0~n扫描)
//输出参数：无
//备注：默认时钟8分频，数据右对齐
void ADC1_Init_Scan(u8 ch)
{
    //设置数据右对齐
    ADC1->CR2 |= (uint8_t)(ADC1_ALIGN_RIGHT);
	//设置为单次转换模式
    ADC1->CR1 &= (uint8_t)(~ADC1_CR1_CONT);
	//选择要转换的截止通道
    ADC1->CSR &= (uint8_t)(~ADC1_CSR_CH);
    ADC1->CSR |= (uint8_t)(ch);
	//设置扫描模式
	ADC1->CR2 |= ADC1_CR2_SCAN;
	//设置时钟预分频，默认8分频
    ADC1->CR1 &= (uint8_t)(~ADC1_CR1_SPSEL);
   	ADC1->CR1 |= (uint8_t)(ADC1_PRESSEL_FCPU_D8);
	//失能外部触发
	ADC1->CR2 &= (uint8_t)(~ADC1_CR2_EXTTRIG);
	//失能斯密特触发器，降低功耗
	if(ch<8)ADC1->TDRL |=(u8)(0x01)<<ch;
	else ADC1->TDRH |=(u8)(0x01)<<(ch-8);
	//使能ADC1
	ADC1->CR1 |= ADC1_CR1_ADON;   
}

//ADC1获取单次扫描模式结果
//输入参数：存储结果的数组地址，通道总数
//输出参数：无
//备注：默认时钟8分频，数据右对齐
void ADC1_Get_Scan(u16* data,u8 n)
{
  u8 i,tmp;
  	//启动一次转换
	ADC1->CR1 |= ADC1_CR1_ADON;
	//等待转换结束
	while(!(ADC1->CSR&ADC1_FLAG_EOC));
	//清标志	
	ADC1->CSR&=~ADC1_FLAG_EOC;
	//存储值
	for(i=0;i<=n;i++)
	{
		tmp = *(&(ADC1->DB0RL)+i*2);	//右对齐先读低位
		data[i] = *(&(ADC1->DB0RH)+i*2);
		data[i]= (data[i]<<8)|tmp;
	}
}