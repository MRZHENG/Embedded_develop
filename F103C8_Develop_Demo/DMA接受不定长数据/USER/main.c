#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "DMA.h"
#include "usart1.h"





int main(void)
{
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	Initial_UART1(115200);
 
	while(1){};
}
