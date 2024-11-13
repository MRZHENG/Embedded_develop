#include "AdvanceTIM.h"


static void Advance_TIM_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//开启TIM1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//开启GOIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//初始化时基和output_compare寄存器
static void Advance_TIM_Mode_Config(void){
 TIM_TimeBaseInitTypeDef TIM_Struct;
 TIM_OCInitTypeDef  TIM_OCInitStruct;
	
 TIM_Struct.TIM_Period = 50000;  //自动重装载ARR
 TIM_Struct.TIM_Prescaler = 0;   //预分频器
 TIM_Struct.TIM_ClockDivision = 0;
 TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
 TIM_TimeBaseInit(TIM1,&TIM_Struct);
	
	

}

void Advance_TIM_Init(void){


};


void PWM_LED_Init(void){
   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // 使能PC端口时钟  
  
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//选择对应的引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      // 推挽输出 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PC端口
	
	
	TIM_InternalClockConfig(TIM2); // 打开内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //分频因子
  TIM_TimeBaseInitStructure.TIM_Period = 100-1; //配置ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720-1 ; //配置分频,PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //重复计数
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1  ; //输出模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High  ; //极性
	TIM_OCInitStructure.TIM_OutputState= TIM_OutputNState_Enable ;  //使能
	TIM_OCInitStructure.TIM_Pulse = 50;            //设置CCR
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

//更改CCR的值实现呼吸灯
void PWM_SetCompare(uint16_t Compare){
   TIM_SetCompare1(TIM2,Compare);
}

