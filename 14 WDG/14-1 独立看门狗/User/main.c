#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "IWDG TEST");
	
	Key_Init();
	
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET) // 看门狗复位
	{
		OLED_ShowString(2, 1, "IWDG RST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "        ");
		Delay_ms(100);
		
		RCC_ClearFlag(); // 清零
	}
	else // 按键复位
	{
		OLED_ShowString(3, 1, "RST");
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}
	
	// 写使能
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	// 超时时间: 1000ms = 0.025ms * PSC * (ARR + 1)
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(2500 - 1);
	
	IWDG_ReloadCounter(); // 装载: 喂狗
	IWDG_Enable(); // 使能
	
	while(1)
	{
		Key_GetNum();
		IWDG_ReloadCounter(); // 喂狗
		
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(200);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(600);
	}
}
