#include "stm32f10x.h"
#include "led.h"
#include "bsp_key.h"
#include "usart1.h"
#include "delay.h"
#include "stm32f10x_iwdg.h"
int main(){
     
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
  LED_GPIO_Config(); //LED �˿ڳ�ʼ��   	
  Key_GPIO_Config();//�����˿ڳ�ʼ�� 
	USART1_Config();  //���ڳ�ʼ��
	
  printf("���Ź�����\n");
	
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET){
	  
		printf("IWDGRST��λ��ι��ʧ�� \n");
		delay_ms(500);
		RCC_ClearFlag();
	}else{
		
	  printf("RST \n");
		delay_ms(500);
	  RCC_ClearFlag();
	}
  
	//���д����
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//����Ԥ��Ƶ����װֵ
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(2499);  // 2499 = 1000/0.025/16 - 1 (0 - 4096)
	
	IWDG_ReloadCounter();
	IWDG_Enable();
	// ���ڳ�ʱʱ��1000ms  T = T LSI * PR * ��RL+1��
	while(1){
		
		Key_Scan(GPIOA,GPIO_Pin_0);  //һֱ����������ʱ����ι��
		
	  IWDG_ReloadCounter();
		
		delay_ms(600);
		
		printf("Feed \n");
		delay_ms(200);
	}
	
	return 0;
}








