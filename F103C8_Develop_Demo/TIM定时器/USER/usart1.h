#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_usart.h"
#include "led.h"
void USART1_Config(void);
void UART1Test(void);
void UART1SendByte(unsigned char Data);
unsigned char UART1GetByte(unsigned char* GetData);
void UART1Test(void);
void Usart_sendHalfWord(USART_TypeDef* pUSARTx , uint16_t data);
void UART1SendArray(USART_TypeDef* USARTx,char *array,uint8_t num);
void USART1_Test2(void);
	
//重新定义c库函数prinf到串口
int fputc(int ch,FILE*f);

//重新定义scanf函数到串口
int fgetc(FILE *f);
#endif /* __USART1_H */



