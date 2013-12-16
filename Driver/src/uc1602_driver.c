/***************************************************
@author:吴才泽
@date:2012-1-11
@description: uc1602驱动函数
******************************************************/
#include "uc1602_driver.h"  

/***********************************************************
@function:  Com_Head(void)
@description:发送命令头,写命令
@input:      void
@output:     void
************************************************************/
static void Com_Head(void)
{
	i2c_Start();
	i2c_SendByteA(0x78);
}

/***********************************************************
@function:  WD_Head(void)
@description:发送数据头,写数据
@input:      void
@output:     void
************************************************************/
static void WD_Head(void)
{
	i2c_Start();
	i2c_SendByteA(0x7a);
}

/***********************************************************
@function:  RD_Head(void)
@description:发送数据头,读数据
@input:      void
@output:     void
************************************************************/
static void RD_Head(void)
{
	i2c_Start();
	i2c_SendByteA(0x7b);
}

/***********************************************************
@function:  Init_UC1602(void)
@description:初始化
@input:      void
@output:     void
************************************************************/
void UC1602_Init(void)
{
        while(i2c_CheckDevice(0x7b));
 	Com_Head();
	i2c_SendByteA(0x0e2);   //复位
	i2c_Stop();
	Soft_DelayMs(20);
	Com_Head();
			//显示开  默认是关 这里一定要设置
	i2c_SendByteA(0x0af);
		//全显示    A5 全显示  A4关全显示
	i2c_SendByteA(0x0a4);
		//结束
	i2c_Stop();
	UC1602_Clear();
//	Init_GBK1616();//初始化字库
//	//电源控制
//	i2c_SendByteA(0x2e);  
//	//温度补偿 
//	i2c_SendByteA(0x26);
//	//驱动电压   
//	i2c_SendByteA(0x81);   //固定字节
//	i2c_SendByteA(0x62);   //对比度，0~255，默认62H

//	//LCD 偏置
//	i2c_SendByteA(0x0eb);   //LCD 偏置 EB=1/9  默认
//	//AC 页列自动增加 及增加方向 默认89
//	i2c_SendByteA(0x89);
//	//部分显示 84禁止 85 允许
//	i2c_SendByteA(0x84);
//	//卷轴起始行，设置起始行 范围0-63 特效用，可不管
//	i2c_SendByteA(0x40);


//	//MX MY 数据顺序  默认C0 ； CC 是倒的 ；  c8是倒的镜像 c4是正的镜像
//	i2c_SendByteA(0x0c0);

//	// 反转显示 A6 不反转 A7 反转
//	i2c_SendByteA(0x0a6);
//	//COM END ，设置结束电极
//	i2c_SendByteA(0x0f1);
//	i2c_SendByteA(0x03f);
}

/***********************************************************
@function:  Clear_UC1602(void)
@description:清屏
@input:      void
@output:     void
************************************************************/
void UC1602_Clear(void)
{
u8 i,j;
	Com_Head();
		//列设置
	i2c_SendByteA(0x00);   //列低四位   0000****	 
	i2c_SendByteA(0x10);   //列高三位位 00010***
		//设置起始页地址， 共8页
	i2c_SendByteA(0x0b0);  //页地址		1011****
	i2c_Stop();
	WD_Head();
	for(i=0;i<9;i++)
	{
		for(j=0;j<102;j++)
			i2c_SendByteA(0x00);
	}
	i2c_Stop();
}

/***********************************************************
@function:  UC1602_ClearRec(u8 xs,u8 xe,u8 ys,u8 ye)
@description:清矩形区域
@input:      xs xe ys ye
@output:     void
************************************************************/
void UC1602_ClearRec(u8 xs,u8 xe,u8 ys,u8 ye)
{
u8 x,y;
	for(y=ys;y<=ye;y++)
	{
		set_xy(xs,y);
		WD_Head();
		for(x=xs;x<=xe;x++)
		{
			i2c_SendByteA(0x00);
		}	
	}


}

/***********************************************************
@function:  disp_85(u8 *p,u8 x,u8 y)
@description:显示8*5的ASCII字符
@input:      p ASCII码数组指针,x=0~95,y=0~8
@output:     返回显示完成后光标指向第几行
************************************************************/
u8 UC1602_Disp85(u8 *p,u8 x,u8 y)
{
u8 i,j,k;
	k=0;	//自动换行数
	x%=96;
	y%=8;
	set_xy(x,y);
	WD_Head();
	for(i=0;p[i]>=32&&p[i]<=126;i++)
	{
		if(WRAP&&(x+i*(5+GAP85)-102*k)>91) //如果自动换行开启则填充0至下一行
		{
			for(j=0;j<102*(k+1)-x-i*(5+GAP85);j++)
				i2c_SendByteA(0x00);	//填充0
			k+=1;
		}

		for(j=0;j<5;j++){
	 		i2c_SendByteA(ASCII85[p[i]-0x20][j]);	//查ASCII码表，写字符
		}
		for(j=0;j<GAP85;j++){
			i2c_SendByteA(0x00);		//填充字符间隔
		}
	}
	i2c_Stop();
	return y+k;
}

/***********************************************************
@function:  disp_a(u8 p,u8 x,u8 y)
@description:显示一个8*5的ASCII字符
@input:      p ASCII码,x=0~95,y=0~8
@output:     返回显示完成后光标指向第几行
************************************************************/
void UC1602_DispA(u8 p,u8 x,u8 y)
{
u8 j;
	x%=96;
	y%=8;
	set_xy(x,y);
	WD_Head();
		for(j=0;j<5;j++){
	 		i2c_SendByteA(ASCII85[p-0x20][j]);	//查ASCII码表，写字符
		}
		for(j=0;j<GAP85;j++){
			i2c_SendByteA(0x00);		//填充字符间隔
		}
	i2c_Stop();
}

/***********************************************************
@function:  disp_GBK(u8 *p,u8 x,u8 y)
@description:显示16*16的汉字
@input:      p ASCII码数组指针,x=0~95,y=0~8
@output:     返回显示完成后光标指向第几行
************************************************************/
u8 UC1602_DispGBK(u8 *p,u8 x,u8 y)
{
u8 i,j;
u8 index;//字符在索引表的下标
u16 GBKcode;
	while(x>95||y>8);//出错则停在这
	set_xy(x,y);
	for(i=0;p[i];i+=2)
	{
		if(WRAP&&x>(80-GAPGBK)) //如果自动换行开启则光标移至下一行
		{
			y+=2;
			x=0;
		}
		GBKcode=p[i]+(p[i+1]<<8);
		for(index=0;GBK1616_ref[index*2]!='0';index++){
			if(((u16)GBK1616_ref[index])==GBKcode)break;//此处要做u8->u16的转换,*((u16*)(GBK1616_ref+index))
		}	//在索引表里查找位置

		set_xy(x,y);
		WD_Head();
		for(j=0;j<16;j++){
	 		i2c_SendByteA(*(GBK1616+index*32+j));	//查ASCII码表，写字符上半部分
		}
		for(j=0;j<GAPGBK;j++){
			i2c_SendByteA(0x00);		//填充字符间隔
		}
		i2c_Stop();
		set_xy(x,y+1);
		WD_Head();
		for(j=16;j<32;j++){
	 		i2c_SendByteA(*(GBK1616+index*32+j));	//查ASCII码表，写字符下半部分
		}
		for(j=0;j<GAPGBK;j++){
			i2c_SendByteA(0x00);		//填充字符间隔
		}
		i2c_Stop();
		x+=(16+GAPGBK);
	}
	return y;
}

/***********************************************************
@function:  u8 disp_pic(u8 *p,u8 x,u8 y,u8 l,u8 h)
@description:在x,y处画图 ,每8行取模，再下8行取模。
@input:      p,图的数组地址，x,y,l列数，即宽，h行数，即高
@output:     y，最后的y值
************************************************************/
u8 UC1602_DispPic(u8* p,u8 x,u8 y,u8 l,u8 h)
{
u16 index;
u8 i,j;
	for(i=0;i<(h/8+1-!(h%8));i++)
	{
		set_xy(x,y);
		WD_Head();
		for(j=0;j<l;j++)
		{
			index=i*l+j;
	 		i2c_SendByteA(p[index]);
		}
		i2c_Stop();
		y++;
	}
	return y;

}


/***********************************************************
@function:  set_xy(u8 x,u8 y)
@description:光标移至x,y
@input:      x,y
@output:     void
************************************************************/
static void set_xy(u8 x,u8 y)
{
u8 tmp;
	do{
	tmp=0;
 	//Com_Head();
	i2c_Start();
	tmp|=i2c_SendByteA(0x78);
		//列设置
	tmp|=i2c_SendByteA(x&0x0f);   //列低四位   0000****	 
	tmp|=i2c_SendByteA((x>>4)|0x10);   //列高三位位 00010***
		//设置起始页地址， 共8页
	tmp|=i2c_SendByteA(y|0x0b0);  //页地址		1011****
	i2c_Stop();
	}while(tmp);
}

/***********************************************************
@function:  UC1602_Scroll(u8 n)
@description:向上卷屏
@input:      卷n行
@output:     void
************************************************************/
void UC1602_Scroll(u8 n)
{
	Com_Head();
	i2c_SendByteA(0x40|n);
	i2c_Stop();
}

//设置某位置的点
void UC1602_SetP(u8 x,u8 y,u8 color)
{
u8 tmp;
	//设置坐标
 	set_xy(x,y/8);
	//读原来的数据
	RD_Head();
	tmp=i2c_ReadByteA();
	i2c_Stop();
	//写数据
	if(color)tmp|=(0x01<<(y%8));
	else tmp&=~(0x01<<(y%8));
	set_xy(x,y/8);
	WD_Head();
	i2c_SendByteA(tmp);
	i2c_Stop();
}

//获取某位置点信息
u8 UC1602_GetP(u8 x,u8 y)
{
u8 tmp;
	//设置坐标
 	set_xy(x,y/8);
	//读原来的数据
	RD_Head();		
	tmp=i2c_ReadByteA();
	i2c_Stop();
	tmp=(tmp>>(y%8))&0x01;
	return tmp;
}

//反色某位置点信息
void UC1602_XorP(u8 x,u8 y)
{
u8 tmp;
	//设置坐标
 	set_xy(x,y/8);
	//读原来的数据
	RD_Head();
	tmp=i2c_ReadByteA();
	i2c_Stop();
	//反色
	tmp^=(0x01<<(y%8));
	set_xy(x,y/8);
	WD_Head();
	i2c_SendByteA(tmp);
	i2c_Stop();
}

void UC1602_SetPage(u8 x,u8 y,u8 dat)
{
	//设置坐标
 	set_xy(x,y/8);
	WD_Head();
	i2c_SendByteA(dat);
	i2c_Stop();
}

//void UC1602_Test(void)
//{
//u8 i,j,tmp;
//
//}

//整数转化为字符串
/**
* @brief  整数转化为字符串，可以为负数
* @param    
* @retval 字符串长度
*/   
u8 Int2Str(int num,u8 *str)
{
u8 i,j;
u32 tmp=abs(num);
	for(i=0;;i++)	//num有几位
	{
		tmp/=10;
	 	if(!tmp)break;		
	}
	if(num<0)i++;
	tmp=abs(num);
	for(j=0;j<=i;j++)
	{
	 	str[i-j]=tmp%10+'0';
		tmp/=10;
	}
	if(num<0)str[0]='-';
	str[i+1]=0;
	return i+1;
}

//浮点数转字符串
/****************************************************************************
* 名    称：void Float2Str(float a,char * str ,u8 len)
* 功    能：浮点数转字符串
* 入口参数：a	待转换的浮点数
			str 数组的首地址
			len 长度
* 出口参数：无
* 说    明：默认包含小数点，只能转正数
****************************************************************************/
const int pow10[]={1,10,100,1000,10000,100000,1000000,10000000,100000000};
void Float2Str(float a,char * str ,u8 len)
{
u8 i,j;
u16 b=(u16)a;
	for(i=0;;i++)
	{
		b/=10;
	 	if(!b)break;
	}	//退出后i为a的位数-1
	b=(u16)a;
	for(j=0;len&&(i!=0xff);len--,i--,j++)
	{
	 	str[j]=(b/pow10[i])%10+'0';
	}
	if(len)
	{
		str[j]='.';
		j++;
	 	a-=b;
		a*=pow10[len];
		b=(u16)a;
		for(;len;len--,j++)
		{
		 	str[j]=(b/pow10[len-1])%10+'0';
		}
	}
	str[j]=0;
}


//指定位置显示整数,左对齐，最长显示len个字符，不足则后面补空格
void UC1602_DispInt(int num,u8 x,u8 y,u8 len)
{
char str[12];
u8 tmp;
  tmp=Int2Str(num,str);
  if(tmp<len) {
    memset(str+tmp,' ',len-tmp); //填充空格
  }
  str[len]=0;  //截断 
  UC1602_Disp85(str,x,y);
  return;
}


//指定位置显示浮点数