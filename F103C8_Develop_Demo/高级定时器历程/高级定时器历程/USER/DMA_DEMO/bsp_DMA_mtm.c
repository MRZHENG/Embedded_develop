#include "bsp_DMA_mtm.h"

//�������ݵĴ�С


//����������Ϊ����Դ���洢���ڲ���flash��

const uint32_t  aSRC_Const_Buffer[BUFFER_SIZE] ={
                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                   11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                   21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

//����DMA����Ŀ��洢��
//�洢���ڲ���SRAM��
									 
uint32_t aDST_Buffer[BUFFER_SIZE];

void MtM_DMA_Config(){
	//����DMA1��ʱ��
   RCC_AHBPeriphClockCmd(bsp_DMA_CLK,ENABLE);
	 
	 DMA_InitTypeDef DMA_InitStruct;
	 //���÷������ݵ������ַ
	 DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;
	 //���ý������ݵ������ַ
	 DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)aDST_Buffer;
	 //�������ݴ��䷽��
	 DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	 
	//����Դ
	//���ô�����Ŀ
	 DMA_InitStruct.DMA_BufferSize = BUFFER_SIZE;
	 //����ģʽ
	 DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	 //�������ݿ��
	 DMA_InitStruct.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Word;
	 
	 //����memory
	 DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	 DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; //�ر�ѭ��ģʽ
	 DMA_InitStruct.DMA_Priority = DMA_Priority_High; //�������ȼ�
	 DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;
	 
	 //���ö˿�
	 DMA_Init(MTM_DMA_CHANEL,&DMA_InitStruct);
	 //ʹ��dma
	 DMA_Cmd(MTM_DMA_CHANEL,ENABLE);
}

//�ж������Ƿ���ɹ�
uint8_t BufferCmp(const uint32_t* pBuffer,const uint32_t* pBuffer1,uint16_t size){
  while(size--){
	  if(* pBuffer!=* pBuffer1){
		  return 0;
		}
		pBuffer++;
		pBuffer1++;
	}
	return 1;
}

