#include "DMA.h"

/*
DMA1 USART1 ��ʼ��
 DMA1ͨ��4��Ӧ������1����
 DMA1ͨ��5��Ӧ������1����        
 RxTx  ����/���� DMA1ͨ����ʼ����1���� 2���� 3�շ� 0������DMA��

*/


u8 Rx1Buf[BUF_NUM_RX1][BUF_SIZE_RX1] = {{0}, {0}, {0}, {0}};  //����1 �Ľ��ջ��� 
u8 Tx1Buf[BUF_SIZE_TX1] = {0};                                //����1 �ķ��ͻ���

void DMA1_USART1_Init(u8 RxTx)
{
    DMA_InitTypeDef  DMA_Str;           //DMA���ýṹ��
    NVIC_InitTypeDef NVIC_InitStruct;   //�ж�����

    if(!RxTx)
        return;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    //���� DMA1 ʱ��

    //DMA ����ͨ������
    if(RxTx == 1 || RxTx == 3)
    {
        USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);   //���� USART1 ���� DMA ����
        DMA_DeInit(DMA1_Channel5);                       //ͨ�� 5 ��Ӧ����1 ����
    }
    //DMA ����ͨ������
    if(RxTx == 2 || RxTx == 3)
    {
        USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);    //���� USART1 ���� DMA ����
        DMA_DeInit(DMA1_Channel4);                        //ͨ�� 4 ��Ӧ����1 ����
    }

    //DMA ����
    DMA_Str.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   //�����ʱ�������ַ�Ƿ��������

    /*DMA ���ڴ洫��һ���ֽڣ�ÿ�ζ�������ݷŵ��ڴ�����ƫ�� 1 ����ַ���ڴ��׷��*/
    DMA_Str.DMA_MemoryInc          = DMA_MemoryInc_Enable;        //�����ʱ���ڴ��ַ�Ƿ���Ҫ�������ǣ�
    DMA_Str.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ��ȣ�1�ֽڣ�8λ��
    DMA_Str.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     //�ڴ����ݳ��ȣ�1�ֽڣ�8λ��
    DMA_Str.DMA_M2M                = DMA_M2M_Disable;             //�Ƿ��ڴ浽�ڴ洫����ʹ�ܣ���
    DMA_Str.DMA_PeripheralBaseAddr = (u32)&USART1->DR;            //ָ��DMA�ŵ����������ַ
    DMA_Str.DMA_Priority           = DMA_Priority_High;           //DMAͨ��������ȼ����ߣ�

    //USART1���գ�DMA1ͨ��5����
    if(RxTx==1 || RxTx==3) 
    {   
        /*DMA_SIZE_RX1 ��������Сһ��Ҫ�������Ǵ��ڵ��ν��յ�����ֽ����������һ��������
          ����û��ϵ�����ǲ���С */
         DMA_Str.DMA_BufferSize     = DMA_SIZE_RX1;           //DMA һ�δ������ݴ�С

        /*���� DMA �Ӵ��ڴ����������ݷŵ� Rx1Buf[0] ��ַ��ʼ���ڴ���
          �� Rx1Buf[0] ��һ����ά���飬���� Rx1Buf[0] ��һ��ָ�룬���Ǳ�����*/
         DMA_Str.DMA_MemoryBaseAddr = (u32)Rx1Buf[0];         //ָ�� DMA �ŵ����ڴ����ַ         

        //���ǰ�������յ������ݴ��䵽�ڴ���
         DMA_Str.DMA_DIR            = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣨴����赽�ڴ棩

        /*ѭ��ģʽ�����������˴������ݴ�С�� 8 �ֽڣ� DMA �ڴ����� 8 �ֽڵ�����֮��
        DMA ���Զ���ʼ��һ�ִ��䣬��������ݻ��Ƿ��ڻ���ַ��ʼ���ڴ��У�
        �Ḳ��֮ǰ���Ƕ��ڴ�
        ��ʵ����ѡ����ͨģʽҲ�ǿ��Եģ���Ϊ����ÿ���յ� 1 �����ݣ��ͻ��л����棬���¿�ʼһ�δ���*/
         DMA_Str.DMA_Mode           = DMA_Mode_Circular;      //ָ��DMAͨ������ģʽ��ѭ��ģʽ��
        DMA_Init(DMA1_Channel5, &DMA_Str);                   //DMA1ͨ��5��ʼ��
        DMA_Cmd(DMA1_Channel5, ENABLE);                      //ʹ�� DMA1 ͨ��5����ʼ����
    }
    //USART1���ͣ�DMA1ͨ��4����
    if(RxTx==2 || RxTx==3) 
    {
        DMA_Str.DMA_BufferSize     = DMA_SIZE_TX1;           //DMA һ�δ������ݴ�С
        DMA_Str.DMA_MemoryBaseAddr = (u32)Tx1Buf;            //ָ�� DMA �ŵ����ڴ����ַ          
        DMA_Str.DMA_DIR            = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣨴��ڴ浽���裩           

         /*���ͣ�����û��ѡ��ѭ��ģʽ��������ͨģʽ����ͨģʽֻ����һ�ξͽ���������һֱѭ�����䣬
           Ŀ�ľ����õ���ʱ��ŷ� */
         DMA_Str.DMA_Mode           = DMA_Mode_Normal;        //ָ��DMAͨ������ģʽ����ͨģʽ��
        DMA_Init(DMA1_Channel4, &DMA_Str);                   //DMA1 ͨ�� 4 ��ʼ��

        /*���������������귢��ͨ����û�п������䣬����Ϊ���Ƿ��͵�����Ҳ�ǲ������ģ�
          ���Ҵ��ڵķ������������������ϵģ�ʲôʱ���ͣ����͵����ݶ೤��������֪�ġ�
          �����������һ����Ҫһֱ����һ�ִ�����״̬���������ǻ�дһ��ͨ�� DMA ���ʹ��ڵĺ����ӿڡ�
          ����Ҫ��ʱ���ͨ�� DMA ���䷢�ʹ���*/
    }
}


void DMA1_RxTx_Start(DMA_Channel_TypeDef* DMAy_Channelx, u8 *RxTx_Buf, u16 len)
{
    /*������֮ǰ����Ҫ�Ȱ� DMA ��ص�ͨ���ر�*/
    DMA_Cmd(DMAy_Channelx, DISABLE);                      //�ر�DMA1ָ��ͨ��

    /*���ǰ�Ҫ���͵����ݷŵ� RxTx_Buf ����ڴ��оͿ�����*/
    DMAy_Channelx -> CMAR = (u32)RxTx_Buf;                //DMA����Ļ���ַ
    DMAy_Channelx -> CNDTR = len;                         //DMA��������ݳ���
    DMA_Cmd(DMAy_Channelx, ENABLE);                       //����DMA1ָ��ͨ��
}


