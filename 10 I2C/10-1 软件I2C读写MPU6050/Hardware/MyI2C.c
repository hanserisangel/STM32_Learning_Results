#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void MyI2C_W_SCL(uint8_t Bitvalue) // SCL 引脚电平操作
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)Bitvalue); // PB10
	Delay_us(10);
}

void MyI2C_W_SDA(uint8_t Bitvalue) // SDA 引脚电平写操作
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)Bitvalue); // PB11
	Delay_us(10);
}

uint8_t MyI2C_R_SDA() // SDA 引脚电平读操作
{
	uint8_t Bitvalue;
	Bitvalue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11); // PB11
	Delay_us(10);
	return Bitvalue;
}

void MyI2C_Init(void)
{
	// 开启 APB2 上的 GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	// GPIO 初始化 PB10 和 PB11
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD ; // 开漏输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	// I2C 时序的最开始
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

void MyI2C_Start()
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	
	MyI2C_W_SDA(0); // SCL = 1, SDA 1 → 0
	MyI2C_W_SCL(0);
}

void MyI2C_Stop()
{
	MyI2C_W_SDA(0);
	
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1); // SCL = 1, SDA 0 → 1
}

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i)); // 高位先行
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2C_ReceiveByte()
{
	uint8_t i;
	uint8_t Byte = 0x00;
	
	MyI2C_W_SDA(1); // 释放 SDA
	
	for(i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA()) Byte |= (0x80 >> i); // 高位先行
		MyI2C_W_SCL(0);
	}
	
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck()
{
	uint8_t AckBit;
	
	MyI2C_W_SDA(1); // 释放 SDA
	
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	
	return AckBit;
}
