#include "usart1.h"
#include <stdarg.h>

/*���ڽṹ�壬���ǰ� DMA ����Ļ����ַ����ֵ����
  ����ṹ���� */
_UART_Stru_ UartStru = 
{
    Rx1Buf[0], Tx1Buf,   //����1�ա��ͻ���
    0              //���ջ�����
};

int fputc(int ch, FILE *f) {
    USART_SendData(USART1, (uint8_t)ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return ch;
}



int fgetc(FILE *f) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(USART1);
}



void USART1_Init(u32 baud)
{
    GPIO_InitTypeDef  GPIO_InitStructure;  //���� GPIO �ṹ��
    USART_InitTypeDef USART_InitStruct;    //���ô��ڽṹ��
    NVIC_InitTypeDef  NVIC_InitStruct;     //�ж�����

    //ʹ�� USART1��GPIOA ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);

    //PA9 (UART1_TX)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;   //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //���Ƶ��
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;        //���� PA9
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //PA10 (UART1_RX)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  //��������
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;            //���� PA10
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //���� USART1 ����
    USART_InitStruct.USART_BaudRate            = baud;                 //������
    USART_InitStruct.USART_WordLength          = USART_WordLength_8b;  //����λ 8 λ
    USART_InitStruct.USART_StopBits            = USART_StopBits_1;     //ֹͣλ 1
    USART_InitStruct.USART_Parity              = USART_Parity_No;      //����żУ��
    USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;   //���գ��ܷ�
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
    USART_Init(USART1, &USART_InitStruct);

    //���� USART1 �����ж�����
    NVIC_InitStruct.NVIC_IRQChannel                   = USART1_IRQn;  //�ж�ͨ��
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x03;         //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0x01;         //��Ӧ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);                   //ʹ�ܴ��� USART1 �����ж�
    USART_Cmd(USART1, ENABLE);                                       //ʹ�ܴ���

    DMA1_USART1_Init(3);                                              //����1 DMA1 ��������
}


void USART1_IRQHandler(void)
{
    u8 data;
    s32 i;

    //���ڿ����ж�
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) 
    {
        /*��ȡ�������Ĵ�����ֵ��
        ֻ��Ϊ������жϱ�־λ��û�������ô�*/
        data = USART1->SR;
        data = USART1->DR;

        /*�� 1 ��������յ�����*/
        if(UartStru.uart1_num <= 0)
        {
            /*��ȡ DMA ������ֽ�����Ҳ�������ǵĴ��ڽ��յ��˶��ٸ��ֽڣ�
              ���浽���ջ������һ���ֽڣ���һ�����Ǳ���ģ��������ã�
              ����Ӧ�ò�ĳ�����������֪�������յ���������*/
            Rx1Buf[UartStru.uart1_num][BUF_SIZE_RX1-1] =(*(volatile uint32_t *)(0x40020014)) ;
            UartStru.uart1_num = 1;                                        //������
            /*����������ͨ�� DMA ���յ��ĵ� 1 �δ������ݣ����浽�� Rx1Buf[0] �
              ���ھ���Ҫ�����л����棬�ѽ������յ������ݱ��浽 Rx1Buf[1] �� */
            DMA1_RxTx_Start(DMA1_Channel5, Rx1Buf[UartStru.uart1_num], DMA_SIZE_RX1); //DMA
        }
        /*����������յ�����*/
        else
        {
            /*ͨ��������������жϳ��Ƕλ������Ѿ��洢������*/
            for(i=0; i<BUF_NUM_RX1; i++)
            {
                if(UartStru.uart1_num == i)
                {
                    //���ջ������һ���ֽڱ�����յ����ֽ���
                    Rx1Buf[UartStru.uart1_num][BUF_SIZE_RX1-1] = (*(volatile uint32_t *)(0x40020014)) ;
                    UartStru.uart1_num++;                                   //������
                    //DMA ����1 ���ջ����л������ݻ����ǣ������л���δʹ�õĻ���
                    DMA1_RxTx_Start(DMA1_Channel5, Rx1Buf[UartStru.uart1_num], DMA_SIZE_RX1);  
                    break;
                    //return;    //Ҳ����ֱ���˳��жϺ���
                }
            }
        }
    }
}


