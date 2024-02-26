#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

void Serial_Init(void)
{
	// ���� APB2 �ϵ� USART1 �� RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// ��ʼ�� TX �� PA9
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; // �����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// ��ʼ�� UART1
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600; // ������
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // Ӳ��������
	USART_InitStructure.USART_Mode=USART_Mode_Tx; // ����ģʽ
	USART_InitStructure.USART_Parity=USART_Parity_No; // У��λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1; // 1 λֹͣλ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b; // 8 λ����
	USART_Init(USART1, &USART_InitStructure);
	
	// ʹ�� USART1
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 1 ��ʾ�������
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *string)
{
	uint16_t i;
	for(i = 0; string[i] != '\0'; i ++) // .... \0
	{
		Serial_SendByte(string[i]);
	}
}

uint32_t power(uint32_t number, uint32_t x)
{
	uint32_t i;
	uint32_t res = 1;
	for(i = 0; i < x; i ++)
	{
		res *= number;
	}
	return res;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for(i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / power(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...) // ��ʽ���ַ���, �ɱ����
{
	char string[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(string, format, arg);
	va_end(arg);
	Serial_SendString(string);
}
