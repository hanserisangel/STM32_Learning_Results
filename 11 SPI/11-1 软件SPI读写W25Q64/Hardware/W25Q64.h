#ifndef __W25Q64_H
#define __W25Q64_H

void W25Q64_Init();
void W25Q64_ReadID(uint16_t *ID);
void W25Q64_WriteData(uint32_t Address, uint8_t *SendArray, uint16_t Count);
void W25Q64_SectorErase(uint32_t Address);
void W25Q64_ReadData(uint32_t Address, uint8_t *SendArray, uint32_t Count);

#endif