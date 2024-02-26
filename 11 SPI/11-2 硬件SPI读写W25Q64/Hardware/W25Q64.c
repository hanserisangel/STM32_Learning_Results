#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

void W25Q64_Init()
{
	MySPI_Init();
}

void W25Q64_ReadID(uint16_t *ID)
{
	uint8_t IDH, IDL;
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_JEDEC_ID); // ID 指令码
	MySPI_SwapByte(W25Q64_DUMMY_BYTE); // 空: 0xFF

	IDH = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	IDL = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	MySPI_Stop();
	
	*ID = (IDH << 8) | IDL;
}

void W25Q64_WriteEnable()
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE); // 写使能
	MySPI_Stop();
}

void W25Q64_WaitBusy()
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	while((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 1); // 1 表示忙
	MySPI_Stop();
}

void W25Q64_WriteData(uint32_t Address, uint8_t *SendArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable(); // 写使能
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address >> 16 % 256);
	MySPI_SwapByte(Address >> 8 % 256);
	MySPI_SwapByte(Address % 256);
	for(i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(SendArray[i]);
	}
	MySPI_Stop();
	
	W25Q64_WaitBusy(); // 等待忙
}

void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable(); // 写使能
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	MySPI_SwapByte(Address >> 16 % 256);
	MySPI_SwapByte(Address >> 8 % 256);
	MySPI_SwapByte(Address % 256);
	MySPI_Stop();
	
	W25Q64_WaitBusy(); // 等待忙
}

void W25Q64_ReadData(uint32_t Address, uint8_t *SendArray, uint32_t Count)
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address >> 16 % 256);
	MySPI_SwapByte(Address >> 8 % 256);
	MySPI_SwapByte(Address % 256);
	for(i = 0; i < Count; i ++)
	{
		SendArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
}
