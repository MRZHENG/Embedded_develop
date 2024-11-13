#include "BasicTIM.h"
#include "stm32f10x_tim.h"
#include "misc.h"

void BASIC_TIM_Config(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	  //开启定时器时钟，设置成72m
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
	  //自动重装载寄存器的值，累计TIM_Period+1个频率后中断
	  TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;
	  
	  //时钟预分频数为
	  TIM_TimeBaseStructure.TIM_Prescaler = BASIC_TIM_Prescaler;
	
	  //初始化定时器
	  TIM_TimeBaseInit(BASIC_TIM,&TIM_TimeBaseStructure);
	  
	  //清除计数器的中断标志位
	  TIM_ClearFlag(BASIC_TIM,TIM_FLAG_Update);
	  
	  //开启计时器中断
	  TIM_ITConfig(BASIC_TIM, TIM_IT_Update,ENABLE);
	  
	  //使能计数器
		TIM_Cmd(TIM2, ENABLE);
	  
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE);
}


//中断优先级配置
void BASIC_TIM_NVIC_Config(void){
  NVIC_InitTypeDef  NVIC_InitStructure;
  //设置中断组为0
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	//设置中断源
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	//设置中断优先级为0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
	NVIC_InitStructure.NVIC_IRQChannelCmd  =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void BASIC_TIM_Init(void){
  BASIC_TIM_Config();
	BASIC_TIM_NVIC_Config();
}

