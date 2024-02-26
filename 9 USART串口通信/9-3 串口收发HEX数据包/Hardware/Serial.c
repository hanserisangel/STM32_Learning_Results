#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_TxPacket[4];
uint8_t Serial_RxPacket[4];
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
	// 开启 APB2 上的 USART1 的 RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 初始化 TX 的 PA9 和 RX 的 PA10
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; // 复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 初始化 UART1
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600; // 波特率
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // 硬件流控制
	USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx; // 发送模式和接收模式
	USART_InitStructure.USART_Parity=USART_Parity_No; // 校验位
	USART_InitStructure.USART_StopBits=USART_StopBits_1; // 1 位停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b; // 8 位数据
	USART_Init(USART1, &USART_InitStructure);
	
	// 中断输出控制: 打开接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	// NVIC 初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 优先级分组: 2 + 2
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn; // USART1 通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; // 抢占
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; // 响应
	NVIC_Init(&NVIC_InitStructure);
	
	// 使能 USART1
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 1 表示发送完成
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

void Serial_Printf(char *format, ...) // 格式化字符串, 可变参数
{
	char string[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(string, format, arg);
	va_end(arg);
	Serial_SendString(string);
}

void Serial_SendPacket()
{
	Serial_SendByte(0xff); // 包头
	
	Serial_SendArray(Serial_TxPacket, 4); // 数组
	
	Serial_SendByte(0xfe); // 包尾
}

uint8_t Serial_GetRxFlag()
{
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}


void USART1_IRQHandler() // 中断函数
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET) // 1 表示接收完成
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		
		if(RxState == 0)
		{
			if(RxData == 0xff)
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if(RxState == 1)
		{
			Serial_RxPacket[pRxPacket] = RxData;
			pRxPacket ++;
			if(pRxPacket >= 4) RxState = 2;
		}
		else if(RxState == 2)
		{
			if(RxData == 0xfe)
			{
				RxState = 0;
				Serial_RxFlag = 1;
			}
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清零
	}
}
