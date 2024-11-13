#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_usart.h"
#include "led.h"
#include "misc.h"
#include "DMA.h"
/*���ڽṹ��*/
typedef struct
{
    //�����շ�����
    u8 *uart1_rx;     //����1���ջ���
    u8 *uart1_tx;     //����1���ͻ���
    vs8 uart1_num;    //����1��¼������
   
}_UART_Stru_;

void USART1_Init(u32 baud);
//���¶���c�⺯��prinf������
int fputc(int ch,FILE*f);
void USART1_IRQHandler(void);
//���¶���scanf����������
int fgetc(FILE *f);



#endif /* __USART1_H */



