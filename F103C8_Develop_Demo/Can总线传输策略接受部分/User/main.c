#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "myCan.h"
#include "Key.h"
uint8_t KeyNum =0;

CanRxMsg RxMsg;


CanTxMsg TxMsg_Request_Remote = {
	.StdId = 0x300,
	.ExtId = 0x00000000,
	.IDE = CAN_Id_Standard,
	.RTR = CAN_RTR_Remote,
	.DLC = 0,
	.Data = {0x00}
};

CanTxMsg TxMsg_Request_Data = {
	.StdId = 0x3FF,
	.ExtId = 0x00000000,
	.IDE = CAN_Id_Standard,
	.RTR = CAN_RTR_Data,
	.DLC = 0,
	.Data = {0x00}
};


int main(void)
{
	OLED_Init();
	Key_Init();
	myCan_Init();
	
	OLED_ShowString(1, 1, "Rx");
	OLED_ShowString(2, 1, "Tim:");
	OLED_ShowString(3, 1, "Tir:");
	OLED_ShowString(4, 1, "Req:");
	
	while (1)
	{
			/*���󲿷�*/
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			myCanTransmit(&TxMsg_Request_Remote);
			
		}
		if (KeyNum == 2)
		{
			myCanTransmit(&TxMsg_Request_Data);
			
		}
		
		
		// ���������ݽ���ͽ���
		if (myCan_ReceiveFlag())
		{
				myCanReceive(&RxMsg);
			
			if (RxMsg.RTR == CAN_RTR_Data)
			{
				/*�յ���ʱ����֡*/
				if (RxMsg.StdId == 0x100 && RxMsg.IDE == CAN_Id_Standard)
				{
					OLED_ShowHexNum(2, 5, RxMsg.Data[0], 2);
					OLED_ShowHexNum(2, 8, RxMsg.Data[1], 2);
					OLED_ShowHexNum(2, 11, RxMsg.Data[2], 2);
					OLED_ShowHexNum(2, 14, RxMsg.Data[3], 2);
				}
				
				/*�յ���������֡*/
				if (RxMsg.StdId == 0x200 && RxMsg.IDE == CAN_Id_Standard)
				{
					OLED_ShowHexNum(3, 5, RxMsg.Data[0], 2);
					OLED_ShowHexNum(3, 8, RxMsg.Data[1], 2);
					OLED_ShowHexNum(3, 11, RxMsg.Data[2], 2);
					OLED_ShowHexNum(3, 14, RxMsg.Data[3], 2);
				}
				
				/*�յ���������֡*/
				if (RxMsg.StdId == 0x300 && RxMsg.IDE == CAN_Id_Standard)
				{
					OLED_ShowHexNum(4, 5, RxMsg.Data[0], 2);
					OLED_ShowHexNum(4, 8, RxMsg.Data[1], 2);
					OLED_ShowHexNum(4, 11, RxMsg.Data[2], 2);
					OLED_ShowHexNum(4, 14, RxMsg.Data[3], 2);
				}
			}
			
		}
	}
}
