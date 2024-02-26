#include "stm32f10x.h"                  // Device header

void MySPI_W_CS(uint8_t BitValue) // CS ���Ų���
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue); // PA4
}

void MySPI_Init()
{
	// ���� APB2 �ϵ� GPIOA �� SPI1 �� RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	// GPIO ��ʼ�� PA4/PA5/PA7 �� PA6
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ; // �������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP ; // �����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU ; // ��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// SPI ��ʼ��
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128; // ʱ�ӷ�Ƶ
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low; // SPI ģʽ 0
	SPI_InitStructure.SPI_CRCPolynomial=7; // �޹�
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b; // 8 λ����֡
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex; // ȫ˫��
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB; // ��λ����
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master; // ����
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft; // �޹�
	SPI_Init(SPI1, &SPI_InitStructure);
	
	// ʹ�� SPI1
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
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); // 1 ��ʾ TDR Ϊ��
	SPI_I2S_SendData(SPI1, SendByte);
	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); // 1 ��ʾ RDR ���
	Byte = SPI_I2S_ReceiveData(SPI1);
	return Byte;
}
