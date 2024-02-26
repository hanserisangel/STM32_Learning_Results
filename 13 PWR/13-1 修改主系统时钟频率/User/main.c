#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "SYSCLK:");
	
	OLED_ShowNum(1, 8, SystemCoreClock, 8); // SystemCoreClock Ϊ��Ƶ����
	while(1)
	{
		OLED_ShowString(2, 1, "Running");
		Delay_ms(500); // �����ʱ����֧�ֵ��� 72MHz ��Ƶ
		OLED_ShowString(2, 1, "       ");
		Delay_ms(500);
	}
}
