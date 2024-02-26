#include "stm32f10x.h"                  // Device header

uint16_t ADValue[4];

void AD_Init()
{
	// ���� APB2 �ϵ� ADC1 �� GPIOA �� AHB �ϵ� DMA, �� 6 ��Ƶ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 72M / 6 = 12MHz
	
	// ��ʼ�� PA0/PA1/PA2/PA3
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN; // ģ������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// ADC ͨ��ѡ��: 55.5 ��ADC���ڵĲ�������; ͨ��0-����1, ͨ��1-����2, ...
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	// ��ʼ�� ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent; // ���� AD ģʽ
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; // �����Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; // ���ⲿ����Դ
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE; // ����ת��
	ADC_InitStructure.ADC_ScanConvMode=ENABLE; // ɨ��ģʽ
	ADC_InitStructure.ADC_NbrOfChannel=4; // ͨ���� 4
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// ��ʼ�� DMA1 �� ͨ�� 1
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize=4; // ���������
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC; // ���䷽��: ������ΪԴ
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable; // ѡ�񴥷���ʽ: Ӳ������
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)ADValue; // �洢����ʼ��ַ
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord; // �洢�����ݿ��: 0.5����
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;  // �洢����ַ����
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular; // ����ģʽ: �Զ���װ
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t) &ADC1->DR; // ������ʼ��ַ: ADֵ�Ĵ���
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord; // �������ݿ��: 0.5����
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;  // �����ַ������
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium; // ���ȼ�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	// ʹ�� DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// ʹ�� DMA �����ź�
	ADC_DMACmd(ADC1, ENABLE);
	
	// ʹ�� ADC
	ADC_Cmd(ADC1, ENABLE);
	
	// У׼
	ADC_ResetCalibration(ADC1); // ��λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); // 0 ��ʾ��λУ׼���
	ADC_StartCalibration(ADC1); // ��ʼУ׼
	while(ADC_GetCalibrationStatus(ADC1) == SET); // 0 ��ʾУ׼���
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // ��������ж�
}
