#include "stm32f10x.h"                  // Device header

void Encoder_Init()
{
	// 开启 APB1 上 TIM3 和 APB2 上 GPIOA 的 RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 初始化 PA6 和 PA7
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU ; // 上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 时基单元: 编码器接口托管
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 无关
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 无用: 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // ARR, 满量程计数
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; // PSC 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 高级定时器
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	// 输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; // 选择通道 1
	TIM_ICInitStructure.TIM_ICFilter=0xF; // 滤波
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2; // 选择通道 2
	TIM_ICInitStructure.TIM_ICFilter=0xF; // 滤波
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	// 配置编码器接口: 工作模式, A 相和 B 相的极性选择
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	// 控制使能
	TIM_Cmd(TIM3, ENABLE);
}

// 1s 闸门时间的 CNT 值, 配合测周法求速度
int16_t Encoder_Get()
{
	int16_t temp;
	temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0); // CNT 清零
	return temp;
}
