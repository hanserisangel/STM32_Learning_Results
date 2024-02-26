#include "stm32f10x.h"                  // Device header

void PWM_Init()
{
	// 开启 APB1 上 TIM2 和 APB2 上 GPIOA 的 RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 初始化 PA0
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP ; // 复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 时钟源选择
	TIM_InternalClockConfig(TIM2); // 内部时钟
	
	// 时基单元: 1kHz
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 无关
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // ARR 
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 高级定时器
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	// 输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); // 因为结构体参数不完全设置, 所以先默认
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1; // 输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; // 电平不翻转
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable; // 输出使能
	TIM_OCInitStructure.TIM_Pulse=0; // CCR 寄存器值
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	// 控制使能
	TIM_Cmd(TIM2, ENABLE);
}

// 占空比, Compare = [0, 100], 因为 ARR = 100
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}

// 设置频率
void PWM_SetPrescaler(uint16_t Prescale)
{
	TIM_PrescalerConfig(TIM2, Prescale, TIM_PSCReloadMode_Update);
}
