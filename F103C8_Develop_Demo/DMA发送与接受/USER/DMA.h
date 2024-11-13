#ifndef __DMA__H
#define __DMA__H

#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "misc.h"


/*���ڻ��������������洮��1 ���յ�������
����1 ʹ�� 4 �����棬*/
#define BUF_NUM_RX1 4        //����1�ջ�����

//�����շ������С
#define BUF_SIZE_RX1  104    //����1�ջ����С
#define BUF_SIZE_TX1  64     //����1�������С

#define DMA_SIZE_RX1  104
#define DMA_SIZE_TX1  64

void DMA1_USART1_Init(u8 RxTx);
void DMA1_RxTx_Start(DMA_Channel_TypeDef* DMAy_Channelx, u8 *RxTx_Buf, u16 len);

extern u8 Rx1Buf[BUF_NUM_RX1][BUF_SIZE_RX1];  //����1 �Ľ��ջ��� 
extern u8 Tx1Buf[BUF_SIZE_TX1] ;                                //����1 �ķ��ͻ���
#endif

