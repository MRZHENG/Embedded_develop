#include "bsp_DMA_mtp.h"

uint8_t SendBuff[SENDBUFF_SIZE];
void MtP_DMA_Config(){
	//����DMA1��ʱ��
   RCC_AHBPeriphClockCmd(USART_TX_DMA_CLK,ENABLE);
	 
	 DMA_InitTypeDef DMA_InitStruct;
	 //���÷������ݵ������ַ
	 DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)USART_DR_ADR ;
	 //���ý������ݵ������ַ
	 DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)SendBuff;
	 //�������ݴ��䷽�� ���䵽����
	 DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	 
	//����Դ
	//���ô�����Ŀ
	 DMA_InitStruct.DMA_BufferSize = SENDBUFF_SIZE;
	 //����ģʽ ��Ϊ����ֻ��һ���Ĵ���������Ҫ����
	 DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 //�������ݿ��
	 DMA_InitStruct.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte;
	 
	 //����memory
	 //��Ϊ���������ݣ�m������ģʽ��
	 DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 //����ֻ���ܰ�λ����
	 DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	 
	 DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; //�ر�ѭ��ģʽ
	 DMA_InitStruct.DMA_Priority = DMA_Priority_High; //�������ȼ�
	 DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	 
	 //���ö˿�
	 DMA_Init(USART_TX_DMA_CHANNEL,&DMA_InitStruct);
	 
   
	 //ʹ��dma
	 DMA_Cmd(USART_TX_DMA_CHANNEL,ENABLE);
}

