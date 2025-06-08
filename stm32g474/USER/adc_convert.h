#ifndef __ADC_CONVERT_H
#define __ADC_CONVERT_H

#include "adc.h"

// 枚举用于表示 ADC 差分值的通道
typedef enum {
    ADC_VOLT_IN,     // 电压输入通道
    ADC_VOLT_OUT,    // 电压输出通道
    ADC_CURR_IN,     // 电流输入通道
    ADC_CURR_OUT,    // 电流输出通道
    ADC_DIFF_CH_MAX
} AdcDiffChannel_t;

typedef struct {
    float voltage_in;
    float voltage_out;
    float current_in;
    float current_out;
} AdcConvertResult_t;

// 声明结构体变量
extern AdcConvertResult_t adc_results;

// 函数声明
float* adc_val_diff(void);
float adc_convert_voltage(float adc_value);
float adc_convert_current(float adc_value);

#endif