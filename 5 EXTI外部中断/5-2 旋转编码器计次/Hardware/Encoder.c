#include "stm32f10x.h"                  // Device header

int16_t CountEncoder;

void Encoder_Init()
{
	// 开启 GPIOB 和 AFIO 的 RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	// GPIOB 初始化 PB0 和 PB1
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU ;  
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	// AFIO 初始化 PB0 和 PB1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	
	// EXTI 初始化
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=EXTI_Line0 | EXTI_Line1; // 中断线路 0 和 1
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt; // 响应方式: 中断响应
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; // 触发方式: 下降沿
	EXTI_Init(&EXTI_InitStructure);
	
	// NVIC 初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 优先级分组: 2 + 2
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn; // 中断通道 0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; // 抢占
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; // 响应
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn; // 中断通道 1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; // 抢占
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; // 响应
	NVIC_Init(&NVIC_InitStructure);
}

int16_t Encoder_Get() // CountEncoder 的变化量
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
			CountEncoder --; // 反转
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
			CountEncoder ++; // 正转
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
