#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main()
{
	// ���� RCC ����ʱ�ӿ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// GPIO ��ʼ�� B12
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	while (1)
	{
		// ��һ�� ������ ���з���
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		Delay_ms(500);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		Delay_ms(500);
		
		// �ڶ��� ������ ���з���
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
		Delay_ms(500);
	}
}
