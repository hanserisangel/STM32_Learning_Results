#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[] = {2024, 2, 21, 14, 20, 0}; // 时间数组

void MyRTC_SetTime();

void MyRTC_Init()
{
	// 开启 APB1 上的 PWR 和 BKP 的 RCC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) // 为了只设置 1 次起始时间
	{
		// 时钟源
		RCC_LSEConfig(RCC_LSE_ON); // 启动 LSE
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET); // 等待 LSE 启动完成
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // 选择 LSE 作为时钟源
		RCC_RTCCLKCmd(ENABLE); // 使能时钟源
		
		// 等待
		RTC_WaitForSynchro(); // 等待同步
		RTC_WaitForLastTask(); // 等待上一次写完成
		
		// 初始化 RTC 单元
		RTC_SetPrescaler(32768 - 1); // 预分频值
		RTC_WaitForLastTask(); // 等待写完成
		
		MyRTC_SetTime(); // 设置起始时间
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		// 等待
		RTC_WaitForSynchro(); // 等待同步
		RTC_WaitForLastTask(); // 等待上一次写完成
	}
	
}

void MyRTC_SetTime() // 设置时间
{
	time_t time_cnt;
	struct tm time_date;
	
	time_date.tm_year = MyRTC_Time[0] - 1900;
	time_date.tm_mon = MyRTC_Time[1] - 1;
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min = MyRTC_Time[4];
	time_date.tm_sec = MyRTC_Time[5];
	
	time_cnt = mktime(&time_date) - 8 * 60 * 60; // 生成秒计数器 - 时区偏移
	
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask(); // 等待写完成
}

void MyRTC_ReadTime()
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60; // 秒计数器 + 时区偏移
	time_date = *localtime(&time_cnt); // 秒计数器 → 时间
	
	MyRTC_Time[0] = time_date.tm_year + 1900;
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}
