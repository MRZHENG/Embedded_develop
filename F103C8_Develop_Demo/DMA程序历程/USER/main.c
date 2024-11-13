#include "stm32f10x.h"
#include "led.h"
#include "bsp_key.h"
#include "usart1.h"
#include "bsp_DMA_mtm.h"
#include "bsp_DMA_mtp.h"

//test_mtm
extern uint32_t aDST_Buffer[BUFFER_SIZE];
extern const uint32_t  aSRC_Const_Buffer[BUFFER_SIZE];

//test_mtp
extern uint8_t SendBuff[SENDBUFF_SIZE];

void test_mtm(){
  int result;
	SystemInit();	// 配置系统时钟为72M 	
  LED_GPIO_Config(); //LED 端口初始化   
	MtM_DMA_Config(); //DMA初始化
	//检测传输是否成功
	while(DMA_GetFlagStatus(DMA1_FLAG_TC6)==RESET);
  result = BufferCmp(aSRC_Const_Buffer,aDST_Buffer,32);
	if(result){
	  LED1(ON);
	}else{
	  LED1(OFF);
	}
}

void display(int num){
  while(--num);
}
int main(){
	
	LED_GPIO_Config();
	USART1_Config();
	MtP_DMA_Config();
	
	//初始化发送数组,填充数据
	for(uint16_t i =0;i<SENDBUFF_SIZE;++i){
	  SendBuff[i] = 'P';
	}
	
	//串口请求cpu
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	
	while(1)
  {
	  
	}	
}

