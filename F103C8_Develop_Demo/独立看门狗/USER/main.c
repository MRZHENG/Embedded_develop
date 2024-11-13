#include "stm32f10x.h"
#include "led.h"
#include "bsp_key.h"
#include "usart1.h"
#include "delay.h"
#include "stm32f10x_iwdg.h"
int main(){
     
	SystemInit();	// 配置系统时钟为72M 	
  LED_GPIO_Config(); //LED 端口初始化   	
  Key_GPIO_Config();//按键端口初始化 
	USART1_Config();  //串口初始化
	
  printf("看门狗测试\n");
	
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET){
	  
		printf("IWDGRST复位，喂狗失败 \n");
		delay_ms(500);
		RCC_ClearFlag();
	}else{
		
	  printf("RST \n");
		delay_ms(500);
	  RCC_ClearFlag();
	}
  
	//解除写保护
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//配置预分频和重装值
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(2499);  // 2499 = 1000/0.025/16 - 1 (0 - 4096)
	
	IWDG_ReloadCounter();
	IWDG_Enable();
	// 基于超时时间1000ms  T = T LSI * PR * （RL+1）
	while(1){
		
		Key_Scan(GPIOA,GPIO_Pin_0);  //一直按按键，超时则不能喂狗
		
	  IWDG_ReloadCounter();
		
		delay_ms(600);
		
		printf("Feed \n");
		delay_ms(200);
	}
	
	return 0;
}








