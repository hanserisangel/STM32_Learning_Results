#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "OLED");
	
	Serial_Init();
	
	Serial_SendByte(0x41);
	Serial_SendByte('A');
	
	uint8_t MyArray[] = {0x41, 0x42, 0x43, 0x44};
	Serial_SendArray(MyArray, 4);
	
	Serial_SendString("hanser");
	
	Serial_SendNumber(123, 3);
	
	printf("Num = %d\n", 123); // 串口打印
	
	Serial_Printf("Num = %d\n", 123); // 串口打印
	
	Serial_Printf("你干嘛", 123); // 串口打印
	
	while(1)
	{
		
	}
}
