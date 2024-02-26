#include "stm32f10x.h"                  // Device header

uint32_t MyFlash_ReadWord(uint32_t Address) // �� 1 ����
{
	return *((__IO uint32_t *)(Address));
}

uint16_t MyFlash_ReadHalfWord(uint32_t Address) // �� 0.5 ����
{
	return *((__IO uint16_t *)(Address));
}

uint8_t MyFlash_ReadByte(uint32_t Address) // �� 1 ���ֽ�
{
	return *((__IO uint8_t *)(Address));
}

void MyFlash_EraseAllPages() // ȫ����
{
	FLASH_Unlock();
	FLASH_EraseAllPages();
	FLASH_Lock();
}

void MyFlash_ErasePage(uint32_t Address) // ҳ����
{
	FLASH_Unlock();
	FLASH_ErasePage(Address);
	FLASH_Lock();
}

void MyFlash_WriteWord(uint32_t Address, uint32_t Data) // д 1 ����
{
	FLASH_Unlock();
	FLASH_ProgramWord(Address, Data);
	FLASH_Lock();
}

void MyFlash_WriteHalfWord(uint32_t Address, uint16_t Data) // д 0.5 ����
{
	FLASH_Unlock();
	FLASH_ProgramHalfWord(Address, Data);
	FLASH_Lock();
}
