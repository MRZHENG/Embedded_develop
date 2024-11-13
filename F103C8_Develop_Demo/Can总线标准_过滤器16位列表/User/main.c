#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "myCan.h"
#include "Key.h"

#define DATA_Size 6 
uint8_t KeyNum;

CanRxMsg RxMessage;


CanTxMsg TxArray[] = {
    // StdId   ExtId       IDE            RTR           DLC    DATA[8]
   { 0x123,  0x00000000, CAN_Id_Standard, CAN_RTR_Data  , 4 ,  {0x11,0x22,0x33,0x44}}, //标准数据帧
   { 0x234,  0x00000000, CAN_Id_Standard, CAN_RTR_Data  , 4 ,  {0x22,0x33,0x44,0x55}}, //标准数据帧
   { 0x345,  0x00000000, CAN_Id_Standard, CAN_RTR_Data  , 4 ,  {0x33,0x44,0x55,0x66}}, //标准数据帧
   { 0x456,  0x00000000, CAN_Id_Standard, CAN_RTR_Data  , 4 ,  {0x44,0x55,0x66,0x77}}, //标准数据帧
   { 0x567,  0x00000000, CAN_Id_Standard, CAN_RTR_Data  , 4 ,  {0x55,0x66,0x77,0x88}}, //标准数据帧
   { 0x678,  0x00000000, CAN_Id_Standard, CAN_RTR_Data  , 4 ,  {0x66,0x77,0x88,0x99}} //标准数据帧

};


int main(void)
{
	OLED_Init();
	Key_Init();
	myCan_Init();
	
	OLED_ShowString(1, 1, "Rx:");
	OLED_ShowString(2, 1, "RxID:");
	OLED_ShowString(3, 1, "Leng:");
	OLED_ShowString(4, 1, "Data:");
	uint8_t index = 0;
	while (1)
	{
		KeyNum = Key_GetNum();
	
	
		if (KeyNum == 1)
		{
			
		 int num = 	myCanTransmit(&TxArray[index]);
		
			index = (index+1)% DATA_Size;
		 if(num == -1){
		  OLED_ShowString(2,1,"Send Error!!!");
		 }
		}
		
	  // 检测报文队列中是否存在 ID
		if (myCan_ReceiveFlag())
		{
			myCanReceive(&RxMessage);
		}
	}
}
