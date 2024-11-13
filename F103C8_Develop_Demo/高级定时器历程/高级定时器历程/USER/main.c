#include "stm32f10x.h"
#include "led.h"
#include "usart1.h"
#include "AdvanceTIM.h"
uint16_t time =0;
uint8_t i;
int main(){
	
	 SystemInit();
	 PWM_LED_Init();
	
	 while(1){
	   for(i = 0;i<100;++i){
		    PWM_SetCompare(i);
		 }
		 for(int i = 100;i>=0;--i){
		    PWM_SetCompare(i);
		 }
	 }
	 
}

