#include "stm32f10x.h"                  // Device header

uint32_t MyFlash_ReadWord(uint32_t Address) // ¶Á 1 ¸ö×Ö
{
	return *((__IO uint32_t *)(Address));
}

uint16_t MyFlash_ReadHalfWord(uint32_t Address) // ¶Á 0.5 ¸ö×Ö
{
	return *((__IO uint16_t *)(Address));
}

uint8_t MyFlash_ReadByte(uint32_t Address) // ¶Á 1 ¸ö×Ö½Ú
{
	return *((__IO uint8_t *)(Address));
}

void MyFlash_EraseAllPages() // È«²Á³ý
{
	FLASH_Unlock();
	FLASH_EraseAllPages();
	FLASH_Lock();
}

void MyFlash_ErasePage(uint32_t Address) // Ò³²Á³ý
{
	FLASH_Unlock();
	FLASH_ErasePage(Address);
	FLASH_Lock();
}

void MyFlash_WriteWord(uint32_t Address, uint32_t Data) // Ð´ 1 ¸ö×Ö
{
	FLASH_Unlock();
	FLASH_ProgramWord(Address, Data);
	FLASH_Lock();
}

void MyFlash_WriteHalfWord(uint32_t Address, uint16_t Data) // Ð´ 0.5 ¸ö×Ö
{
	FLASH_Unlock();
	FLASH_ProgramHalfWord(Address, Data);
	FLASH_Lock();
}
