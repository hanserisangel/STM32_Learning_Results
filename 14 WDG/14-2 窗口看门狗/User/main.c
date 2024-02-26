#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "WWDG TEST");
	
	Key_Init();
	
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET) // 看门狗复位
	{
		OLED_ShowString(2, 1, "WWDG RST");
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
	
	// 开启 APB1 上的 WWDG 的 RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	// 时间: 50ms = 1/36M * 4096 * PSC * (ARR + 1); 
	// 30ms = 1/36M * 4096 * PSC * (ARR - window)
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(0x40 | 21); // 窗口值
	WWDG_Enable(0x40 | 54); // 使能: 重装计数器
	
	while(1)
	{
		Key_GetNum();
		
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(20);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(20);
		
		WWDG_SetCounter(0x40 | 54); // 喂狗
	}
}
