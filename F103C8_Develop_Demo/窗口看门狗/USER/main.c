#include "stm32f10x.h"
#include "led.h"
#include "bsp_key.h"
#include "usart1.h"
#include "delay.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_wwdg.h"
int main(){
     
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
  LED_GPIO_Config(); //LED �˿ڳ�ʼ��   	
  Key_GPIO_Config();//�����˿ڳ�ʼ�� 
	USART1_Config();  //���ڳ�ʼ��
	
  printf("���ڿ��Ź�����\n");
	
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)==SET){
	  
		printf("WWDGRST��λ��ι��ʧ�� \n");
		delay_ms(500);
		RCC_ClearFlag();
	}else{
		
	  printf("RST \n");
		delay_ms(500);
	  RCC_ClearFlag();
	}
  
	//�������ڿ��Ź�ʱ��APB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	//����Ԥ��Ƶ�ʹ���ֵ ,��ʱʱ��50ms������ʱ��30ms
	WWDG_SetPrescaler(WWDG_Prescaler_8);
  WWDG_SetWindowValue(0x40 |21);
	
	//ʹ��,����װ
	WWDG_Enable(0x40 | 54); //T6 λҪ��1 ������
	
	
	while(1){
		
		Key_Scan(GPIOA,GPIO_Pin_0);  //һֱ����������ʱ����ι��
		
	 
		
		delay_ms(20);
		
		printf("Feed \n");
		delay_ms(20); 
		//���ڿ��Ź���ι��
		WWDG_SetCounter(0x40 | 54);
	}
	
	return 0;
}








