#include "BasicTIM.h"
#include "stm32f10x_tim.h"
#include "misc.h"

void BASIC_TIM_Config(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	  //������ʱ��ʱ�ӣ����ó�72m
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
	  //�Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ��ж�
	  TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;
	  
	  //ʱ��Ԥ��Ƶ��Ϊ
	  TIM_TimeBaseStructure.TIM_Prescaler = BASIC_TIM_Prescaler;
	
	  //��ʼ����ʱ��
	  TIM_TimeBaseInit(BASIC_TIM,&TIM_TimeBaseStructure);
	  
	  //������������жϱ�־λ
	  TIM_ClearFlag(BASIC_TIM,TIM_FLAG_Update);
	  
	  //������ʱ���ж�
	  TIM_ITConfig(BASIC_TIM, TIM_IT_Update,ENABLE);
	  
	  //ʹ�ܼ�����
		TIM_Cmd(TIM2, ENABLE);
	  
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE);
}


//�ж����ȼ�����
void BASIC_TIM_NVIC_Config(void){
  NVIC_InitTypeDef  NVIC_InitStructure;
  //�����ж���Ϊ0
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	//�����ж�Դ
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	//�����ж����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//������ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
	NVIC_InitStructure.NVIC_IRQChannelCmd  =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void BASIC_TIM_Init(void){
  BASIC_TIM_Config();
	BASIC_TIM_NVIC_Config();
}

