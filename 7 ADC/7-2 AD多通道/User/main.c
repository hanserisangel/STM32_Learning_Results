#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t AD0, AD1, AD2, AD3;

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "AD0:");
	OLED_ShowString(2, 1, "AD1:");
	OLED_ShowString(3, 1, "AD2:");
	OLED_ShowString(4, 1, "AD3:");
	
	AD_Init();
	
	while(1)
	{
		AD0 = ADC_GetValue(ADC_Channel_0); // 通道 0
		AD1 = ADC_GetValue(ADC_Channel_1); // 通道 1
		AD2 = ADC_GetValue(ADC_Channel_2); // 通道 2
		AD3 = ADC_GetValue(ADC_Channel_3); // 通道 3
		
		OLED_ShowNum(1, 5, AD0, 4);
		OLED_ShowNum(2, 5, AD1, 4);
		OLED_ShowNum(3, 5, AD2, 4);
		OLED_ShowNum(4, 5, AD3, 4);
		
		Delay_ms(100);
	}
}
