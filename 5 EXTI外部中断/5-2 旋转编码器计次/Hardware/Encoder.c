#include "stm32f10x.h"                  // Device header

int16_t CountEncoder;

void Encoder_Init()
{
	// ���� GPIOB �� AFIO �� RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	// GPIOB ��ʼ�� PB0 �� PB1
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU ;  
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	// AFIO ��ʼ�� PB0 �� PB1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	
	// EXTI ��ʼ��
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=EXTI_Line0 | EXTI_Line1; // �ж���· 0 �� 1
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt; // ��Ӧ��ʽ: �ж���Ӧ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; // ������ʽ: �½���
	EXTI_Init(&EXTI_InitStructure);
	
	// NVIC ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ���ȼ�����: 2 + 2
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn; // �ж�ͨ�� 0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; // ��ռ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; // ��Ӧ
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn; // �ж�ͨ�� 1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; // ��ռ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; // ��Ӧ
	NVIC_Init(&NVIC_InitStructure);
}

int16_t Encoder_Get() // CountEncoder �ı仯��
{
	int16_t temp;
	temp = CountEncoder;
	CountEncoder = 0;
	return temp;
}

void EXTI0_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
		{
			CountEncoder --; // ��ת
		}
		
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
		{
			CountEncoder ++; // ��ת
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
