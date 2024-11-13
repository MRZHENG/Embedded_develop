#include "usart1.h"


int fputc(int ch, FILE *f) {
    USART_SendData(USART1, (uint8_t)ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return ch;
}



int fgetc(FILE *f) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(USART1);
}

void GPIO_Config(){
	
	//开启GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
  GPIO_InitTypeDef  GPIO_Initstruct;
	
	//PA9
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Initstruct.GPIO_Mode= GPIO_Mode_AF_PP; 
  GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstruct);
	
	//PA10
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Initstruct.GPIO_Mode= GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_Initstruct);

}

void USART_Congig(){
  GPIO_Config();
	//开启USART1的时钟
	
	USART_InitTypeDef USART1_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;     //中断配置
	
	USART1_InitStruct.USART_BaudRate = 115200;
	USART1_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART1_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART1_InitStruct.USART_Parity = USART_Parity_No;
  USART1_InitStruct.USART_StopBits = USART_StopBits_1;
	USART1_InitStruct.USART_WordLength =USART_WordLength_8b;
	USART_Init(USART1,&USART1_InitStruct);
	
	//配置优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//开启串口空闲中断
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	//使能
	USART_Cmd(USART1,ENABLE);
}