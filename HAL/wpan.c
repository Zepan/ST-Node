#include "wpan.h"
#include "delay.h"
#include <string.h>
#include "app.h"

//WPAN密匙
#define WPAN_KEY_LEN 8
const u8 wpan_key[WPAN_KEY_LEN]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
const u8 bc_addr[WPAN_ADDR_SIZE]={0xff,0xff,0xff,0xff,0xff};
const u8 mytype=DEV_HUMITURE;
const u8 myaddr[WPAN_ADDR_SIZE]={0x02,0xff,0xff,0xff,0xff};
u8 myid;
//  通道， 地址， 频率
static NRF24L01_CH_TypeDef mych;    //本节点地址
static NRF24L01_CH_TypeDef ch;   //发送/接收使用的节点地址
 pkt_t wpan_pkt;
static u8 wpan_state=0;

extern attr_func attr_list[];

//大端转小端
void b2l(u8* big)
{
   *(big) ^= *(big+3)^= *(big) ^= *(big+3);
   *(big+1) ^= *(big+2)^= *(big+1) ^= *(big+2);
}

/* 属性处理demo
//属性1处理，temp
u8 attr1(u8 rw, dat_t* dat)
{
  if(!rw) { //R0
    dat->u32 = dht[1];
    b2l((u8 *)(&(dat->u32)));
  }
  return 1;
}
*/

//发送数据包， 发送完成后自动返回监听自己的通道
//0,ok;1,err
 u8 send_pkt(pkt_t * pkt)
{
    u32 t;
    u8 res;
    TX_Mode(&ch);//准备发射//130us
    t = systick;
    do {
        res = NRF24L01_TxPacket((u8 *)pkt);//发送数据 //发送次数超限用时10.7ms//发送成功830us
    }while(res != TX_DS && (systick - t < 10));//20ms内不停尝试//5us 
    RX_Mode(&mych);//最后返回接收模式//100us
    return (res != TX_DS);
}

//接收数据包
//0,ok;1,err
u8 rec_pkt(pkt_t * pkt)
{
    u32 t;
    u8 res = 1;
    RX_Mode(&ch);   //使用对端地址接收
    t = systick;
    while(systick - t < 10 && NRF24L01_IRQ); //等待20ms，读取返回值
    if(systick - t < 10)
    {
        NRF24L01_RxPacket((u8 *)pkt); //170us读取
        res = 0;
    }
    RX_Mode(&mych);//最后返回本机接收模式
    return res;
}

//处理接收到的数据包
//返回:1,读属性，返回属性值；0，写属性，不用返回
static u8 deal_pkt(pkt_t * pkt)
{
u8 i,j,flag;
u8 attr;
u8 rw;
  switch(pkt->header) {
  case WPAN_RW:
    for(i = 0, j = 0, flag = 0; i < WPAN_CMD_N && pkt->pl.cmd[i].type != 0 ; i++) {
      attr = (pkt->pl.cmd[i].type) & WPAN_ATTR_MASK;
      rw = (pkt->pl.cmd[i].type) & WPAN_RW_MASK;
      if(!rw) {     //读命令
        flag=1;     //表示需要返回数据 
      }
      if(attr <= ATTR_N) { //有效attr
        attr_list[attr-1](rw, &(pkt->pl.cmd[i].dat));//由属性函数在原位置读取/写入数据，后面再转移
      }
      if(j != i) {  //往前挪命令
        memcpy(&pkt->pl.cmd[j], &pkt->pl.cmd[i], WPAN_CMD_LEN);
      }
      j++;
    }
    break;
  case WPAN_INVITE:
    if(wpan_state==0 && memcmp(pkt->pl.u8, wpan_key, WPAN_KEY_LEN) == 0) {//未连接且密匙正确
      flag=1;//要返回dev_type和addr
      pkt->header = WPAN_JOIN;
      pkt->pl.u8[0] = mytype;//设备类型
      memcpy(pkt->pl.u8 + 1, myaddr, WPAN_ADDR_SIZE);//地址
    }
    break;
  case WPAN_ASSIGN:
    if(wpan_state == 0) {//未连接
      flag = 0;
      pkt->header = WPAN_JOIN;
      myid = pkt->pl.u8[0];//设备id
    }
    break;
  default:
    break; 
  }
  return flag;
}


void wpan_init(NRF24L01_CH_TypeDef *init_ch)
{
    NRF24L01_Init();
    while(NRF24L01_Check());
    memcpy(&mych, init_ch, sizeof(NRF24L01_CH_TypeDef));
    CH_USE(&mych);
    RX_Mode(&ch);
}

//主循环检测数据包
//返回是否成功处理包，0成功，1失败或无数据
u8 deal_wpan(void)  //wpan检查是否有包，及处理
{
    if(NRF24L01_IRQ==0) {   //收到包
        NRF24L01_RxPacket((u8 *)&wpan_pkt); //170us读取
        if(deal_pkt(&wpan_pkt)) {   //返回1表示读信息，要返回数据//40us   
            return send_pkt(&wpan_pkt);
        }
        return 0;
    }
    return 1;
}

u8 wpan_readAttr(u8* addr, u8 attr, u32* val)
{
    memcpy(ch.addr, addr, WPAN_ADDR_SIZE);
    wpan_pkt.header = WPAN_RW;
    wpan_pkt.reserve = 0;
    WPAN_CMD_TYPE(0) = attr & WPAN_ATTR_MASK;
    WPAN_CMD_DAT(0) = *val; //作为附加参数，可选
    if(send_pkt(&wpan_pkt)) return 1;   //发送失败
    if(!rec_pkt(&wpan_pkt)) {    //收到包
        *val = WPAN_CMD_DAT(0); //TODO: check pkt
        return 0;
    }
	else {
		return 1;   //超时未收到
	}	  
}

u8 wpan_writeAttr(u8* addr, u8 attr, u32* val)
{
    memcpy(ch.addr, addr, WPAN_ADDR_SIZE);
    wpan_pkt.header = WPAN_RW;
    wpan_pkt.reserve = 0;
    WPAN_CMD_TYPE(0) = WPAN_RW_MASK | (attr & WPAN_ATTR_MASK);
    WPAN_CMD_DAT(0) = *val;
    return send_pkt(&wpan_pkt); 
}
