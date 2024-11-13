#ifndef MYCAN__H
#define MYCAN__H

#include "stm32f10x.h"

void myCan_Init(void);

// 书写Can 的 接受函数

int myCanTransmit(uint32_t ID, uint8_t Length, uint8_t *Data);


uint8_t myCan_ReceiveFlag(void);


// 书写Can 的 发送函数

void myCanReceive(uint32_t *ID, uint8_t *Length, uint8_t *Data);


#endif
