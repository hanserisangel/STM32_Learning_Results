#include "stm32f10x.h"                  // Device header

void LightSensor_Init()
{
	// 开启 RCC 外设时钟控制
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// GPIO 初始化 B13
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t LightSensor_Get()
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
}
