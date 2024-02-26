#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

uint16_t ArrayWrite[] = {0x1234, 0x5678};
uint16_t ArrayRead[2];
uint8_t KeyNum;

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "W:");
	OLED_ShowString(2, 1, "R:");
	
	Key_Init();
	
	// ���� APB1 �ϵ� PWR �� BKP �� RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE); // BKP ʹ��
	
	while(1)
	{
		KeyNum = Key_GetNum();
		
		if(KeyNum == 1)
		{
			ArrayWrite[0] ++;
			ArrayWrite[1] ++;
			BKP_WriteBackupRegister(BKP_DR1, ArrayWrite[0]); // д�� DR1 ���ݼĴ���
			BKP_WriteBackupRegister(BKP_DR2, ArrayWrite[1]); // д�� DR2 ���ݼĴ���
			
			OLED_ShowHexNum(1, 3, ArrayWrite[0], 4);
			OLED_ShowHexNum(1, 8, ArrayWrite[1], 4);
		}
		
		ArrayRead[0] = BKP_ReadBackupRegister(BKP_DR1); // �� DR1 ���ݼĴ���
		ArrayRead[1] = BKP_ReadBackupRegister(BKP_DR2); // �� DR1 ���ݼĴ���
		
		OLED_ShowHexNum(2, 3, ArrayRead[0], 4);
		OLED_ShowHexNum(2, 8, ArrayRead[1], 4);
	}
}
