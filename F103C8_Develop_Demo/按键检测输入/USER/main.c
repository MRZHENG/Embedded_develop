#include"stm32f10x.h"
#include"led.h"
#include"bsp_key.h"



int main(){
     
	SystemInit();	// 配置系统时钟为72M 	
  LED_GPIO_Config(); //LED 端口初始化   	
  Key_GPIO_Config();//按键端口初始化


  while(1)                            
  {	   
 	   if( Key_Scan(GPIOA,GPIO_Pin_0) == KEY_ON  )	 //判断KEY1是否按下
	   {			
     GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))));//LED1翻转
	   } 
  }
	
	return 0;
}



