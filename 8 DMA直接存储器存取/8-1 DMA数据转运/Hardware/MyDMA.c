#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_size;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t size) // A �� B
{
	// ���� AHB �ϵ� DMA �� RCC
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	MyDMA_size = size;
	
	// ��ʼ�� DMA1 �� ͨ�� 1
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize=size; // ����Ĵ���
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC; // ���䷽��: ������ΪԴ
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable; // ѡ�񴥷���ʽ: �������
	DMA_InitStructure.DMA_MemoryBaseAddr=AddrB; // �洢����ʼ��ַ
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte; // �洢�����ݿ��: 1 ���ֽ�
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;  // �洢����ַ����
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal; // ����ģʽ: ���Զ���װ
	DMA_InitStructure.DMA_PeripheralBaseAddr=AddrA; // ������ʼ��ַ
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte; // �������ݿ��: 1 ���ֽ�
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Enable;  // �����ַ����
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium; // ���ȼ�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	// ʹ�� DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

void MyDMA_Transfer()
{
	DMA_Cmd(DMA1_Channel1, DISABLE); // ʧ�ܺ����д���������
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_size); // ����д�봫�������
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET); // 1 ��ʾת�����
	DMA_ClearFlag(DMA1_FLAG_TC1); // ������
}
