#include "stm32f10x.h"                  // Device header

uint16_t CountSenor;

void CountSensor_Init()
{
	// ���� GPIOB �� AFIO �� RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	// GPIOB ��ʼ�� PB14
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU ;  //��������ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	// AFIO ��ʼ�� PB14
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	// EXTI ��ʼ��
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=EXTI_Line14; // �ж���· 14
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt; // ��Ӧ��ʽ: �жϴ���
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; // ������ʽ: �½��ش���
	EXTI_Init(&EXTI_InitStructure);
	
	// NVIC ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ���ȼ�����: 2 + 2
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn; // �ж�ͨ�� 14
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; // ��ռ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; // ��Ӧ
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t CountSensor_Get()
{
	return CountSenor;
}

void EXTI15_10_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line14) == SET) // �ж��Ƿ�Ϊ EXTI14 �ж�
	{
		CountSenor ++;
		
		EXTI_ClearITPendingBit(EXTI_Line14); // ����жϱ�־λ
	}
}