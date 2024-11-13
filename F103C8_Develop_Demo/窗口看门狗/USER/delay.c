#include "delay.h"


void delay_us(uint32_t us){
   SysTick_Config(72);
	 for(uint32_t i = 0 ; i< us;++i){
	    while(!(SysTick ->CTRL) &(1<<16)); 
	 }
	 SysTick->CTRL &= ~SYSTICK_CLKSOURCE;
	 
}


void delay_ms(uint32_t ms){
	uint32_t i;
	SysTick_Config(72000);
	
	for( i = 0;i < ms;i++){
		while( !((SysTick->CTRL) & (1<<16)) );
	}
	
	SysTick->CTRL &= ~SYSTICK_CLKSOURCE;
}