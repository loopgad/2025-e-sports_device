/**
  ******************************************************************************
  * @file    pwm_control.c
  * @brief   PWM控制接口实现（基于HRTIM HAL库）
  ******************************************************************************
  */

#include "pwm_control.h"

/*
 * 从hrtim.c中获取的HRTIM周期配置。
 * 请务必根据您实际的 HRTIM 模块初始化代码（通常在 main.c 或 CubeMX 生成的 hrtim.c 中）
 * 确认 HRTIM 定时器的实际周期值。
 * 例如，如果 HRTIM_TIMEBASE_PERIOD 是 54400，则这里的值应与该值对应。
 * PWM 占空比计算公式为：Compare_Value = Period_Value * DutyCycle / 100
 */
#define HRTIM_PERIOD_VALUE 21760  // 示例值，请根据您的实际HRTIM周期配置进行调整！
#define HRTIM_MIN_CMP_VALUE 96 // 最小比较值



/**
  * @brief  启动静态全局数组中指定的PWM通道输出
  * @param  hhrtim HRTIM句柄指针
  * @retval HAL_StatusTypeDef 操作状态，若所有通道都成功启动返回HAL_OK，否则返回HAL_ERROR
  */
HAL_StatusTypeDef PWM_Start(HRTIM_HandleTypeDef *hhrtim)
{
    HAL_StatusTypeDef status = HAL_OK;

    // HAL_HRTIM_WaveformOutputStart 只接受两个参数
    HAL_StatusTypeDef result1 = HAL_HRTIM_WaveformOutputStart(hhrtim, HRTIM_OUTPUT_TA1|HRTIM_OUTPUT_TA2|HRTIM_OUTPUT_TB1| \
		HRTIM_OUTPUT_TB2|HRTIM_OUTPUT_TC1|HRTIM_OUTPUT_TC2|HRTIM_OUTPUT_TD1|HRTIM_OUTPUT_TD2|
		HRTIM_OUTPUT_TE1|HRTIM_OUTPUT_TE2|HRTIM_OUTPUT_TF1|HRTIM_OUTPUT_TF2);
    if (result1 != HAL_OK) {
        status = HAL_ERROR;
        // 可以在这里添加错误日志，指明哪个通道启动失败
    }
    HAL_StatusTypeDef result2 = HAL_HRTIM_WaveformCounterStart(hhrtim, HRTIM_TIMERID_MASTER | HRTIM_TIMERID_TIMER_A | \
		HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_C | HRTIM_TIMERID_TIMER_D | HRTIM_TIMERID_TIMER_E | \
		HRTIM_TIMERID_TIMER_F); 
    if (result2 != HAL_OK) {
        status = HAL_ERROR;
    }
    

    return status;
}

/**
  * @brief  停止静态全局数组中指定的所有PWM通道输出
  * @param  hhrtim HRTIM句柄指针
  * @retval HAL_StatusTypeDef 操作状态，若所有通道都成功停止返回HAL_OK，否则返回HAL_ERROR
  */
HAL_StatusTypeDef PWM_Stop(HRTIM_HandleTypeDef *hhrtim)
{
    HAL_StatusTypeDef status = HAL_OK;
        // HAL_HRTIM_WaveformOutputStop 只接受两个参数
        HAL_StatusTypeDef result1 = HAL_HRTIM_WaveformOutputStop(hhrtim, HRTIM_OUTPUT_TA1|HRTIM_OUTPUT_TA2|HRTIM_OUTPUT_TB1| \
		HRTIM_OUTPUT_TB2|HRTIM_OUTPUT_TC1|HRTIM_OUTPUT_TC2|HRTIM_OUTPUT_TD1|HRTIM_OUTPUT_TD2|
		HRTIM_OUTPUT_TE1|HRTIM_OUTPUT_TE2|HRTIM_OUTPUT_TF1|HRTIM_OUTPUT_TF2);
        if (result1 != HAL_OK) {
            status = HAL_ERROR;
        }
		HAL_StatusTypeDef result2 = HAL_HRTIM_WaveformCounterStop(hhrtim, HRTIM_TIMERID_MASTER | HRTIM_TIMERID_TIMER_A | \
		HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_C | HRTIM_TIMERID_TIMER_D | HRTIM_TIMERID_TIMER_E | \
		HRTIM_TIMERID_TIMER_F); 
        if (result2 != HAL_OK) {
            status = HAL_ERROR;
        }
    

    return status;
}


 /**
    * @brief  设置指定子定时器的PWM脉冲值（直接设置比较寄存器值），同一子定时器通道互补
    * @param  hhrtim HRTIM句柄指针
    * @param  child_tim（Child_TIMTypeDef枚举）
    * @param  pulseValue 脉冲值（实际写入比较寄存器的值，自动限幅为0~HRTIM_PERIOD_VALUE）
    * @retval HAL_StatusTypeDef 操作状态（HAL_OK表示成功，HAL_ERROR表示通道无效）
    */
inline HAL_StatusTypeDef PWM_SetPulse(HRTIM_HandleTypeDef *hhrtim, Child_TIMTypeDef child_tim, uint32_t pulseValue)
{
    // 检查通道枚举值是否在有效范围内
    if (child_tim >= Timer_COUNT) {
        return HAL_ERROR;
    }

    // 对脉冲值进行限幅（0~HRTIM_PERIOD_VALUE）
    pulseValue = (pulseValue > HRTIM_PERIOD_VALUE) ? HRTIM_PERIOD_VALUE : pulseValue;
    pulseValue = (pulseValue < HRTIM_MIN_CMP_VALUE) ? HRTIM_MIN_CMP_VALUE : pulseValue;

    // 设置指定子定时器比较器的值
    hhrtim->Instance->sTimerxRegs[child_tim].CMP1xR = pulseValue;
    //	hhrtim1.Instance->sTimerxRegs[0].PERxR = 50000;//通过修改重装载值PER，从而修改PWM的频率
    // hhrtim1.Instance->sTimerxRegs[0].CMP1xR = 504;//通过修改比较值CMP，从而修改占空比	
    //sTimerxRegs[0]---->Timer_A
    //sTimerxRegs[1]---->Timer_B
    //sTimerxRegs[2]---->Timer_C
    //sTimerxRegs[3]---->Timer_D
    //sTimerxRegs[4]---->Timer_E
    //sTimerxRegs[5]---->Timer_F
    return HAL_OK;
}


