#include "stm32f10x.h"
#include "usart1.h"




int main(void) {
    // 初始化系统
    SystemInit();

    // 初始化USART1
    USART1_Init(115200);  // 115200是波特率示例，按照实际需要设置

    // 初始化DMA1用于USART1
    DMA1_USART1_Init(3);


   bool a =1,b=0,c=0,d=0;
    // 等待数据发送完成
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){
		  if(a){
				DMA_Cmd(DMA1_Channel4, ENABLE);  
			  DMA1_RxTx_Start(DMA1_Channel4,Rx1Buf[1],104);
				a=c=d=0;
				b=1;
			}else if(b){
				DMA_Cmd(DMA1_Channel4, ENABLE); 
			  DMA1_RxTx_Start(DMA1_Channel4,Rx1Buf[2],104);
				a=d=b=0;
				c=1;
			}
			else if(c){
				DMA_Cmd(DMA1_Channel4, ENABLE); 
			  DMA1_RxTx_Start(DMA1_Channel4,Rx1Buf[3],104);
				a=b=c=0;
				d=1;
			}
			else{
				DMA_Cmd(DMA1_Channel4, ENABLE); 
				DMA1_RxTx_Start(DMA1_Channel4,Rx1Buf[0],104);
				b=c=d=0;
				a=1;
			}
		};


   
}






