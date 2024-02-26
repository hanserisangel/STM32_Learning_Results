#include "stm32f10x.h"

void Timer_Init()
{
	// 开启 APB1 总线 TIM2 的 RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 时钟源选择
	TIM_InternalClockConfig(TIM2); // 内部时钟
	
	// 设置时基单元: 1s
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 无关: 消抖滤波, f 采样速率, N 采样数
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1; // ARR 自动重装值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // PSC 预分频值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器值(高级计时器)
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	// 使能中断控制
	TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除中断标志
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 更新中断
	
	// NVIC 初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 优先级分组: 2 + 2
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; // 定时器 TIM2 中断
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; // 抢占
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; // 响应
	NVIC_Init(&NVIC_InitStructure);
	
	// 控制使能
	TIM_Cmd(TIM2, ENABLE);
}

/*
void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/