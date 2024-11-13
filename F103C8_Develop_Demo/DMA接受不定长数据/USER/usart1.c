#include "usart1.h"




void Initial_UART1(unsigned long baudrate)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);							//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);							//ʹ��USART1ʱ��
		
	//USART1_TX   GPIOA.9��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;										//PA.9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;								//GPIO����50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);											//��ʼ��GPIOA.9
	
	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;										//PA.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);											//��ʼ��GPIOA.10
	 
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = baudrate;									//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;							//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); 										//��ʼ������1
	
	//�жϿ�������
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);									//������⴮�ڿ���״̬�ж�
	USART_ClearFlag(USART1,USART_FLAG_TC);											//���USART1��־λ
	
	USART_Cmd(USART1, ENABLE);														//ʹ�ܴ���1
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								//NVICͨ������
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;						//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;								//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);													//����ָ���Ĳ�����ʼ��NVIC�Ĵ���
	
	DMA1_USART1_Init();																//DMA1_USART1��ʼ��
}-



