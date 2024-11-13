#include "stm32f10x.h"
#include "led.h"
#include "bsp_key.h"
#include "usart1.h"
#include "bsp_DMA_mtm.h"
#include "bsp_DMA_mtp.h"
#include "BasicTIM.h"
#define START_TIME  time=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)

uint16_t time = 0;
int main(){
	
	 SystemInit();
   LED_GPIO_Config();
	
	 BASIC_TIM_Config();
	 BASIC_TIM_NVIC_Config();
	
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	 TIM_Cmd(TIM2, ENABLE);
	 while(1){
	   if(time == 1000){time = 0;
			 GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)))); /* LED1 È¡·´ */
			 
		 }
	 }
}

