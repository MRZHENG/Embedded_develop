#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "myCan.h"
#include "Key.h"
#include "Timer.h"

int TimerFlag = 0;
int KeyFlag = 0;
int ReqFlag = 0;
/*
 数据策略传输部分
 1. 每隔100ms 自动发送
 2. 当其他设备请求时，发送
 3. 满足一定条件自动发送
*/


CanRxMsg RxMessage;


CanTxMsg TimTxMessage = 
    // StdId   ExtId       IDE            RTR           DLC    DATA[8]
    { 0x100,  0x00000000, CAN_Id_Standard, CAN_RTR_Data  , 4 ,  {0x11,0x22,0x33,0x44} };

CanTxMsg KeyTxMessage = {
  // StdId   ExtId       IDE            RTR           DLC    DATA[8]
     0x200,  0x00000000, CAN_Id_Standard, CAN_RTR_Data  , 4 ,  {0x22,0x33,0x44,0x55}};

CanTxMsg ReqTxMessage = {
  // StdId   ExtId       IDE            RTR           DLC    DATA[8]
     0x300,  0x00000000, CAN_Id_Standard, CAN_RTR_Data  , 4 ,  {0x55,0x66,0x77,0x88}};
		 
		 
int main(void)
{
	OLED_Init();
	Key_Init();
	myCan_Init();
	Timer_Init();
	OLED_ShowString(1, 1, "Tx");
	OLED_ShowString(2, 1, "Tim:");
	OLED_ShowString(3, 1, "Tri:");
	OLED_ShowString(4, 1, "Req:");
	
	while (1)
	{  
		if(TimerFlag==1){
     TimTxMessage.Data[0]++;
		 TimTxMessage.Data[1]++;
		 TimTxMessage.Data[2]++;
		 TimTxMessage.Data[3]++;
		 int num = 	myCanTransmit(&TimTxMessage);
		
		
		 if(num == -1){
		  OLED_ShowString(2,1,"Send Error!!!");
		 }
		 
		 else{	
			OLED_ShowHexNum(2, 6, TimTxMessage.Data[0], 2);
		  OLED_ShowHexNum(2, 9, TimTxMessage.Data[1], 2);
			OLED_ShowHexNum(2, 12,TimTxMessage.Data[2], 2);
			OLED_ShowHexNum(2, 15,TimTxMessage.Data[3], 2);
		 }
		 TimerFlag = 0;
	  };
		
		if(Key_GetNum() == 1){
		    KeyFlag = 1;
		};
		
		if(KeyFlag==1){
     KeyTxMessage.Data[0]++;
		 KeyTxMessage.Data[1]++;
		 KeyTxMessage.Data[2]++;
		 KeyTxMessage.Data[3]++;
		 int num = 	myCanTransmit(&KeyTxMessage);
		
		
		 if(num == -1){
		  OLED_ShowString(2,1,"Send Error!!!");
		 }
		 
		 else{	
			OLED_ShowHexNum(3, 6, KeyTxMessage.Data[0], 2);
		  OLED_ShowHexNum(3, 9, KeyTxMessage.Data[1], 2);
			OLED_ShowHexNum(3, 12,KeyTxMessage.Data[2], 2);
			OLED_ShowHexNum(3, 15,KeyTxMessage.Data[3], 2);
		 }
		 KeyFlag = 0;
	 }
	
		if(myCan_ReceiveFlag()==1){
		   myCanReceive(&RxMessage);
			 if (RxMessage.IDE == CAN_Id_Standard &&
				RxMessage.RTR == CAN_RTR_Remote &&
				RxMessage.StdId == 0x300)
			{
				ReqFlag = 1;
			}
			
			if (RxMessage.IDE == CAN_Id_Standard &&
				RxMessage.RTR == CAN_RTR_Data &&
				RxMessage.StdId == 0x3FF)
			{
				ReqFlag = 1;
			}
		}
	  
		if(ReqFlag==1){
   
		 int num = 	myCanTransmit(&ReqTxMessage);
		
		 if(num == -1){
		  OLED_ShowString(2,1,"Send Error!!!");
		 }
		 
		 else{	
			OLED_ShowHexNum(4, 6, ReqTxMessage.Data[0], 2);
		  OLED_ShowHexNum(4, 9, ReqTxMessage.Data[1], 2);
			OLED_ShowHexNum(4, 12,ReqTxMessage.Data[2], 2);
			OLED_ShowHexNum(4, 15,ReqTxMessage.Data[3], 2);
		 }
		 ReqFlag = 0;
	 }
	
		
		 
  }

}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TimerFlag =1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

