#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

int16_t Speed;

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "Speed:");
	
	Timer_Init();
	Encoder_Init();
	
	while(1)
	{
		OLED_ShowSignedNum(1, 7, Speed, 3);
		
	}
}

// 中断实现: 1s 闸门时间, 测频法求速度
void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Speed = Encoder_Get();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
