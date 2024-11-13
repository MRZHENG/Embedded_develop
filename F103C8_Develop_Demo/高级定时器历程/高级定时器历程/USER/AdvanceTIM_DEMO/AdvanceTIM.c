#include "AdvanceTIM.h"


static void Advance_TIM_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//����TIM1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//����GOIO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//��ʼ��ʱ����output_compare�Ĵ���
static void Advance_TIM_Mode_Config(void){
 TIM_TimeBaseInitTypeDef TIM_Struct;
 TIM_OCInitTypeDef  TIM_OCInitStruct;
	
 TIM_Struct.TIM_Period = 50000;  //�Զ���װ��ARR
 TIM_Struct.TIM_Prescaler = 0;   //Ԥ��Ƶ��
 TIM_Struct.TIM_ClockDivision = 0;
 TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
 TIM_TimeBaseInit(TIM1,&TIM_Struct);
	
	

}

void Advance_TIM_Init(void){


};


void PWM_LED_Init(void){
   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // ʹ��PC�˿�ʱ��  
  
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//ѡ���Ӧ������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      // ������� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PC�˿�
	
	
	TIM_InternalClockConfig(TIM2); // ���ڲ�ʱ��
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //��Ƶ����
  TIM_TimeBaseInitStructure.TIM_Period = 100-1; //����ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720-1 ; //���÷�Ƶ,PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //�ظ�����
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1  ; //���ģʽ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High  ; //����
	TIM_OCInitStructure.TIM_OutputState= TIM_OutputNState_Enable ;  //ʹ��
	TIM_OCInitStructure.TIM_Pulse = 50;            //����CCR
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

//����CCR��ֵʵ�ֺ�����
void PWM_SetCompare(uint16_t Compare){
   TIM_SetCompare1(TIM2,Compare);
}

