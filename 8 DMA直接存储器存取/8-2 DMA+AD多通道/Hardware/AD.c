#include "stm32f10x.h"                  // Device header

uint16_t ADValue[4];

void AD_Init()
{
	// 开启 APB2 上的 ADC1 和 GPIOA 和 AHB 上的 DMA, 并 6 分频
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 72M / 6 = 12MHz
	
	// 初始化 PA0/PA1/PA2/PA3
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN; // 模拟输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// ADC 通道选择: 55.5 个ADC周期的采样周期; 通道0-序列1, 通道1-序列2, ...
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	// 初始化 ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent; // 独立 AD 模式
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; // 数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; // 无外部触发源
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE; // 连续转换
	ADC_InitStructure.ADC_ScanConvMode=ENABLE; // 扫描模式
	ADC_InitStructure.ADC_NbrOfChannel=4; // 通道数 4
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// 初始化 DMA1 的 通道 1
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize=4; // 传输计数器
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC; // 传输方向: 外设作为源
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable; // 选择触发方式: 硬件触发
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)ADValue; // 存储器起始地址
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord; // 存储器数据宽度: 0.5个字
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;  // 存储器地址自增
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular; // 传输模式: 自动重装
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t) &ADC1->DR; // 外设起始地址: AD值寄存器
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord; // 外设数据宽度: 0.5个字
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;  // 外设地址不自增
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium; // 优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	// 使能 DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// 使能 DMA 触发信号
	ADC_DMACmd(ADC1, ENABLE);
	
	// 使能 ADC
	ADC_Cmd(ADC1, ENABLE);
	
	// 校准
	ADC_ResetCalibration(ADC1); // 复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); // 0 表示复位校准完成
	ADC_StartCalibration(ADC1); // 开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET); // 0 表示校准完成
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 触发软件中断
}
