#ifndef MYCAN__H
#define MYCAN__H

#include "stm32f10x.h"
#include "OLED.h"
void myCan_Init(void);

// ��дCan �� ���ܺ���

int myCanTransmit(CanTxMsg *TxMessage);

uint8_t myCan_ReceiveFlag(void);


// ��дCan �� ���ͺ���

void myCanReceive(CanRxMsg* RxMessage);


#endif
