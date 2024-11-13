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
	
	// 开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	// 配置USART1 的端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置工作模式
	USART_InitStructure.USART_BaudRate   = 115200; //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位数：8
	USART_InitStructure.USART_StopBits   = USART_StopBits_1; //停止位1
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);  //初始化USART1
	

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);   // 使能USART1
	
}

 // 发8位数据
 void UART1SendByte(unsigned char Data)
{	      
        USART_SendData(USART1,Data);
	      // 如果Tx位一直为0，就等待数据
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	    
}


//发送16位数据
void Usart_sendHalfWord(USART_TypeDef* pUSARTx , uint16_t data){
   
	uint8_t temp_h,temp_l;
	temp_h = (data & 0xff00)>>8;
	temp_l = (data & 0xff);
	USART_SendData(pUSARTx,temp_h);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
	
	USART_SendData(pUSARTx,temp_l);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
	  
}

// 接受8位数据
unsigned char UART1GetByte(unsigned char* GetData)
{   	   
	      //检测
        if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {  return 0;//没有收到数据 
		     }
        *GetData = USART_ReceiveData(USART1); 
        return 1;//收到数据
}

//发送数组数据
void UART1SendArray(USART_TypeDef* USARTx,char *array,uint8_t num)
{   	   
	 
	 for(int i = 0;i<num;i++){
	   UART1SendByte(array[i]);
	 }
	 while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);
}

//重新定义c库函数prinf到串口
int fputc(int ch,FILE*f){
  USART_SendData(USART1,(uint8_t)ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return ch;
};

//重新定义scanf函数到串口
int fgetc(FILE *f){
   while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
	 return (int)USART_ReceiveData(USART1);
};


//端口测试1
//接收一个数据，马上返回接收到的这个数据
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
	printf("串口控制LED测试\n");
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
