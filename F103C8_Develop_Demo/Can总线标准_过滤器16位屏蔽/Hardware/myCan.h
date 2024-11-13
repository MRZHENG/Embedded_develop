#ifndef MYCAN__H
#define MYCAN__H

#include "stm32f10x.h"
#include "OLED.h"
void myCan_Init(void);

// 书写Can 的 接受函数

int myCanTransmit(CanTxMsg *TxMessage);

uint8_t myCan_ReceiveFlag(void);


// 书写Can 的 发送函数

void myCanReceive(CanRxMsg* RxMessage);


#endif
