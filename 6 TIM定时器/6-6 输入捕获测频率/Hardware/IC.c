#include "stm32f10x.h"                  // Device header

void IC_Init()
{
	// 开启 APB1 上 TIM3 和 APB2 上 GPIOA 的 RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 初始化 PA6
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU ; // 上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 时钟源选择
	TIM_InternalClockConfig(TIM3); // 内部时钟
	
	// 时基单元: 1MHz
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 无关
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // ARR, 满量程计数
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // PSC 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 高级定时器
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	// 输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; // 选择通道 1
	TIM_ICInitStructure.TIM_ICFilter=0xF; // 滤波
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; // 上升沿触发
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1; // 分频
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI; // 数据选择, 直连
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	//主从触发模式
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1); // 从模式触发源
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); // 从模式选择
	
	// 控制使能
	TIM_Cmd(TIM3, ENABLE);
}

// 返回频率, (1M / CCR) Hz
uint32_t IC_GetFreq()
{
	return 1000000 / TIM_GetCapture1(TIM3);
}
