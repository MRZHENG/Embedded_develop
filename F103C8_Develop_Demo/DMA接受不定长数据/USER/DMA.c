#include "DMA.h"

u8 USART1_TX_BUF[USART1_MAX_TX_LEN]; 	//发送缓冲,最大USART1_MAX_TX_LEN字节
u8 u1rxbuf[USART1_MAX_RX_LEN];			//发送数据缓冲区1
u8 u2rxbuf[USART1_MAX_RX_LEN];			//发送数据缓冲区2
u8 witchbuf=0;                  		//标记当前使用的是哪个缓冲区,0：使用u1rxbuf；1：使用u2rxbuf
u8 USART1_TX_FLAG=0;					//USART2发送标志，启动发送时置1
u8 USART1_RX_FLAG=0;					//USART2接收标志，启动接收时置1


//DMA 发送应用源码
void DMA_USART1_Tx_Data(u8 *buffer, u32 size)
{
    while (USART1_TX_FLAG);                     // 等待上一次发送完成（USART1_TX_FLAG为1即还在发送数据）
    USART1_TX_FLAG = 1;                        // USART1发送标志（启动发送）

    DMA1_Channel4->CMAR = (uint32_t)buffer;    // 设置要发送的数据地址 (USART1 通道4)
    DMA1_Channel4->CNDTR = size;               // 设置要发送的字节数目
    DMA_Cmd(DMA1_Channel4, ENABLE);            // 开始DMA发送
}



void DMA1_USART1_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // 使能DMA1时钟

    // DMA_USART1_RX  USART1->RAM的数据传输
    DMA_DeInit(DMA1_Channel5); // 将DMA的通道5寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR); // 外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)u1rxbuf; // 内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 数据传输方向，从外设到内存
    DMA_InitStructure.DMA_BufferSize = USART1_MAX_RX_LEN; // 缓冲区大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // 内存数据宽度
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // 正常模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; // 高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // 非内存到内存传输

    DMA_Init(DMA1_Channel5, &DMA_InitStructure); // 初始化DMA1通道5

    // DMA_USART1_TX  RAM->USART1的数据传输
    DMA_DeInit(DMA1_Channel4); // 将DMA的通道4寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR); // 外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART1_TX_BUF; // 内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; // 数据传输方向，从内存到外设
    DMA_InitStructure.DMA_BufferSize = USART1_MAX_TX_LEN; // 缓冲区大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // 内存数据宽度
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // 正常模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; // 高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // 非内存到内存传输

    DMA_Init(DMA1_Channel4, &DMA_InitStructure); // 初始化DMA1通道4
    
    // DMA1通道5 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn; // NVIC通道设置
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); // 根据指定的参数初始化NVIC寄存器

    // DMA1通道4 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn; // NVIC通道设置
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); // 根据指定的参数初始化NVIC寄存器

    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE); // 开USART1 Rx DMA中断
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE); // 开USART1 Tx DMA中断

    DMA_Cmd(DMA1_Channel5, ENABLE); // 使DMA通道5工作
    DMA_Cmd(DMA1_Channel4, DISABLE); // 使DMA通道4停止工作

    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); // 开启串口DMA发送
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); // 开启串口DMA接收
}

//串口1中断函数
void USART1_IRQHandler(void)                	
{
	u8 *p;
	u8 USART1_RX_LEN = 0;											//接收数据长度
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)			//串口2空闲中断
	{
		USART_ReceiveData(USART1); 									//清除串口2空闲中断IDLE标志位
		USART_ClearFlag(USART1,USART_FLAG_TC);						//清除USART2标志位
		DMA_Cmd(DMA1_Channel5, DISABLE );   						//关闭USART1 TX DMA1 所指示的通道
		USART1_RX_LEN = USART1_MAX_RX_LEN - DMA1_Channel5->CNDTR;	//获得接收到的字节数
		if(witchbuf)                        						//之前用的u2rxbuf，切换为u1rxbuf
		{
			p=u2rxbuf;												//先保存前一次数据地址再切换缓冲区
			DMA1_Channel5->CMAR=(u32)u1rxbuf;						//切换为u1rxbuf缓冲区地址
			witchbuf=0;                     						//下一次切换为u2rxbuf
		}else                               						//之前用的u1rxbuf，切换为u2rxbuf
		{
			p=u1rxbuf;												//先保存前一次数据地址再切换缓冲区
			DMA1_Channel5->CMAR=(u32)u2rxbuf;						//切换为u2rxbuf缓冲区地址
			witchbuf=1;                     						//下一次切换为u1rxbuf
		}
		DMA1_Channel5->CNDTR = USART1_MAX_RX_LEN;					//DMA通道的DMA缓存的大小
		DMA_Cmd(DMA1_Channel5, ENABLE);     						//使能USART2 TX DMA1 所指示的通道
		
		//这里作数据处理
		
		DMA_USART1_Tx_Data(p,USART1_RX_LEN);
	
  }
}

// DMA1通道4中断
void DMA1_Channel4_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC4) != RESET)  // DMA接收完成标志（对于DMA1_Channel4）
    {
        DMA_ClearITPendingBit(DMA1_IT_TC4);     // 清除中断标志
        USART_ClearFlag(USART1, USART_FLAG_TC); // 清除串口1的标志位
        DMA_Cmd(DMA1_Channel4, DISABLE);        // 关闭USART1 TX DMA1 所指示的通道
        USART1_TX_FLAG = 0;                    // USART1发送标志（关闭）
    }
}

void USART1_printf(char *format, ...)
{
    va_list arg_ptr;                                                       // 实例化可变长参数列表

    while (USART1_TX_FLAG);                                                // 等待上一次发送完成（USART1_TX_FLAG为1即还在发送数据）

    va_start(arg_ptr, format);                                             // 初始化可变参数列表，设置format为可变长列表的起始点（第一个元素）

    // USART1_MAX_TX_LEN+1可接受的最大字符数(非字节数，UNICODE一个字符两个字节)，防止产生数组越界
    vsnprintf((char*)USART1_TX_BUF, USART1_MAX_TX_LEN+1, format, arg_ptr); // 从USART1_TX_BUF的首地址开始拼合，拼合format内容；USART1_MAX_TX_LEN+1限制长度，防止产生数组越界

    va_end(arg_ptr);                                                        // 注意必须关闭

    DMA_USART1_Tx_Data(USART1_TX_BUF, strlen((const char*)USART1_TX_BUF));  // 发送USART1_TX_BUF内容
}


// 处理 DMA1 通道5 的接收完成中断
void DMA1_Channel5_IRQHandler(void)
{
    u8 *p;

    // 检查 DMA1 通道5 是否完成传输
    if (DMA_GetITStatus(DMA1_IT_TC5) != RESET)
    {
        // 清除 DMA1 通道5 的传输完成中断标志
        DMA_ClearITPendingBit(DMA1_IT_TC5);

        // 清除 USART1 传输完成标志位（如果需要）
        USART_ClearFlag(USART1, USART_FLAG_TC);

        // 关闭 DMA1 通道5
        DMA_Cmd(DMA1_Channel5, DISABLE);

        // 切换缓冲区
        if (witchbuf)
        {
            p = u2rxbuf;                  // 保存之前的数据缓冲区地址
            DMA1_Channel5->CMAR = (u32)u1rxbuf;  // 切换到 u1rxbuf 缓冲区
            witchbuf = 0;                // 设置下次使用 u2rxbuf
        }
        else
        {
            p = u1rxbuf;                  // 保存之前的数据缓冲区地址
            DMA1_Channel5->CMAR = (u32)u2rxbuf;  // 切换到 u2rxbuf 缓冲区
            witchbuf = 1;                // 设置下次使用 u1rxbuf
        }

        // 设置 DMA 传输的数据量
        DMA1_Channel5->CNDTR = USART1_MAX_RX_LEN;

        // 使能 DMA1 通道5
        DMA_Cmd(DMA1_Channel5, ENABLE);

        // ******************↓↓↓↓↓这里作数据处理↓↓↓↓↓******************
        DMA_USART1_Tx_Data(p, USART1_MAX_RX_LEN);
        // ******************↑↑↑↑↑这里作数据处理↑↑↑↑↑******************
    }
}

