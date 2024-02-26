#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"

uint16_t ID;

uint8_t ArrayWrite[] = {0x01, 0x02, 0x03, 0x04};
uint8_t ArrayRead[4];

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "ID:");
	OLED_ShowString(2, 1, "W:");
	OLED_ShowString(3, 1, "R:");
	
	W25Q64_Init();
	
	W25Q64_ReadID(&ID);
	OLED_ShowHexNum(1, 4, ID, 4);
	
	W25Q64_SectorErase(0x000000); // Ð´Èë Flash Ç°, ÏÈ²Á³ý
	W25Q64_WriteData(0x000000, ArrayWrite, 4);
	W25Q64_ReadData(0x000000, ArrayRead, 4);
	
	OLED_ShowHexNum(2, 3, ArrayWrite[0], 2);
	OLED_ShowHexNum(2, 6, ArrayWrite[1], 2);
	OLED_ShowHexNum(2, 9, ArrayWrite[2], 2);
	OLED_ShowHexNum(2, 12, ArrayWrite[3], 2);
	
	OLED_ShowHexNum(3, 3, ArrayRead[0], 2);
	OLED_ShowHexNum(3, 6, ArrayRead[1], 2);
	OLED_ShowHexNum(3, 9, ArrayRead[2], 2);
	OLED_ShowHexNum(3, 12, ArrayRead[3], 2);
	
	while(1)
	{
		
	}
}
