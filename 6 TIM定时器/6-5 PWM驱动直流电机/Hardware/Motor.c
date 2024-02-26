#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init()
{
	PWM_Init();
	
	// 初始化 PA4 和 PA5
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;  //推挽输出模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

// Speed = [-100, 100], 因为 ARR = 100, 占空比范围 0 ~ 100
void Motor_SetSpeed(int8_t Speed)
{
	if(Speed < 0)
	{
		// 旋转方向控制
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		
		// 旋转速度
		PWM_SetCompare3(-Speed);
	}
	else
	{
		// 旋转方向控制
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		
		// 旋转速度
		PWM_SetCompare3(Speed);
	}
}
