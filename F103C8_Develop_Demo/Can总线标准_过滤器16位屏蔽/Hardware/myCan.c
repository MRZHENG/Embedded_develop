#include "myCan.h"

// ����Can ����  ���˲�������

void myCan_Init(void){
   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	// CAN GPIO �ڳ�ʼ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// CAN ��ʼ��
	CAN_InitTypeDef CAN_InitStructure;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; 
	
	CAN_InitStructure.CAN_Prescaler = 48;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;
	
	CAN_InitStructure.CAN_NART = DISABLE;  // �ر��Զ��ش�ģʽ
	CAN_InitStructure.CAN_RFLM = ENABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;   // auto bus �����Զ��ָ�
	CAN_InitStructure.CAN_AWUM = ENABLE;   // ������߻ �Զ�����
	CAN_InitStructure.CAN_TTCM = DISABLE;  
	CAN_InitStructure.CAN_TXFP = DISABLE;  //���ȼ�������˳�����
	
	CAN_Init(CAN1,&CAN_InitStructure);
	
	//��ʼ���˲���  // ��Ϊ���ĸ�16λ������ģʽ
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	CAN_FilterInitStruct.CAN_FilterNumber = 0 ;
	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x123<<5;
	CAN_FilterInitStruct.CAN_FilterIdLow = 0x234<<5;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = (0x1FF<<5 )| (0x3<<3);
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = (0x1FF<<5) | (0x3<<3);
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
  
	CAN_FilterInit(&CAN_FilterInitStruct);
}



// ��дCan �� ���ܺ���

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


// ��дCan �� ���ͺ���

void myCanReceive(CanRxMsg*RxMessage){

   CAN_Receive(CAN1,CAN_FIFO0,RxMessage);
	
	 // �ж��Ǳ�׼ID ������չID
	 if(RxMessage->IDE == CAN_Id_Standard){
	    OLED_ShowString(1, 6, "Std");
		  OLED_ShowHexNum(2, 6, RxMessage->StdId, 8);
	 }else{
		  OLED_ShowString(1, 6, "Ext");
	    OLED_ShowHexNum(2, 6, RxMessage->ExtId, 8);
	 }
	 // �ж�������֡����ң��֡ 
	 if(RxMessage->RTR == CAN_RTR_Data){
		  OLED_ShowString(1, 10, "Data  ");
		  OLED_ShowHexNum(3, 6, RxMessage->DLC, 1);
		  
	    OLED_ShowHexNum(3,4,RxMessage->DLC,2);
		  OLED_ShowHexNum(4, 6, RxMessage->Data[0], 2);
		  OLED_ShowHexNum(4, 9, RxMessage->Data[1], 2);
			OLED_ShowHexNum(4, 12,RxMessage->Data[2], 2);
			OLED_ShowHexNum(4, 15,RxMessage->Data[3], 2);
		 
		 
	 }else if (RxMessage->RTR == CAN_RTR_Remote){
		  OLED_ShowString(1, 10, "Remote");
	    OLED_ShowHexNum(3,4,RxMessage->DLC,2);
		 
		  OLED_ShowHexNum(4, 6, RxMessage->Data[0], 2);
		  OLED_ShowHexNum(4, 9, RxMessage->Data[1], 2);
			OLED_ShowHexNum(4, 12,RxMessage->Data[2], 2);
			OLED_ShowHexNum(4, 15,RxMessage->Data[3], 2);
		 
	 }
}


