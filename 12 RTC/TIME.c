#include <stdio.h>
#include <time.h>

time_t time_cnt;
struct tm *time_date;
char *time_str;

int main()
{
	// 获取系统时钟 
	time(&time_cnt);
	printf("%d\n", time_cnt); // 秒计数器
	
	// GMT 时间(时间 →秒计数器)
	time_date = gmtime(&time_cnt);
	printf("%d-%d-%d %d:%d:%d\n", time_date->tm_year + 1900, time_date->tm_mon + 1,
			time_date->tm_mday, time_date->tm_hour, time_date->tm_min, time_date->tm_sec);
	
	// 当地时间(时间 →秒计数器)
	time_date = localtime(&time_cnt);
	printf("%d-%d-%d %d:%d:%d\n", time_date->tm_year + 1900, time_date->tm_mon + 1,
			time_date->tm_mday, time_date->tm_hour, time_date->tm_min, time_date->tm_sec);
	
	// 时间 →秒计数器
	time_cnt = mktime(time_date);
	printf("%d\n", time_cnt); // 秒计数器
	 
	return 0;
} 


