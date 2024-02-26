#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_size;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t size) // A → B
{
	// 开启 AHB 上的 DMA 的 RCC
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	MyDMA_size = size;
	
	// 初始化 DMA1 的 通道 1
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize=size; // 传输寄存器
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC; // 传输方向: 外设作为源
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable; // 选择触发方式: 软件触发
	DMA_InitStructure.DMA_MemoryBaseAddr=AddrB; // 存储器起始地址
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte; // 存储器数据宽度: 1 个字节
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;  // 存储器地址自增
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal; // 传输模式: 无自动重装
	DMA_InitStructure.DMA_PeripheralBaseAddr=AddrA; // 外设起始地址
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte; // 外设数据宽度: 1 个字节
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Enable;  // 外设地址自增
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium; // 优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	// 使能 DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

void MyDMA_Transfer()
{
	DMA_Cmd(DMA1_Channel1, DISABLE); // 失能后才能写传输计数器
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_size); // 重新写入传输计数器
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET); // 1 表示转运完成
	DMA_ClearFlag(DMA1_FLAG_TC1); // 软件清除
}
