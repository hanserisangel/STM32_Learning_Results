#include "stm32f10x.h"                  // Device header

void LED_Init()
{
	// 开启 RCC 外设时钟控制
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// GPIO 初始化 A1 和 A2
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 初始熄灭 LED
	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

// 点亮 PA1
void LED1_ON()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

// 熄灭 PA1
void LED1_OFF()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

// 翻转 PA1
void LED1_Turn()
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	}
}

// 点亮 PA2
void LED2_ON()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

// 熄灭 PA2
void LED2_OFF()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

// 翻转 PA2
void LED2_Turn()
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}
}