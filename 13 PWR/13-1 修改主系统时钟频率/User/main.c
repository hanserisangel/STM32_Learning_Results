#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "SYSCLK:");
	
	OLED_ShowNum(1, 8, SystemCoreClock, 8); // SystemCoreClock 为主频变量
	while(1)
	{
		OLED_ShowString(2, 1, "Running");
		Delay_ms(500); // 这个延时函数支持的是 72MHz 主频
		OLED_ShowString(2, 1, "       ");
		Delay_ms(500);
	}
}
