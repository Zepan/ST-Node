#ifndef __WPAN_H
#define __WPAN_H
	 		  
//CPU相关的头文件
#include "common.h"
#include "24l01.h"

//地址长度
#define WPAN_ADDR_SIZE 5

//一帧结构
#define PKT_SIZE 32
#define WPAN_HEAD_LEN 2
#define WPAN_CMD_LEN 5
//消息缓存首字节，表示类型
#define WPAN_RW 1
#define WPAN_IAP 2
#define WPAN_INVITE 3
#define WPAN_JOIN 4
#define WPAN_ASSIGN 5
#define WPAN_CUSTOM 6
#define WPAN_ERROR 0xff	//未知类型
//消息缓存第二字节，保留
//消息中命令个数
#define WPAN_CMD_N ((PKT_SIZE-WPAN_HEAD_LEN)/WPAN_CMD_LEN)

//消息缓存中命令的首字节
#define WPAN_RW_MASK 0x80	//R0/W1
    #define WPAN_ATTR_READ 0
    #define WPAN_ATTR_WRITE 1 
#define WPAN_FORMAT_MASK 0x40 //d0/f1
#define WPAN_ATTR_MASK 0x3f

//消息简写
#define WPAN_CMD_TYPE(x) (wpan_pkt.pl.cmd[(x)].type)
#define WPAN_CMD_DAT(x)  (wpan_pkt.pl.cmd[(x)].dat.u32)

//操作简写
#define CH_USE(x)        do{memcpy(&ch, x, sizeof(NRF24L01_CH_TypeDef));}while(0);

enum DEV_TYPE
{
	DEV_LED3C,
	DEV_LIGHT,
	DEV_HUMITURE,
	DEV_DUST
};

//联合体
typedef union
{
  u8 u8[4];
  u32 u32;
  float f;
}dat_t;

//结构体
typedef struct {
  u8 type;//b7,R0W1;b6,d0f1;b5~0,attr
  dat_t dat;
}cmd_t;

typedef union {
  cmd_t cmd[WPAN_CMD_N];
  u8 u8[PKT_SIZE-WPAN_HEAD_LEN];
}pl_t;

typedef struct {
  u8 header;
  u8 reserve;
  pl_t pl;
}pkt_t;

typedef u8 (* attr_func)(u8 rw,dat_t* dat);

//共享变量
extern pkt_t wpan_pkt;

//函数

static u8 deal_pkt(pkt_t * pkt);
 u8 send_pkt(pkt_t * pkt);
 u8 rec_pkt(pkt_t * pkt);

void wpan_init(NRF24L01_CH_TypeDef *init_ch);
u8 deal_wpan(void);
u8 wpan_readAttr(u8* addr, u8 attr, u32* val);
u8 wpan_writeAttr(u8* addr, u8 attr, u32* val);


#endif