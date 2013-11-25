/********************************************************************************
 * 文件名  ：flash.c
 * 描述    ：flash操作相关函数     
 * 库版本  ：V2.0.0
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2012-7-27
**********************************************************************************/

#include "flash.h"

//设置引脚功能复用选项字OPT2
//输入参数：要设置的位(BIT0~7)，置位or清零(1,0)
//输出参数：无
void Cfg_OPT2(u8 bit_n,u8 opt)
{
  uint16_t status;/*记录原来的备选功能状态*/
  status=FLASH_ReadOptionByte(OPT2_ADDR);
  //此处可检查是否读到FLASH_OPTIONBYTE_ERROR
  //读到的高8位是OPT2，低8位是NOPT2
  status>>=8;
  if(status&bit_n)	//原来是置位
  {
	if(!opt)	//需要清零
    {
		FLASH_ProgramOptionByte(OPT2_ADDR, (uint8_t)(status&(~bit_n)));
    }
  }
  else	//原来是清零
  {
	if(opt)	//需要置位
    {
		FLASH_ProgramOptionByte(OPT2_ADDR, (uint8_t)(status|bit_n));
    }
  }
}


    
