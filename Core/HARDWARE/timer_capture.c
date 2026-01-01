//
// Created by 30427 on 2025/12/21.
//

#include "timer_capture.h"


// 全局变量定义
static TIM_HandleTypeDef *g_htim = NULL;
static CaptureData_t g_capture_data = {0};
static TimerCapture_Config_t TimerCapture_Config = {0};
static volatile uint32_t g_last_rising = 0;
static volatile uint32_t g_rising_value = 0;
static volatile uint32_t g_falling_value = 0;

//uint8_t dan=0;


void TimerCapture_Init(TIM_HandleTypeDef *htim) {
    g_htim = htim;

    TimerCapture_Config.prescaler = htim->Init.Prescaler;
    // 初始化捕获数据结构
    g_capture_data.period_ticks = 0;
    g_capture_data.high_ticks = 0;
    g_capture_data.capture_time = 0;
    g_capture_data.new_data = false;
}

void TimerCapture_Start(void) {
    if (g_htim == NULL) return;

    // 启动输入捕获通道（双沿捕获）
    HAL_TIM_IC_Start_IT(g_htim, TIM_CHANNEL_3);
    HAL_TIM_IC_Start_IT(g_htim, TIM_CHANNEL_4);
}

void TimerCapture_Stop(void) {
    if (g_htim == NULL) return;

    HAL_TIM_IC_Stop_IT(g_htim, TIM_CHANNEL_3);
    HAL_TIM_IC_Stop_IT(g_htim, TIM_CHANNEL_4);
}

// 上升沿捕获回调（周期测量）
void TimerCapture_RisingCallback(TIM_HandleTypeDef *htim) {
    g_rising_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);

    if (g_last_rising != 0) {
        uint32_t period = g_rising_value - g_last_rising;

        // 处理定时器溢出
        if (g_rising_value < g_last_rising) {
            period = g_rising_value + (0xFFFFFFFF - g_last_rising);
        }

        g_capture_data.period_ticks = period;
        g_capture_data.capture_time = HAL_GetTick();
    }

    g_last_rising = g_rising_value;
}

// 下降沿捕获回调（高电平时间测量）
void TimerCapture_FallingCallback(TIM_HandleTypeDef *htim) {
    g_falling_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);

    if (g_rising_value != 0) {
        uint32_t high_time = g_falling_value - g_rising_value;

        // 处理定时器溢出
        if (g_falling_value < g_rising_value) {
            high_time = g_falling_value + (0xFFFFFFFF - g_rising_value);
        }

        g_capture_data.high_ticks = high_time;
        g_capture_data.new_data = true;
    }
}

uint32_t TimerCapture_GetPeriodTicks(void) {
    return g_capture_data.period_ticks;
}

uint32_t TimerCapture_GetHighTicks(void) {
    return g_capture_data.high_ticks;
}

bool TimerCapture_IsDataReady(void) {
    return g_capture_data.new_data;
}

void TimerCapture_ClearDataFlag(void) {
    g_capture_data.new_data = false;
}

float TimerCapture_CalculateFrequency(void) {
    if (g_capture_data.period_ticks == 0) return 0.0f;

    // 获取定时器时钟频率（假设72MHz）
    float timer_freq = 72000000.0f/(TimerCapture_Config.prescaler + 1.0f);

    // 计算频率
    return timer_freq / g_capture_data.period_ticks;
}

float TimerCapture_CalculateDutyCycle(void) {
    if (g_capture_data.period_ticks == 0) return 0.0f;

    // 计算占空比
    return (g_capture_data.high_ticks * 100.0f) / g_capture_data.period_ticks;
}

//float get_capture_Fre(void) {
//    float fre;
//    TimerCapture_Start();
//    fre=TimerCapture_CalculateFrequency();
//    if (fre<1000.0f){
//        dan=0;
//    }
//    else if (fre>1000.0f){
//        fre=fre/1000.0f;
//        dan=1;
//    }
//    else if (fre>1000000.0f){
//        fre/1000000.0f;
//        dan=2;
//    }
//    return fre;
//}