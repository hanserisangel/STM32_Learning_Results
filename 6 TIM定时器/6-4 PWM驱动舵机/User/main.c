#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "Key.h"

uint8_t KeyNum;
float Angle;

int main()
{
	OLED_Init();
	OLED_ShowString(1, 1, "Angle:");
	
	Servo_Init();
	//Key_Init(); // 不知原因, 不注释掉 OLED 不显示
	
	while(1)
	{
		KeyNum = Key_GetNum();
		
		if(KeyNum == 1)
		{
			Angle += 30;
			if(Angle > 180) Angle = 0;
		}
		
		Servo_SetAngle(Angle);
		OLED_ShowNum(1, 7, Angle, 3);
	}
}
