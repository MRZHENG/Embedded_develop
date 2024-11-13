#ifndef __USART1_H
#define __USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stm32f10x_gpio.h"

int fputc(int ch, FILE *f);
int fgetc(FILE *f);
void GPIO_Config(void);
void USART_Congig(void);
#endif /* __USART1_H */