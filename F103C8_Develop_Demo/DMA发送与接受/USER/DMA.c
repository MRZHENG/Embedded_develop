#include "DMA.h"

/*
DMA1 USART1 初始化
 DMA1通道4对应，串口1发送
 DMA1通道5对应，串口1接收        
 RxTx  接收/发送 DMA1通道初始化（1接收 2发送 3收发 0不开启DMA）

*/


u8 Rx1Buf[BUF_NUM_RX1][BUF_SIZE_RX1] = {{0}, {0}, {0}, {0}};  //串口1 的接收缓存 
u8 Tx1Buf[BUF_SIZE_TX1] = {0};                                //串口1 的发送缓存

void DMA1_USART1_Init(u8 RxTx)
{
    DMA_InitTypeDef  DMA_Str;           //DMA配置结构体
    NVIC_InitTypeDef NVIC_InitStruct;   //中断配置

    if(!RxTx)
        return;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    //开启 DMA1 时钟

    //DMA 接收通道配置
    if(RxTx == 1 || RxTx == 3)
    {
        USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);   //启动 USART1 发送 DMA 传输
        DMA_DeInit(DMA1_Channel5);                       //通道 5 对应串口1 接收
    }
    //DMA 发送通道配置
    if(RxTx == 2 || RxTx == 3)
    {
        USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);    //启动 USART1 发送 DMA 传输
        DMA_DeInit(DMA1_Channel4);                        //通道 4 对应串口1 发送
    }

    //DMA 配置
    DMA_Str.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   //传输的时候外设地址是否递增（否）

    /*DMA 往内存传输一个字节，每次都会把数据放到内存往后偏移 1 个地址的内存段追中*/
    DMA_Str.DMA_MemoryInc          = DMA_MemoryInc_Enable;        //传输的时候内存地址是否需要递增（是）
    DMA_Str.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度（1字节，8位）
    DMA_Str.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     //内存数据长度（1字节，8位）
    DMA_Str.DMA_M2M                = DMA_M2M_Disable;             //是否内存到内存传输中使能（否）
    DMA_Str.DMA_PeripheralBaseAddr = (u32)&USART1->DR;            //指定DMA信道的外设基地址
    DMA_Str.DMA_Priority           = DMA_Priority_High;           //DMA通道软件优先级（高）

    //USART1接收，DMA1通道5配置
    if(RxTx==1 || RxTx==3) 
    {   
        /*DMA_SIZE_RX1 这个传输大小一定要大于我们串口单次接收的最大字节数，最好有一点余量，
          大了没关系，但是不能小 */
         DMA_Str.DMA_BufferSize     = DMA_SIZE_RX1;           //DMA 一次传输数据大小

        /*就是 DMA 从串口传过来的数据放到 Rx1Buf[0] 地址起始的内存中
          （ Rx1Buf[0] 是一个二维数组，所以 Rx1Buf[0] 是一个指针，不是变量）*/
         DMA_Str.DMA_MemoryBaseAddr = (u32)Rx1Buf[0];         //指定 DMA 信道的内存基地址         

        //就是把外设接收到的数据传输到内存中
         DMA_Str.DMA_DIR            = DMA_DIR_PeripheralSRC;  //数据传输方向（从外设到内存）

        /*循环模式比如你设置了传输数据大小是 8 字节， DMA 在传输了 8 字节的数据之后
        DMA 会自动开始下一轮传输，传输的数据还是放在基地址开始的内存中，
        会覆盖之前的那段内存
        其实这里选择普通模式也是可以的，因为我们每接收到 1 段数据，就会切换缓存，从新开始一次传输*/
         DMA_Str.DMA_Mode           = DMA_Mode_Circular;      //指定DMA通道操作模式（循环模式）
        DMA_Init(DMA1_Channel5, &DMA_Str);                   //DMA1通道5初始化
        DMA_Cmd(DMA1_Channel5, ENABLE);                      //使能 DMA1 通道5，开始传输
    }
    //USART1发送，DMA1通道4配置
    if(RxTx==2 || RxTx==3) 
    {
        DMA_Str.DMA_BufferSize     = DMA_SIZE_TX1;           //DMA 一次传输数据大小
        DMA_Str.DMA_MemoryBaseAddr = (u32)Tx1Buf;            //指定 DMA 信道的内存基地址          
        DMA_Str.DMA_DIR            = DMA_DIR_PeripheralDST;  //数据传输方向（从内存到外设）           

         /*发送，我们没有选择循环模式，而是普通模式，普通模式只传输一次就结束，不会一直循环传输，
           目的就是用到的时候才发 */
         DMA_Str.DMA_Mode           = DMA_Mode_Normal;        //指定DMA通道操作模式（普通模式）
        DMA_Init(DMA1_Channel4, &DMA_Str);                   //DMA1 通道 4 初始化

        /*在这里我们配置完发送通道后没有开启传输，是因为我们发送的数据也是不定长的，
          而且串口的发送是掌握在我们手上的，什么时候发送？发送的数据多长？都是已知的。
          而不是像接收一样，要一直处于一种待命的状态。所以我们会写一个通过 DMA 发送串口的函数接口。
          在需要的时候才通过 DMA 传输发送串口*/
    }
}


void DMA1_RxTx_Start(DMA_Channel_TypeDef* DMAy_Channelx, u8 *RxTx_Buf, u16 len)
{
    /*在配置之前我们要先把 DMA 相关的通道关闭*/
    DMA_Cmd(DMAy_Channelx, DISABLE);                      //关闭DMA1指定通道

    /*我们把要发送的数据放到 RxTx_Buf 这段内存中就可以了*/
    DMAy_Channelx -> CMAR = (u32)RxTx_Buf;                //DMA传输的基地址
    DMAy_Channelx -> CNDTR = len;                         //DMA传输的数据长度
    DMA_Cmd(DMAy_Channelx, ENABLE);                       //开启DMA1指定通道
}


