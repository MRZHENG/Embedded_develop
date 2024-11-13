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
	CAN_InitStructure.CAN_TXFP = DISABLE;
	
	CAN_Init(CAN1,&CAN_InitStructure);
	
	//��ʼ���˲���
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterNumber = 0 ;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStruct.CAN_FilterIdLow =0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
  
	CAN_FilterInit(&CAN_FilterInitStruct);
}



// ��дCan �� ���ܺ���

int myCanTransmit(uint32_t ID, uint8_t Length, uint8_t *Data){
	CanTxMsg TxMessage;
 	TxMessage.StdId = ID;
	TxMessage.ExtId = ID;
	TxMessage.IDE = CAN_Id_Standard; // �Ƿ���ñ�׼����֡
	TxMessage.DLC = Length; // ���ݳ���
	TxMessage.RTR = CAN_RTR_Data;  //���������֡

  for(uint8_t i = 0;i<Length;++i){
	   TxMessage.Data[i] = Data[i];
	}
	
  uint8_t mailbox = CAN_Transmit(CAN1,&TxMessage);
	
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

void myCanReceive(uint32_t *ID, uint8_t *Length, uint8_t *Data){
	 CanRxMsg  RxMessage;

   CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
	
	 // �ж��Ǳ�׼ID ������չID
	 if(RxMessage.IDE == CAN_Id_Standard){
	   *ID = RxMessage.StdId;
	 }else{
	   *ID = RxMessage.ExtId;
	 }
	 // �ж�������֡����ң��֡
	 if(RxMessage.RTR == CAN_RTR_Data){
	   *Length = RxMessage.DLC;
		 for(uint8_t i = 0;i<*Length; ++i){
		    Data[i] = RxMessage.Data[i];
		 }
	 }else{
	 
	 }
}


