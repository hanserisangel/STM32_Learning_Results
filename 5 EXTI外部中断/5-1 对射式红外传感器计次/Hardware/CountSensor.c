#include "stm32f10x.h"                  // Device header

uint16_t CountSenor;

void CountSensor_Init()
{
	// 开启 GPIOB 和 AFIO 的 RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	// GPIOB 初始化 PB14
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU ;  //上拉输入模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	// AFIO 初始化 PB14
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	// EXTI 初始化
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=EXTI_Line14; // 中断线路 14
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt; // 响应方式: 中断触发
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; // 触发方式: 下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	// NVIC 初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 优先级分组: 2 + 2
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn; // 中断通道 14
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; // 抢占
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; // 响应
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t CountSensor_Get()
{
	return CountSenor;
}

void EXTI15_10_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line14) == SET) // 判断是否为 EXTI14 中断
	{
		CountSenor ++;
		
		EXTI_ClearITPendingBit(EXTI_Line14); // 清除中断标志位
	}
}