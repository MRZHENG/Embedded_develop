#include "usart1.h"




void Initial_UART1(unsigned long baudrate)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);							//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);							//使能USART1时钟
		
	//USART1_TX   GPIOA.9初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;										//PA.9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//GPIO速率50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);											//初始化GPIOA.9
	
	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;										//PA.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);											//初始化GPIOA.10
	 
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = baudrate;									//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;							//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init(USART1, &USART_InitStructure); 										//初始化串口1
	
	//中断开启设置
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);									//开启检测串口空闲状态中断
	USART_ClearFlag(USART1,USART_FLAG_TC);											//清除USART1标志位
	
	USART_Cmd(USART1, ENABLE);														//使能串口1
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								//NVIC通道设置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;						//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;								//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化NVIC寄存器
	
	DMA1_USART1_Init();																//DMA1_USART1初始化
}-



