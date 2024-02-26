#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "Freq:00000Hz");
	OLED_ShowString(2, 1, "Duty:000%");
	
	PWM_Init();
	IC_Init();
	
	// 可随意修改 PWM 波的参数
	PWM_SetPrescaler(720 - 1); // Freq = 72M / (PSC + 1) / 100
	PWM_SetCompare1(50); // Duty = CCR / 100
	
	while(1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
		OLED_ShowNum(2, 6, IC_GetDuty(), 3);
	}
}
