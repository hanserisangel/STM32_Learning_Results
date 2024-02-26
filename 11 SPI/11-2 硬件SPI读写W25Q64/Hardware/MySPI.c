#include "stm32f10x.h"                  // Device header

void MySPI_W_CS(uint8_t BitValue) // CS 引脚操作
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue); // PA4
}

void MySPI_Init()
{
	// 开启 APB2 上的 GPIOA 和 SPI1 的 RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	// GPIO 初始化 PA4/PA5/PA7 和 PA6
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ; // 推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP ; // 复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU ; // 上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// SPI 初始化
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128; // 时钟分频
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low; // SPI 模式 0
	SPI_InitStructure.SPI_CRCPolynomial=7; // 无关
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b; // 8 位数据帧
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex; // 全双工
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB; // 高位先行
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master; // 主机
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft; // 无关
	SPI_Init(SPI1, &SPI_InitStructure);
	
	// 使能 SPI1
	SPI_Cmd(SPI1, ENABLE);
	
	MySPI_W_CS(1);
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
	uint8_t Byte;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); // 1 表示 TDR 为空
	SPI_I2S_SendData(SPI1, SendByte);
	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); // 1 表示 RDR 完成
	Byte = SPI_I2S_ReceiveData(SPI1);
	return Byte;
}
