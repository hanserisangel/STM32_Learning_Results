#include "stm32f10x.h"

void Timer_Init()
{
	// 开启 APB1 总线 TIM2 和 APB2 总线 GPIOA 的 RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// GPIO 初始化 PA0
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 时钟源选择: 外部时钟模式 2
TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);
	
	// 设置时基单元: 手动
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 无关: 消抖滤波, f 采样速率, N 采样数
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1; // ARR 自动重装值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; // PSC 预分频值
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

uint16_t Timer_GetCounter()
{
	return TIM_GetCounter(TIM2);
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