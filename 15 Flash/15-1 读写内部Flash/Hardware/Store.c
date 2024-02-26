#include "stm32f10x.h"                  // Device header
#include "MyFlash.h"

#define STORE_START_ADDRESS		0x0800FC00
#define STORE_COUNT				512

uint16_t Store_Data[STORE_COUNT];

void Store_Init()
{
	if (MyFlash_ReadHalfWord(STORE_START_ADDRESS) != 0xA5A5) // �� 1 ���� Flash
	{
		MyFlash_ErasePage(STORE_START_ADDRESS);
		MyFlash_WriteHalfWord(STORE_START_ADDRESS, 0xA5A5); // ������ֽ��Ǳ�־λ
		for (uint16_t i = 1; i < STORE_COUNT; i ++)
		{
			MyFlash_WriteHalfWord(STORE_START_ADDRESS + i * 2, 0x0000); // ����
		}
	}
	
	// ��־λ������[0] �±�, ��ΪС�˴洢
	for (uint16_t i = 0; i < STORE_COUNT; i ++) // Flash �� SRAM
	{
		Store_Data[i] = MyFlash_ReadHalfWord(STORE_START_ADDRESS + i * 2);
	}
}

void Store_Save() // SRAM �� Flash
{
	MyFlash_ErasePage(STORE_START_ADDRESS);
	for (uint16_t i = 0; i < STORE_COUNT; i ++)
	{
		MyFlash_WriteHalfWord(STORE_START_ADDRESS + i * 2, Store_Data[i]);
	}
}

void Store_Clear() // ����
{
	for (uint16_t i = 1; i < STORE_COUNT; i ++)
	{
		Store_Data[i] = 0x0000;
	}
	Store_Save();
}
