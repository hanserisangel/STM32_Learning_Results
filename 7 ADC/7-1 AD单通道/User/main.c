#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue;
float v;

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "AD:");
	OLED_ShowString(2, 1, "V:0.00v");
	
	AD_Init();
	
	while(1)
	{
		ADValue = ADC_GetValue();
		v = (float) ADValue / 4095 * 3.3; // µçÑ¹×ª»»
		
		OLED_ShowNum(1, 4, ADValue, 4);
		OLED_ShowNum(2, 3, v, 1);
		OLED_ShowNum(2, 5, (uint16_t)(v * 100) % 100, 2);
		
		Delay_ms(100);
	}
}
