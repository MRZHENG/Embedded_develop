#include "stm32f10x.h"
#include "led.h"
#include "bsp_key.h"
#include "usart1.h"
#include "delay.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_wwdg.h"
int main(){
     
	SystemInit();	// 配置系统时钟为72M 	
  LED_GPIO_Config(); //LED 端口初始化   	
  Key_GPIO_Config();//按键端口初始化 
	USART1_Config();  //串口初始化
	
  printf("窗口看门狗测试\n");
	
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)==SET){
	  
		printf("WWDGRST复位，喂狗失败 \n");
		delay_ms(500);
		RCC_ClearFlag();
	}else{
		
	  printf("RST \n");
		delay_ms(500);
	  RCC_ClearFlag();
	}
  
	//开启窗口看门狗时钟APB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	//设置预分频和窗口值 ,超时时间50ms，窗口时间30ms
	WWDG_SetPrescaler(WWDG_Prescaler_8);
  WWDG_SetWindowValue(0x40 |21);
	
	//使能,和重装
	WWDG_Enable(0x40 | 54); //T6 位要置1 ！！！
	
	
	while(1){
		
		Key_Scan(GPIOA,GPIO_Pin_0);  //一直按按键，超时则不能喂狗
		
	 
		
		delay_ms(20);
		
		printf("Feed \n");
		delay_ms(20); 
		//窗口看门狗的喂狗
		WWDG_SetCounter(0x40 | 54);
	}
	
	return 0;
}








