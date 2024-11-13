#include "stm32f10x.h"
#include "led.h"
#include "bsp_key.h"
#include "usart1.h"


int main(){
     
	SystemInit();	// 配置系统时钟为72M 	
  LED_GPIO_Config(); //LED 端口初始化   	
  Key_GPIO_Config();//按键端口初始化 
	USART1_Config();  //串口初始化
	
	uint8_t cmd ;
	printf("串口控制LED测试\n");
  printf("1.ON\n");
	printf("2.OFF\n");
	while(1){
	 cmd = getchar();
   printf("cmd = %c\n",cmd);
   switch(cmd){
	   case '1': LED1(ON);
		  break;
		 default : LED1(OFF);
		  break;
	 }		
	}
}







