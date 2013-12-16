#include "DHT11.h"

//Pin1 VDD 3~5V
//Pin2 DATA
//Pin4 GND
//注意是从网格那面看

#define DAT_PORT  GPIOD
#define DAT_PIN   GPIO_PIN_3

//读取一次温湿度
//dat[0]=温度，dat[1]=湿度
//返回成功0或失败1
u8 Get_DHT11(u8 *dat)
{
 u8 i,j;
 u32 t;
 u8 tmp[5]={0,0,0,0,0};
  //起始条件，拉低>18ms
  GPIO_Init(DAT_PORT,DAT_PIN,GPIO_MODE_OUT_PP_LOW_SLOW);
  Soft_DelayMs(20);
  //等待响应
  GPIO_Init(DAT_PORT,DAT_PIN, GPIO_MODE_IN_PU_NO_IT);
  Soft_DelayUs(30);
  if((DAT_PORT->IDR&(DAT_PIN))) //未响应，仍是高
  {
    return 1;
  }
  else  //响应
  {
    //读取数据
    t=systick;
    while((DAT_PORT->IDR&(DAT_PIN))==0 && systick-t<2); //等待响应信号结束  
    if(systick-t>=2)return 1;
    t=systick;
    while((DAT_PORT->IDR&(DAT_PIN)) && systick-t<2); //等待低电平时隙到来
    if(systick-t>=2)return 1;
    for(i=0;i<5;i++)
    {
      for(j=0;j<8;j++)
      {
         t=systick;
         while((DAT_PORT->IDR&(DAT_PIN))==0 && systick-t<2); //等待低电平时隙结束 
         if(systick-t>=2)return 1;
         Soft_DelayUs(40);  //0,28us高电平；1,,70us高电平
         tmp[i]=tmp[i]<<1;
         if((DAT_PORT->IDR&(DAT_PIN)))  //如果还是高，该位为1
         {
            tmp[i]++;
            t=systick;
            while((DAT_PORT->IDR&(DAT_PIN)) && systick-t<2); //等待低电平时隙到来
            if(systick-t>=2)return 1;
         }
      }
    }
    //验证数据，保存
    if(tmp[0]+tmp[1]+tmp[2]+tmp[3]!=tmp[4]) //校验失败
    {
     return 1; 
    }
    else
    {
     dat[0]=tmp[0];
     dat[1]=tmp[2];
     return 0;
    }
  }
}

