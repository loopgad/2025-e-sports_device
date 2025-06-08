#include "task_impl.h"

void task_start(void){
	// 启动 TIM3（中断方式）
    if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
    {
			Error_Handler();
    }
	// 启动 TIM4（中断方式）
    if (HAL_TIM_Base_Start_IT(&htim4) != HAL_OK)
    {
			Error_Handler();
    }		
	 // 启动 TIM6（中断方式）
    if (HAL_TIM_Base_Start_IT(&htim6) != HAL_OK)
    {
			Error_Handler();
    }

    // 启动 TIM7（中断方式）
    if (HAL_TIM_Base_Start_IT(&htim7) != HAL_OK)
    {
			Error_Handler();
    }

    // 启动 TIM16（中断方式）
    if (HAL_TIM_Base_Start_IT(&htim16) != HAL_OK)
    {
			Error_Handler();
    }

    // 启动 TIM17（中断方式）
    if (HAL_TIM_Base_Start_IT(&htim17) != HAL_OK)
    {
			Error_Handler();
    }
		
    // 启动adc的dma传输
    HAL_ADC_DMA_Init();
    
    // 启动hhrtim的pwm输出
    if (PWM_Start(&hhrtim1) != HAL_OK)
   {
			Error_Handler();
   }
   
   // oled init
   OLED_Init();

}

void pwm_control_task(void)
{


PWM_SetPulse(&hhrtim1, Timer_A, 789);

}

void adc_convert_task(void)
{
    float *diff_values = adc_val_diff();

    if (diff_values != NULL) {
        adc_results.voltage_in    = adc_convert_voltage(diff_values[ADC_VOLT_IN]);
        adc_results.voltage_out   = adc_convert_voltage(diff_values[ADC_VOLT_OUT]);
        adc_results.current_in    = adc_convert_current(diff_values[ADC_CURR_IN]);
        adc_results.current_out   = adc_convert_current(diff_values[ADC_CURR_OUT]);
    }
}

void uart_send_task(void)
{	
		send_msg[0].data = adc_results.voltage_in;
    send_msg[1].data = adc_results.voltage_out;
    send_msg[2].data = adc_results.current_in;
    send_msg[3].data = adc_results.current_out;

	send_debug_msg(&huart4, send_msg, 4);
}

void spi_oled_task(void)
{
    OLED_ShowChar(25,4,'H');
}

/*
tim1,8,16,17,20,2-7 主频170MHz
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
        adc_convert_task();
    }

    if (htim->Instance == TIM7)
    {
        pwm_control_task();
    }
    
    if (htim->Instance == TIM16)
    {
        uart_send_task();
    }
    
    if (htim->Instance == TIM17)
    {
        spi_oled_task();
    }
}