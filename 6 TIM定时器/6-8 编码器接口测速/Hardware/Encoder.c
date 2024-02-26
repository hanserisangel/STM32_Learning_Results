#include "stm32f10x.h"                  // Device header

void Encoder_Init()
{
	// ���� APB1 �� TIM3 �� APB2 �� GPIOA �� RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// ��ʼ�� PA6 �� PA7
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU ; // ��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// ʱ����Ԫ: �������ӿ��й�
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// �޹�
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // ����: ���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // ARR, �����̼���
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; // PSC 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // �߼���ʱ��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	// ���벶��Ԫ
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; // ѡ��ͨ�� 1
	TIM_ICInitStructure.TIM_ICFilter=0xF; // �˲�
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2; // ѡ��ͨ�� 2
	TIM_ICInitStructure.TIM_ICFilter=0xF; // �˲�
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	// ���ñ������ӿ�: ����ģʽ, A ��� B ��ļ���ѡ��
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	// ����ʹ��
	TIM_Cmd(TIM3, ENABLE);
}

// 1s բ��ʱ��� CNT ֵ, ��ϲ��ܷ����ٶ�
int16_t Encoder_Get()
{
	int16_t temp;
	temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0); // CNT ����
	return temp;
}
