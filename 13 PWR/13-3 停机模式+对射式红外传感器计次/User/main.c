#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "Count:");
	
	CountSensor_Init();
	
	// 开启 APB1 上的 PWR 的 RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	while(1)
	{
		OLED_ShowNum(1, 7, CountSensor_Get(), 5);
		
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		// 进入停机模式: 开启 PVD, WFI 唤醒
		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
		SystemInit(); // 8MHz → 72MHz
	}
}
