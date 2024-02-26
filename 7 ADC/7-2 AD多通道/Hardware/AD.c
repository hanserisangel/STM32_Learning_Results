#include "stm32f10x.h"                  // Device header

void AD_Init()
{
	// 开启 APB2 上的 ADC1 和 GPIOA, 并 6 分频
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 72M / 6 = 12MHz
	
	// 初始化 PA0/PA1/PA2/PA3
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN; // 模拟输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 初始化 ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent; // 独立 AD 模式
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; // 数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; // 无外部触发源
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE; // 单次转换
	ADC_InitStructure.ADC_ScanConvMode=DISABLE; // 非扫描模式
	ADC_InitStructure.ADC_NbrOfChannel=1; // 通道数 1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// 使能 ADC
	ADC_Cmd(ADC1, ENABLE);
	
	// 校准
	ADC_ResetCalibration(ADC1); // 复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); // 0 表示复位校准完成
	ADC_StartCalibration(ADC1); // 开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET); // 0 表示校准完成
	
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 触发软件中断(扫描模式位置)
}

uint16_t ADC_GetValue(uint8_t ADC_Channel)
{
	// ADC 通道选择: 序列 1, 55.5 个ADC周期的采样周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 触发软件中断
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); // 1 表示转换完成
	
	return ADC_GetConversionValue(ADC1);
	
	// return ADC_GetConversionValue(ADC1); // 扫描模式位置
}
