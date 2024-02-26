#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"

uint8_t KeyNumber;

int main()
{
	LED_Init();
	Key_Init();
	
	while (1)
	{
		KeyNumber = Key_GetNum();
		
		if(KeyNumber == 1)
		{
			LED1_Turn(); // µçÆ½·­×ª
		}
		
		if(KeyNumber == 1)
		{
			LED2_Turn();
		}
	}
}
