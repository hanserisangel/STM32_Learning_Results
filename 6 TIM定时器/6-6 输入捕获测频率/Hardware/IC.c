#include "stm32f10x.h"                  // Device header

void IC_Init()
{
	// ���� APB1 �� TIM3 �� APB2 �� GPIOA �� RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// ��ʼ�� PA6
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU ; // ��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// ʱ��Դѡ��
	TIM_InternalClockConfig(TIM3); // �ڲ�ʱ��
	
	// ʱ����Ԫ: 1MHz
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// �޹�
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // ARR, �����̼���
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // PSC 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // �߼���ʱ��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	// ���벶��Ԫ
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; // ѡ��ͨ�� 1
	TIM_ICInitStructure.TIM_ICFilter=0xF; // �˲�
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; // �����ش���
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1; // ��Ƶ
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI; // ����ѡ��, ֱ��
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	//���Ӵ���ģʽ
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1); // ��ģʽ����Դ
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); // ��ģʽѡ��
	
	// ����ʹ��
	TIM_Cmd(TIM3, ENABLE);
}

// ����Ƶ��, (1M / CCR) Hz
uint32_t IC_GetFreq()
{
	return 1000000 / TIM_GetCapture1(TIM3);
}
