#include "usart1.h"
#include <stdarg.h>

static void NVIC_Config(void){
  NVIC_InitTypeDef   NVIC_Init_Struct;
	NVIC_Init_Struct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Init_Struct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Init_Struct);
}

void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	
	// ����USART1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	// ����USART1 �Ķ˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//���ù���ģʽ
	USART_InitStructure.USART_BaudRate   = 115200; //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ����8
	USART_InitStructure.USART_StopBits   = USART_StopBits_1; //ֹͣλ1
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);  //��ʼ��USART1
	

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);   // ʹ��USART1
	
}

 // ��8λ����
 void UART1SendByte(unsigned char Data)
{	      
        USART_SendData(USART1,Data);
	      // ���TxλһֱΪ0���͵ȴ�����
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	    
}


//����16λ����
void Usart_sendHalfWord(USART_TypeDef* pUSARTx , uint16_t data){
   
	uint8_t temp_h,temp_l;
	temp_h = (data & 0xff00)>>8;
	temp_l = (data & 0xff);
	USART_SendData(pUSARTx,temp_h);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
	
	USART_SendData(pUSARTx,temp_l);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
	  
}

// ����8λ����
unsigned char UART1GetByte(unsigned char* GetData)
{   	   
	      //���
        if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {  return 0;//û���յ����� 
		     }
        *GetData = USART_ReceiveData(USART1); 
        return 1;//�յ�����
}

//������������
void UART1SendArray(USART_TypeDef* USARTx,char *array,uint8_t num)
{   	   
	 
	 for(int i = 0;i<num;i++){
	   UART1SendByte(array[i]);
	 }
	 while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);
}

//���¶���c�⺯��prinf������
int fputc(int ch,FILE*f){
  USART_SendData(USART1,(uint8_t)ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return ch;
};

//���¶���scanf����������
int fgetc(FILE *f){
   while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
	 return (int)USART_ReceiveData(USART1);
};


//�˿ڲ���1
//����һ�����ݣ����Ϸ��ؽ��յ����������
void UART1Test(void)
{
       unsigned char i = 0;

       while(1)
       {    
		       while(UART1GetByte(&i))
          {
            USART_SendData(USART1,i);
          }      
       }     
}

void USART1_Test2(){
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
