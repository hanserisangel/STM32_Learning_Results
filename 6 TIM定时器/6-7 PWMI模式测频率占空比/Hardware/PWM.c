#include "stm32f10x.h"                  // Device header

void PWM_Init()
{
	// ���� APB1 �� TIM2 �� APB2 �� GPIOA �� RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// ��ʼ�� PA0
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP ; // �����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// ʱ��Դѡ��
	TIM_InternalClockConfig(TIM2); // �ڲ�ʱ��
	
	// ʱ����Ԫ: 1kHz
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// �޹�
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // ARR 
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // �߼���ʱ��
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	// ����Ƚϵ�Ԫ
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); // ��Ϊ�ṹ���������ȫ����, ������Ĭ��
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1; // ����Ƚ�ģʽ
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; // ��ƽ����ת
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable; // ���ʹ��
	TIM_OCInitStructure.TIM_Pulse=0; // CCR �Ĵ���ֵ
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	// ����ʹ��
	TIM_Cmd(TIM2, ENABLE);
}

// ռ�ձ�, Compare = [0, 100], ��Ϊ ARR = 100
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}

// ����Ƶ��
void PWM_SetPrescaler(uint16_t Prescale)
{
	TIM_PrescalerConfig(TIM2, Prescale, TIM_PSCReloadMode_Update);
}
