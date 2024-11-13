#include "DMA.h"

u8 USART1_TX_BUF[USART1_MAX_TX_LEN]; 	//���ͻ���,���USART1_MAX_TX_LEN�ֽ�
u8 u1rxbuf[USART1_MAX_RX_LEN];			//�������ݻ�����1
u8 u2rxbuf[USART1_MAX_RX_LEN];			//�������ݻ�����2
u8 witchbuf=0;                  		//��ǵ�ǰʹ�õ����ĸ�������,0��ʹ��u1rxbuf��1��ʹ��u2rxbuf
u8 USART1_TX_FLAG=0;					//USART2���ͱ�־����������ʱ��1
u8 USART1_RX_FLAG=0;					//USART2���ձ�־����������ʱ��1


//DMA ����Ӧ��Դ��
void DMA_USART1_Tx_Data(u8 *buffer, u32 size)
{
    while (USART1_TX_FLAG);                     // �ȴ���һ�η�����ɣ�USART1_TX_FLAGΪ1�����ڷ������ݣ�
    USART1_TX_FLAG = 1;                        // USART1���ͱ�־���������ͣ�

    DMA1_Channel4->CMAR = (uint32_t)buffer;    // ����Ҫ���͵����ݵ�ַ (USART1 ͨ��4)
    DMA1_Channel4->CNDTR = size;               // ����Ҫ���͵��ֽ���Ŀ
    DMA_Cmd(DMA1_Channel4, ENABLE);            // ��ʼDMA����
}



void DMA1_USART1_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // ʹ��DMA1ʱ��

    // DMA_USART1_RX  USART1->RAM�����ݴ���
    DMA_DeInit(DMA1_Channel5); // ��DMA��ͨ��5�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR); // �������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)u1rxbuf; // �ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // ���ݴ��䷽�򣬴����赽�ڴ�
    DMA_InitStructure.DMA_BufferSize = USART1_MAX_RX_LEN; // ��������С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // �����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // �ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // �������ݿ��
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // �ڴ����ݿ��
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // ����ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; // �����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // ���ڴ浽�ڴ洫��

    DMA_Init(DMA1_Channel5, &DMA_InitStructure); // ��ʼ��DMA1ͨ��5

    // DMA_USART1_TX  RAM->USART1�����ݴ���
    DMA_DeInit(DMA1_Channel4); // ��DMA��ͨ��4�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR); // �������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART1_TX_BUF; // �ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; // ���ݴ��䷽�򣬴��ڴ浽����
    DMA_InitStructure.DMA_BufferSize = USART1_MAX_TX_LEN; // ��������С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // �����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // �ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // �������ݿ��
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // �ڴ����ݿ��
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // ����ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; // �����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // ���ڴ浽�ڴ洫��

    DMA_Init(DMA1_Channel4, &DMA_InitStructure); // ��ʼ��DMA1ͨ��4
    
    // DMA1ͨ��5 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn; // NVICͨ������
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); // ����ָ���Ĳ�����ʼ��NVIC�Ĵ���

    // DMA1ͨ��4 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn; // NVICͨ������
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); // ����ָ���Ĳ�����ʼ��NVIC�Ĵ���

    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE); // ��USART1 Rx DMA�ж�
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE); // ��USART1 Tx DMA�ж�

    DMA_Cmd(DMA1_Channel5, ENABLE); // ʹDMAͨ��5����
    DMA_Cmd(DMA1_Channel4, DISABLE); // ʹDMAͨ��4ֹͣ����

    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); // ��������DMA����
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); // ��������DMA����
}

//����1�жϺ���
void USART1_IRQHandler(void)                	
{
	u8 *p;
	u8 USART1_RX_LEN = 0;											//�������ݳ���
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)			//����2�����ж�
	{
		USART_ReceiveData(USART1); 									//�������2�����ж�IDLE��־λ
		USART_ClearFlag(USART1,USART_FLAG_TC);						//���USART2��־λ
		DMA_Cmd(DMA1_Channel5, DISABLE );   						//�ر�USART1 TX DMA1 ��ָʾ��ͨ��
		USART1_RX_LEN = USART1_MAX_RX_LEN - DMA1_Channel5->CNDTR;	//��ý��յ����ֽ���
		if(witchbuf)                        						//֮ǰ�õ�u2rxbuf���л�Ϊu1rxbuf
		{
			p=u2rxbuf;												//�ȱ���ǰһ�����ݵ�ַ���л�������
			DMA1_Channel5->CMAR=(u32)u1rxbuf;						//�л�Ϊu1rxbuf��������ַ
			witchbuf=0;                     						//��һ���л�Ϊu2rxbuf
		}else                               						//֮ǰ�õ�u1rxbuf���л�Ϊu2rxbuf
		{
			p=u1rxbuf;												//�ȱ���ǰһ�����ݵ�ַ���л�������
			DMA1_Channel5->CMAR=(u32)u2rxbuf;						//�л�Ϊu2rxbuf��������ַ
			witchbuf=1;                     						//��һ���л�Ϊu1rxbuf
		}
		DMA1_Channel5->CNDTR = USART1_MAX_RX_LEN;					//DMAͨ����DMA����Ĵ�С
		DMA_Cmd(DMA1_Channel5, ENABLE);     						//ʹ��USART2 TX DMA1 ��ָʾ��ͨ��
		
		//���������ݴ���
		
		DMA_USART1_Tx_Data(p,USART1_RX_LEN);
	
  }
}

// DMA1ͨ��4�ж�
void DMA1_Channel4_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC4) != RESET)  // DMA������ɱ�־������DMA1_Channel4��
    {
        DMA_ClearITPendingBit(DMA1_IT_TC4);     // ����жϱ�־
        USART_ClearFlag(USART1, USART_FLAG_TC); // �������1�ı�־λ
        DMA_Cmd(DMA1_Channel4, DISABLE);        // �ر�USART1 TX DMA1 ��ָʾ��ͨ��
        USART1_TX_FLAG = 0;                    // USART1���ͱ�־���رգ�
    }
}

void USART1_printf(char *format, ...)
{
    va_list arg_ptr;                                                       // ʵ�����ɱ䳤�����б�

    while (USART1_TX_FLAG);                                                // �ȴ���һ�η�����ɣ�USART1_TX_FLAGΪ1�����ڷ������ݣ�

    va_start(arg_ptr, format);                                             // ��ʼ���ɱ�����б�����formatΪ�ɱ䳤�б����ʼ�㣨��һ��Ԫ�أ�

    // USART1_MAX_TX_LEN+1�ɽ��ܵ�����ַ���(���ֽ�����UNICODEһ���ַ������ֽ�)����ֹ��������Խ��
    vsnprintf((char*)USART1_TX_BUF, USART1_MAX_TX_LEN+1, format, arg_ptr); // ��USART1_TX_BUF���׵�ַ��ʼƴ�ϣ�ƴ��format���ݣ�USART1_MAX_TX_LEN+1���Ƴ��ȣ���ֹ��������Խ��

    va_end(arg_ptr);                                                        // ע�����ر�

    DMA_USART1_Tx_Data(USART1_TX_BUF, strlen((const char*)USART1_TX_BUF));  // ����USART1_TX_BUF����
}


// ���� DMA1 ͨ��5 �Ľ�������ж�
void DMA1_Channel5_IRQHandler(void)
{
    u8 *p;

    // ��� DMA1 ͨ��5 �Ƿ���ɴ���
    if (DMA_GetITStatus(DMA1_IT_TC5) != RESET)
    {
        // ��� DMA1 ͨ��5 �Ĵ�������жϱ�־
        DMA_ClearITPendingBit(DMA1_IT_TC5);

        // ��� USART1 ������ɱ�־λ�������Ҫ��
        USART_ClearFlag(USART1, USART_FLAG_TC);

        // �ر� DMA1 ͨ��5
        DMA_Cmd(DMA1_Channel5, DISABLE);

        // �л�������
        if (witchbuf)
        {
            p = u2rxbuf;                  // ����֮ǰ�����ݻ�������ַ
            DMA1_Channel5->CMAR = (u32)u1rxbuf;  // �л��� u1rxbuf ������
            witchbuf = 0;                // �����´�ʹ�� u2rxbuf
        }
        else
        {
            p = u1rxbuf;                  // ����֮ǰ�����ݻ�������ַ
            DMA1_Channel5->CMAR = (u32)u2rxbuf;  // �л��� u2rxbuf ������
            witchbuf = 1;                // �����´�ʹ�� u1rxbuf
        }

        // ���� DMA �����������
        DMA1_Channel5->CNDTR = USART1_MAX_RX_LEN;

        // ʹ�� DMA1 ͨ��5
        DMA_Cmd(DMA1_Channel5, ENABLE);

        // ******************�������������������ݴ������������******************
        DMA_USART1_Tx_Data(p, USART1_MAX_RX_LEN);
        // ******************�������������������ݴ������������******************
    }
}

