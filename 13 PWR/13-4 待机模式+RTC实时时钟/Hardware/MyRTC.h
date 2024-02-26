#ifndef __MYRTC_H
#define __MYRTC_H

extern uint16_t MyRTC_Time[]; // 时间数组

void MyRTC_Init();
void MyRTC_SetTime();
void MyRTC_ReadTime();

#endif