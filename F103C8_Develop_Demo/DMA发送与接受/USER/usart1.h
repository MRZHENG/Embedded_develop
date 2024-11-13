#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_usart.h"
#include "led.h"
#include "misc.h"
#include "DMA.h"
/*串口结构体*/
typedef struct
{
    //串口收发缓存
    u8 *uart1_rx;     //串口1接收缓存
    u8 *uart1_tx;     //串口1发送缓存
    vs8 uart1_num;    //串口1记录缓存数
   
}_UART_Stru_;

void USART1_Init(u32 baud);
//重新定义c库函数prinf到串口
int fputc(int ch,FILE*f);
void USART1_IRQHandler(void);
//重新定义scanf函数到串口
int fgetc(FILE *f);



#endif /* __USART1_H */



