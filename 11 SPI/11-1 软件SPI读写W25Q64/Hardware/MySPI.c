#include "stm32f10x.h"                  // Device header

void MySPI_W_SCL(uint8_t BitValue) // SCL ���Ų���
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue); // PA5
}

void MySPI_W_CS(uint8_t BitValue) // CS ���Ų���
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue); // PA4
}

void MySPI_W_MOSI(uint8_t BitValue) // MOSI ���Ų���
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue); // PA7
}

uint8_t MySPI_R_MISO() // MISO ���Ų���
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6); // PA6
}

void MySPI_Init()
{
	// ���� APB2 �ϵ� GPIOA �� RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// GPIO ��ʼ�� PA4/PA5/PA7 �� PA6
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ; // �������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU ; // ��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	MySPI_W_CS(1);
	MySPI_W_SCL(0);
}

void MySPI_Start()
{
	MySPI_W_CS(0);
}

void MySPI_Stop()
{
	MySPI_W_CS(1);
}

uint8_t MySPI_SwapByte(uint8_t SendByte)
{
	uint8_t ReadByte = 0x00;
	uint8_t i;
	
	for(i = 0; i < 8; i ++)
	{
		MySPI_W_MOSI(SendByte & (0x80 >> i)); // SCL = 0, ������
		MySPI_W_SCL(1);
		if(MySPI_R_MISO()) ReadByte |= (0x80 >> i); // SCL = 1, ������
		MySPI_W_SCL(0);
	}
	
	return ReadByte;
}
