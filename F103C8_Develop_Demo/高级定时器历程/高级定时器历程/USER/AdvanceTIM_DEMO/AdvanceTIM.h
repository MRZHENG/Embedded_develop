#ifndef __ADVANCETIM__H
#define __ADVANCETIM__H

#include "stm32f10x.h"

void PWM_LED_Init(void);
void PWM_SetCompare(uint16_t Compare);
void Advance_TIM_GPIO_Config(void);
void Advance_TIM_Mode_Config(void);
void Advance_TIM_Init(void);


#endif

