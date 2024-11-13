#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "myCan.h"
#include "Key.h"
uint8_t KeyNum;

CanRxMsg RxMessage;


CanTxMsg TxArray[] = {
    // StdId   ExtId       IDE            RTR           DLC    DATA[8]
   { 0x555,  0x00000000, CAN_Id_Standard, CAN_RTR_Data  , 4 ,  {0x11,0x22,0x33,0x44}}, //��׼����֡
   { 0x666,  0x00000000, CAN_Id_Standard, CAN_RTR_Remote, 0 ,  {0x00,0x00,0x00,0x00}}, //��׼ң��֡
   { 0x555,  0x12345678, CAN_Id_Extended, CAN_RTR_Data  , 4 ,  {0x55,0x66,0x77,0x88}}, //��չ����֡
   { 0x666,  0x0789ABCD, CAN_Id_Standard, CAN_RTR_Remote, 0 ,  {0x00,0x00,0x00,0x00}} //��չң��֡
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
	int i = 0;
	while (1)
	{
		KeyNum = Key_GetNum();
	
	
		if (KeyNum == 1)
		{
			
		 int num = 	myCanTransmit(&TxArray[i]);
		
			i = (i+1)%4;
		 if(num == -1){
		  OLED_ShowString(2,1,"Send Error!!!");
		 }
		}
		
	  // ��ⱨ�Ķ������Ƿ���� ID
		if (myCan_ReceiveFlag())
		{
			myCanReceive(&RxMessage);
		}
	}
}
