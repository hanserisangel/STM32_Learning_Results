#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init()
{
	PWM_Init();
	
	// ��ʼ�� PA4 �� PA5
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;  //�������ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

// Speed = [-100, 100], ��Ϊ ARR = 100, ռ�ձȷ�Χ 0 ~ 100
void Motor_SetSpeed(int8_t Speed)
{
	if(Speed < 0)
	{
		// ��ת�������
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		
		// ��ת�ٶ�
		PWM_SetCompare3(-Speed);
	}
	else
	{
		// ��ת�������
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		
		// ��ת�ٶ�
		PWM_SetCompare3(Speed);
	}
}
