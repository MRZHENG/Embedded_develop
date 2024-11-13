#ifndef MYCAN__H
#define MYCAN__H

#include "stm32f10x.h"
#include "OLED.h"

extern int myCAN_ReceiveFlag;
extern CanRxMsg RxMessage;


void myCan_Init(void);


// ��дCan �� ���ܺ���

int myCanTransmit(CanTxMsg *TxMessage);


uint8_t myCan_ReceiveFlag(void);

// ��дCan �� ���ͺ���

void myCanReceive(void);


void USB_LP_CAN1_RX0_IRQHandler(void);

#endif
