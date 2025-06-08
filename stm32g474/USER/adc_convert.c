#include "adc_convert.h"

// 初始化结构体变量
AdcConvertResult_t adc_results = {
    .voltage_in    = 0.0f,
    .voltage_out   = 0.0f,
    .current_in    = 0.0f,
    .current_out   = 0.0f,
};

// 静态数组用于保存差分结果
#define ADC_DIFF_CH_MAX  8
static float adc_diff_values[ADC_DIFF_CH_MAX];

/**
 * @brief 将 ADC 值转换为电压（单位：V）
 */
float adc_convert_voltage(float adc_value)
{
    const float vref = 3.3f;
    const uint16_t adc_max = 65535; // 16-bit ADC

    return (adc_value * vref) / adc_max;
}

/**
 * @brief 将 ADC 值转换为电流（单位：A）
 */
float adc_convert_current(float adc_value)
{
    const float vref = 3.3f;
    const uint16_t adc_max = 65535;
    const float r_shunt = 0.1f;      // 分流电阻阻值（单位：Ω）
    const float gain_ina = 20.0f;    // 放大器增益

    float voltage = (adc_value * vref) / adc_max;
    float current = (voltage / r_shunt) * gain_ina;

    return current;
}


/**
 * @brief 计算 ADC 差分值并返回数组（访问注意数组越界问题）
 */
float* adc_val_diff(void)
{
    adc_diff_values[ADC_VOLT_IN]  = adc1_dma_buffer[0] - adc1_dma_buffer[1]; // voltage_in
    adc_diff_values[ADC_VOLT_OUT] = adc1_dma_buffer[2] - adc1_dma_buffer[3]; // voltage_out
    adc_diff_values[ADC_CURR_IN]  = adc3_dma_buffer[0] - adc3_dma_buffer[1]; // current_in
    adc_diff_values[ADC_CURR_OUT] = adc3_dma_buffer[2] - adc3_dma_buffer[3]; // current_out

    return adc_diff_values;
}