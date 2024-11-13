#include "myCan.h"

int myCAN_ReceiveFlag = 0;
CanRxMsg RxMessage;

// 开启Can 外设  和滤波器设置

void myCan_Init(void){
   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	// CAN GPIO 口初始化
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置接受FIFO的中断 (需要在初始化CAN之前配置，避免出现问题)
  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);    // 开启中断 开启什么中断
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置分组

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn ; //开启数据进入FiFo0的中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);   // 开启NVIC
	
	
	
	
	// CAN 初始化
	CAN_InitTypeDef CAN_InitStructure;
	CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack; 
	
	CAN_InitStructure.CAN_Prescaler = 48;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;
	
	CAN_InitStructure.CAN_NART = DISABLE;  // 关闭自动重传模式
	CAN_InitStructure.CAN_RFLM = ENABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;   // auto bus 离线自动恢复
	CAN_InitStructure.CAN_AWUM = ENABLE;   // 检测总线活动 自动唤醒
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	
	CAN_Init(CAN1,&CAN_InitStructure);
    
	
	
	//初始化滤波器
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	CAN_FilterInitStruct.CAN_FilterNumber = 0 ;
	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStruct.CAN_FilterIdLow =0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
  
	CAN_FilterInit(&CAN_FilterInitStruct);
}



// 书写Can 的 接受函数

int myCanTransmit(CanTxMsg *TxMessage){

  uint8_t mailbox = CAN_Transmit(CAN1,TxMessage);
	
	int TimeOut = 0;
	
	while(CAN_TransmitStatus(CAN1,mailbox) != CAN_TxStatus_Ok)
	{
   TimeOut++;
   if(TimeOut == 50000){
	   return -1;
	 }		
	}
  return 0;
}


uint8_t myCan_ReceiveFlag(void){
   if(CAN_MessagePending(CAN1,CAN_FIFO0)>0) return 1;
	 return 0;
}


// 书写Can 的 发送函数

void myCanReceive(void){
   
	 
	 // 判断是标准ID 还是扩展ID
	 if(RxMessage.IDE == CAN_Id_Standard){
	    OLED_ShowString(1, 6, "Std");
		  OLED_ShowHexNum(2, 6, RxMessage.StdId, 8);
	 }else{
		  OLED_ShowString(1, 6, "Ext");
	    OLED_ShowHexNum(2, 6, RxMessage.ExtId, 8);
	 }
	 // 判断是数据帧还是遥控帧 
	 if(RxMessage.RTR == CAN_RTR_Data){
		  OLED_ShowString(1, 10, "Data  ");
		  OLED_ShowHexNum(3, 6, RxMessage.DLC, 1);
		  
	    OLED_ShowHexNum(3,4,RxMessage.DLC,2);
		  OLED_ShowHexNum(4, 6, RxMessage.Data[0], 2);
		  OLED_ShowHexNum(4, 9, RxMessage.Data[1], 2);
			OLED_ShowHexNum(4, 12,RxMessage.Data[2], 2);
			OLED_ShowHexNum(4, 15,RxMessage.Data[3], 2);
		 
		 
	 }else if (RxMessage.RTR == CAN_RTR_Remote){
		  OLED_ShowString(1, 10, "Remote");
	    OLED_ShowHexNum(3,4,RxMessage.DLC,2);
		 
		  OLED_ShowHexNum(4, 6, RxMessage.Data[0], 2);
		  OLED_ShowHexNum(4, 9, RxMessage.Data[1], 2);
			OLED_ShowHexNum(4, 12,RxMessage.Data[2], 2);
			OLED_ShowHexNum(4, 15,RxMessage.Data[3], 2);
		 
	 }
	 
	 
}


void USB_LP_CAN1_RX0_IRQHandler(void){
    if(CAN_GetITStatus(CAN1,CAN_IT_FMP0) == SET){
		   CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
			 myCAN_ReceiveFlag = 1;
		}
}

