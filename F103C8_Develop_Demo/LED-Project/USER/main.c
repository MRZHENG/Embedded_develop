#include"stm32f10x.h"
#include"led.h"

void Delay(__IO u32 nCount);
int main(){
     
	SystemInit();
	LED_GPIO_Config();
	while(1){
	  LED1 (ON);
		Delay(0x100000);
		LED1 (OFF);
		Delay(0x100000);
	
	}
	return 0;
}

void Delay(__IO u32 nCount){
   while(nCount){
	   --nCount;
	 }
};

