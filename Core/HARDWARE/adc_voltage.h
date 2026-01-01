//
// Created by 30427 on 2025/12/21.
//

#ifndef RECTANGULAR_WAVE_ADC_VOLTAGE_H
#define RECTANGULAR_WAVE_ADC_VOLTAGE_H

#include "stdint.h"
#include "main.h"
// ADC配置结构
typedef struct {
    float vref;              // 参考电压(V)
    uint32_t resolution;     // ADC分辨率(12bit=4096)
    float scaling_factor;    // 缩放系数
    uint8_t oversampling;    // 过采样次数
} ADC_Config_t;

// 函数声明
void ADCVoltage_Init(ADC_HandleTypeDef *hadc);
void ADCVoltage_Start(void);
void ADCVoltage_Stop(void);
float ADCVoltage_GetRawValue(void);
float ADCVoltage_GetVoltage(void);
float ADCVoltage_GetRMSVoltage(void);
void ADCVoltage_SetCalibration(float gain, float offset);
void ADCVoltage_ApplyFilter(uint8_t filter_type);




#endif //RECTANGULAR_WAVE_ADC_VOLTAGE_H
