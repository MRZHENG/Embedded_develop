#include "stm32f10x.h"
#include "led.h"
#include "bsp_key.h"
#include "usart1.h"


int main(){
     
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
  LED_GPIO_Config(); //LED �˿ڳ�ʼ��   	
  Key_GPIO_Config();//�����˿ڳ�ʼ�� 
	USART1_Config();  //���ڳ�ʼ��
	
	uint8_t cmd ;
	printf("���ڿ���LED����\n");
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







