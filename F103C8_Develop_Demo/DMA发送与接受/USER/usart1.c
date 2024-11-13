#include "usart1.h"
#include <stdarg.h>

/*串口结构体，我们把 DMA 传输的缓存地址都赋值到了
  这个结构体里 */
_UART_Stru_ UartStru = 
{
    Rx1Buf[0], Tx1Buf,   //串口1收、送缓存
    0              //接收缓存标记
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
    GPIO_InitTypeDef  GPIO_InitStructure;  //配置 GPIO 结构体
    USART_InitTypeDef USART_InitStruct;    //配置串口结构体
    NVIC_InitTypeDef  NVIC_InitStruct;     //中断配置

    //使能 USART1，GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);

    //PA9 (UART1_TX)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //输出频率
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;        //引脚 PA9
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //PA10 (UART1_RX)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  //浮空输入
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;            //引脚 PA10
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //串口 USART1 配置
    USART_InitStruct.USART_BaudRate            = baud;                 //波特率
    USART_InitStruct.USART_WordLength          = USART_WordLength_8b;  //数据位 8 位
    USART_InitStruct.USART_StopBits            = USART_StopBits_1;     //停止位 1
    USART_InitStruct.USART_Parity              = USART_Parity_No;      //无奇偶校验
    USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;   //能收，能发
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //无硬件数据流控制
    USART_Init(USART1, &USART_InitStruct);

    //串口 USART1 空闲中断配置
    NVIC_InitStruct.NVIC_IRQChannel                   = USART1_IRQn;  //中断通道
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x03;         //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0x01;         //响应优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);                   //使能串口 USART1 空闲中断
    USART_Cmd(USART1, ENABLE);                                       //使能串口

    DMA1_USART1_Init(3);                                              //串口1 DMA1 传输配置
}


void USART1_IRQHandler(void)
{
    u8 data;
    s32 i;

    //串口空闲中断
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) 
    {
        /*读取这两个寄存器的值，
        只是为了清除中断标志位，没有其他用处*/
        data = USART1->SR;
        data = USART1->DR;

        /*第 1 个缓存接收到数据*/
        if(UartStru.uart1_num <= 0)
        {
            /*获取 DMA 传输的字节数，也就是我们的串口接收到了多少个字节，
              保存到接收缓存最后一个字节，这一步不是必须的，但很有用，
              我们应用层的程序可以清楚的知道串口收到多少数据*/
            Rx1Buf[UartStru.uart1_num][BUF_SIZE_RX1-1] =(*(volatile uint32_t *)(0x40020014)) ;
            UartStru.uart1_num = 1;                                        //缓存标记
            /*这里是我们通过 DMA 接收到的第 1 段串口数据，保存到了 Rx1Buf[0] 里，
              现在就需要我们切换缓存，把接下来收到的数据保存到 Rx1Buf[1] 里 */
            DMA1_RxTx_Start(DMA1_Channel5, Rx1Buf[UartStru.uart1_num], DMA_SIZE_RX1); //DMA
        }
        /*其他缓存接收到数据*/
        else
        {
            /*通过这遍历，我们判断出那段缓存里已经存储了数据*/
            for(i=0; i<BUF_NUM_RX1; i++)
            {
                if(UartStru.uart1_num == i)
                {
                    //接收缓存最后一个字节保存接收到的字节数
                    Rx1Buf[UartStru.uart1_num][BUF_SIZE_RX1-1] = (*(volatile uint32_t *)(0x40020014)) ;
                    UartStru.uart1_num++;                                   //缓存标记
                    //DMA 串口1 接收缓存切换，依据缓存标记，我们切换到未使用的缓存
                    DMA1_RxTx_Start(DMA1_Channel5, Rx1Buf[UartStru.uart1_num], DMA_SIZE_RX1);  
                    break;
                    //return;    //也可以直接退出中断函数
                }
            }
        }
    }
}


