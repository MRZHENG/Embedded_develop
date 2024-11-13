#ifndef __BASICTIM__H
#define __BASICTIM__H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"

void BASIC_TIM_Init(void);
void BASIC_TIM_Config(void);

void BASIC_TIM_NVIC_Config(void);

#define      BASIC_TIM                       TIM2
#define      BASIC_TIM_APBxClock_FUN         RCC_APB1PeriphClockCmd
#define      BASIC_TIM_CLK                   RCC_APB1Periph_TIM2
#define      BASIC_TIM_Period                1000
#define      BASIC_TIM_Prescaler             71
#define      BASIC_TIM_IRQ                   TIM2_IRQn
#define      BASIC_TIM_IRQHandler            TIM2_IRQHandler


#endif

