#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "IWDG TEST");
	
	Key_Init();
	
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET) // ���Ź���λ
	{
		OLED_ShowString(2, 1, "IWDG RST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "        ");
		Delay_ms(100);
		
		RCC_ClearFlag(); // ����
	}
	else // ������λ
	{
		OLED_ShowString(3, 1, "RST");
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}
	
	// дʹ��
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	// ��ʱʱ��: 1000ms = 0.025ms * PSC * (ARR + 1)
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(2500 - 1);
	
	IWDG_ReloadCounter(); // װ��: ι��
	IWDG_Enable(); // ʹ��
	
	while(1)
	{
		Key_GetNum();
		IWDG_ReloadCounter(); // ι��
		
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(200);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(600);
	}
}
