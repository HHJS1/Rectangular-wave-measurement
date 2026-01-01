//
// Created by 30427 on 2025/12/21.
//

#include "adc_voltage.h"
#include "main.h"

// 全局变量定义
static ADC_HandleTypeDef *g_hadc = NULL;
static ADC_Config_t g_adc_config = {
        .vref = 3.3f,
        .resolution = 4095,
        .scaling_factor = 1.0f,
        .oversampling = 16
};
static float g_cal_gain = 1.0f;
static float g_cal_offset = 0.0f;

void ADCVoltage_Init(ADC_HandleTypeDef *hadc) {
    g_hadc = hadc;
    HAL_ADCEx_Calibration_Start(hadc);
}

void ADCVoltage_Start(void) {
    if (g_hadc == NULL) return;
    HAL_ADC_Start(g_hadc);
}

void ADCVoltage_Stop(void) {
    if (g_hadc == NULL) return;
    HAL_ADC_Stop(g_hadc);
}

float ADCVoltage_GetRawValue(void) {
    if (g_hadc == NULL) return 0.0f;

    uint32_t adc_sum = 0;

    // 过采样
    for (uint8_t i = 0; i < g_adc_config.oversampling; i++) {
        HAL_ADC_Start(g_hadc);
        if (HAL_ADC_PollForConversion(g_hadc, 10) == HAL_OK) {
            adc_sum += HAL_ADC_GetValue(g_hadc);
        }
        HAL_ADC_Stop(g_hadc);
    }

    return (float)adc_sum / g_adc_config.oversampling;
}

float ADCVoltage_GetVoltage(void) {
    float adc_value = ADCVoltage_GetRawValue();

    // 转换为电压值
    float voltage = (adc_value * g_adc_config.vref) / g_adc_config.resolution;

    // 应用校准
    return (voltage * g_cal_gain) + g_cal_offset;
}

float ADCVoltage_GetRMSVoltage(void) {
    float voltage = ADCVoltage_GetVoltage();


    return voltage / 1.13f;
}

void ADCVoltage_SetCalibration(float gain, float offset) {
    g_cal_gain = gain;
    g_cal_offset = offset;
}

void ADCVoltage_ApplyFilter(uint8_t filter_type) {
    // 滤波器实现
    switch (filter_type) {
        case 0: // 无滤波
            g_adc_config.oversampling = 1;
            break;
        case 1: // 4倍过采样
            g_adc_config.oversampling = 4;
            break;
        case 2: // 16倍过采样
            g_adc_config.oversampling = 16;
            break;
        default:
            g_adc_config.oversampling = 16;
            break;
    }
}
