#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[] = {2024, 2, 21, 14, 20, 0}; // ʱ������

void MyRTC_SetTime();

void MyRTC_Init()
{
	// ���� APB1 �ϵ� PWR �� BKP �� RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) // Ϊ��ֻ���� 1 ����ʼʱ��
	{
		// ʱ��Դ
		RCC_LSEConfig(RCC_LSE_ON); // ���� LSE
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET); // �ȴ� LSE �������
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // ѡ�� LSE ��Ϊʱ��Դ
		RCC_RTCCLKCmd(ENABLE); // ʹ��ʱ��Դ
		
		// �ȴ�
		RTC_WaitForSynchro(); // �ȴ�ͬ��
		RTC_WaitForLastTask(); // �ȴ���һ��д���
		
		// ��ʼ�� RTC ��Ԫ
		RTC_SetPrescaler(32768 - 1); // Ԥ��Ƶֵ
		RTC_WaitForLastTask(); // �ȴ�д���
		
		MyRTC_SetTime(); // ������ʼʱ��
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		// �ȴ�
		RTC_WaitForSynchro(); // �ȴ�ͬ��
		RTC_WaitForLastTask(); // �ȴ���һ��д���
	}
	
}

void MyRTC_SetTime() // ����ʱ��
{
	time_t time_cnt;
	struct tm time_date;
	
	time_date.tm_year = MyRTC_Time[0] - 1900;
	time_date.tm_mon = MyRTC_Time[1] - 1;
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min = MyRTC_Time[4];
	time_date.tm_sec = MyRTC_Time[5];
	
	time_cnt = mktime(&time_date) - 8 * 60 * 60; // ����������� - ʱ��ƫ��
	
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask(); // �ȴ�д���
}

void MyRTC_ReadTime()
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60; // ������� + ʱ��ƫ��
	time_date = *localtime(&time_cnt); // ������� �� ʱ��
	
	MyRTC_Time[0] = time_date.tm_year + 1900;
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}
