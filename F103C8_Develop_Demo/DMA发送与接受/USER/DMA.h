#ifndef __DMA__H
#define __DMA__H

#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "misc.h"


/*串口缓存数，用来保存串口1 接收到的数据
串口1 使用 4 个缓存，*/
#define BUF_NUM_RX1 4        //串口1收缓存数

//串口收发缓存大小
#define BUF_SIZE_RX1  104    //串口1收缓存大小
#define BUF_SIZE_TX1  64     //串口1发缓存大小

#define DMA_SIZE_RX1  104
#define DMA_SIZE_TX1  64

void DMA1_USART1_Init(u8 RxTx);
void DMA1_RxTx_Start(DMA_Channel_TypeDef* DMAy_Channelx, u8 *RxTx_Buf, u16 len);

extern u8 Rx1Buf[BUF_NUM_RX1][BUF_SIZE_RX1];  //串口1 的接收缓存 
extern u8 Tx1Buf[BUF_SIZE_TX1] ;                                //串口1 的发送缓存
#endif

