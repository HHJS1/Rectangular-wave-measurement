//
// Created by 30427 on 2025/12/21.
//

#ifndef RECTANGULAR_WAVE_TIMER_CAPTURE_H
#define RECTANGULAR_WAVE_TIMER_CAPTURE_H

#include "stdint.h"
#include "main.h"
#include "stdbool.h"

extern uint8_t dan;
// 定时器配置结构
typedef struct {
    uint32_t timer_freq;      // 定时器时钟频率(Hz)
    uint32_t prescaler;       // 预分频值
    uint32_t period;          // 定时器周期值
    uint8_t channel_rising;   // 上升沿捕获通道
    uint8_t channel_falling;  // 下降沿捕获通道
} TimerCapture_Config_t;

// 捕获数据
typedef struct {
    uint32_t period_ticks;    // 周期计数值
    uint32_t high_ticks;      // 高电平计数值
    uint32_t capture_time;    // 最近捕获时间戳
    bool new_data;            // 新数据标志
} CaptureData_t;

// 函数声明
void TimerCapture_Init(TIM_HandleTypeDef *htim);
void TimerCapture_Start(void);
void TimerCapture_Stop(void);
uint32_t TimerCapture_GetPeriodTicks(void);
uint32_t TimerCapture_GetHighTicks(void);
bool TimerCapture_IsDataReady(void);
void TimerCapture_ClearDataFlag(void);
float TimerCapture_CalculateFrequency(void);
float TimerCapture_CalculateDutyCycle(void);

// 中断回调函数（需在stm32fxx_it.c中调用）
void TimerCapture_RisingCallback(TIM_HandleTypeDef *htim);
void TimerCapture_FallingCallback(TIM_HandleTypeDef *htim);
float get_capture_Fre(void);







#endif //RECTANGULAR_WAVE_TIMER_CAPTURE_H
