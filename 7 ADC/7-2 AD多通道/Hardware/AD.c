#include "stm32f10x.h"                  // Device header

void AD_Init()
{
	// ���� APB2 �ϵ� ADC1 �� GPIOA, �� 6 ��Ƶ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 72M / 6 = 12MHz
	
	// ��ʼ�� PA0/PA1/PA2/PA3
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN; // ģ������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// ��ʼ�� ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent; // ���� AD ģʽ
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; // �����Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; // ���ⲿ����Դ
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE; // ����ת��
	ADC_InitStructure.ADC_ScanConvMode=DISABLE; // ��ɨ��ģʽ
	ADC_InitStructure.ADC_NbrOfChannel=1; // ͨ���� 1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// ʹ�� ADC
	ADC_Cmd(ADC1, ENABLE);
	
	// У׼
	ADC_ResetCalibration(ADC1); // ��λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); // 0 ��ʾ��λУ׼���
	ADC_StartCalibration(ADC1); // ��ʼУ׼
	while(ADC_GetCalibrationStatus(ADC1) == SET); // 0 ��ʾУ׼���
	
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE); // ��������ж�(ɨ��ģʽλ��)
}

uint16_t ADC_GetValue(uint8_t ADC_Channel)
{
	// ADC ͨ��ѡ��: ���� 1, 55.5 ��ADC���ڵĲ�������
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // ��������ж�
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); // 1 ��ʾת�����
	
	return ADC_GetConversionValue(ADC1);
	
	// return ADC_GetConversionValue(ADC1); // ɨ��ģʽλ��
}
