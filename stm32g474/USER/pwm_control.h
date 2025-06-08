/**
  ******************************************************************************
  * @file    pwm_control.h
  * @brief   PWM控制接口头文件
  ******************************************************************************
  */
  #ifndef PWM_CONTROL_H
  #define PWM_CONTROL_H
  
  #include "hrtim.h" // 确保 hrtim.h 包含 HAL_StatusTypeDef 和 HRTIM_HandleTypeDef 的定义
  
 
typedef enum {
    Timer_A = 0,   // Timer A 
    Timer_B = 1,   // Timer B 
    Timer_C = 2,   // Timer C 
    Timer_D = 3,   // Timer D 
    Timer_E = 4,   // Timer E 
    Timer_F = 5,   // Timer F 
    Timer_COUNT = 6  // 用于计数，总是放在枚举的最后
} Child_TIMTypeDef;

  /**
    * @brief  启动PWM通道输出。
    * @param  hhrtim HRTIM句柄指针
    * @retval HAL_StatusTypeDef 操作状态，若所有通道都成功启动返回HAL_OK，否则返回HAL_ERROR
    */
  HAL_StatusTypeDef PWM_Start(HRTIM_HandleTypeDef *hhrtim);
  
  /**
    * @brief  停止静态PWM通道输出。
    * @param  hhrtim HRTIM句柄指针
    * @retval HAL_StatusTypeDef 操作状态，若所有通道都成功停止返回HAL_OK，否则返回HAL_ERROR
    */
  HAL_StatusTypeDef PWM_Stop(HRTIM_HandleTypeDef *hhrtim);
  
  /**
    * @brief  设置指定子定时器的PWM脉冲值（直接设置比较寄存器值），同一子定时器通道互补
    * @param  hhrtim HRTIM句柄指针
    * @param  child_tim（Child_TIMTypeDef枚举）
    * @param  pulseValue 脉冲值（实际写入比较寄存器的值，自动限幅为0~HRTIM_PERIOD_VALUE）
    * @retval HAL_StatusTypeDef 操作状态（HAL_OK表示成功，HAL_ERROR表示通道无效）
    */
  HAL_StatusTypeDef PWM_SetPulse(HRTIM_HandleTypeDef *hhrtim, Child_TIMTypeDef child_tim, uint32_t pulseValue);
  
  #endif /* PWM_CONTROL_H */