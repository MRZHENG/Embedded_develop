#include "stm32f10x.h"
#include <stdio.h>
#include "usart1.h"
#include "DMA.h"

int main(void)
{
	 SystemInit();	
   GPIO_Config();
	 USART_Congig();
   DMA1_Recive_Config();
	 USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);        					
	 USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  
	 while(1){};
}