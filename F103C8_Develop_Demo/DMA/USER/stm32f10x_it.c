/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    04/06/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "DMA.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
}






bool a = 1;

void DMA1_Tansmit_Config(int *SendBuff,uint32_t CurSize){
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
  DMA_InitTypeDef DMA_InitStruct;
	 //���÷������ݵ������ַ
	 DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(USART1_BASE+0x04);
	 //���ý������ݵ������ַ
	 DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)SendBuff; ////////
	 //�������ݴ��䷽�� ���䵽����
	 DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	 
	//����Դ
	//���ô�����Ŀ
	 DMA_InitStruct.DMA_BufferSize = CurSize;  //����Ĳ��������ݵĳ���
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
	 DMA_Init(DMA1_Channel4,&DMA_InitStruct);
	 
   
	 //ʹ��dma
	 DMA_Cmd(DMA1_Channel4,ENABLE);
	
}

// �����ж���Ϊ�������ݵ�����
void USART1_IRQHandler(void){
   if(USART_GetFlagStatus(USART1,USART_FLAG_IDLE)==SET){
		 uint32_t CurSize = 256 - DMA_GetCurrDataCounter(DMA1_Channel5);               //bufSize - ʣ�������
		 //////
		 
	
		 //��������buffsize
		 DMA_Cmd(DMA1_Channel5,DISABLE);
		 if(a){
			 
			 //��������
			 DMA1_Tansmit_Config(Rx1Buf, CurSize);
			 USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
			 
			 
			 
			 //��������������DMA,�����л�������
		   DMA_InitTypeDef  DMA_Str;
			 DMA_Str.DMA_BufferSize = 256;
	     DMA_Str.DMA_DIR=DMA_DIR_PeripheralSRC;
	     DMA_Str.DMA_M2M =DMA_M2M_Disable;
	     DMA_Str.DMA_MemoryBaseAddr = (u32)Rx2Buf;  
	     DMA_Str.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	     DMA_Str.DMA_MemoryInc = DMA_MemoryInc_Enable;
	     DMA_Str.DMA_Mode = DMA_Mode_Normal;
	     DMA_Str.DMA_PeripheralBaseAddr = (u32)&USART1->DR;
	     DMA_Str.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	     DMA_Str.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	     DMA_Str.DMA_Priority = DMA_Priority_High;
	     DMA_Init(DMA1_Channel5,&DMA_Str);
			 a = 0;
		 }else{
			 //��������
			 DMA1_Tansmit_Config(Rx2Buf, CurSize);
			 USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
			 
			 
			 //��������������DMA,�����л�������
		   DMA_InitTypeDef  DMA_Str;
			 DMA_Str.DMA_BufferSize = 256;
	     DMA_Str.DMA_DIR=DMA_DIR_PeripheralSRC;
	     DMA_Str.DMA_M2M =DMA_M2M_Disable;
	     DMA_Str.DMA_MemoryBaseAddr = (u32)Rx1Buf;  
	     DMA_Str.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	     DMA_Str.DMA_MemoryInc = DMA_MemoryInc_Enable;
	     DMA_Str.DMA_Mode = DMA_Mode_Normal;
	     DMA_Str.DMA_PeripheralBaseAddr = (u32)&USART1->DR;
	     DMA_Str.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	     DMA_Str.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	     DMA_Str.DMA_Priority = DMA_Priority_High;
	     DMA_Init(DMA1_Channel5,&DMA_Str);
			 a = 1;
		 }
		 DMA_Cmd(DMA1_Channel5,ENABLE);
	   USART_ClearFlag(USART1,USART_FLAG_IDLE);
	 }
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
