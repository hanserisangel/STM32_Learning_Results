#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

uint8_t RxData;

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "RxData:");
	
	Serial_Init();
	
	while(1)
	{
		if(Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData();
			
			Serial_SendByte(RxData); // »Ø´«
			
			OLED_ShowNum(1, 8, RxData, 2);
		}
	}
}
