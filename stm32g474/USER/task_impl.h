#ifndef __TASK_IMPL_H
#define __TASK_IMPL_H

#include "debug_msg.h"
#include "adc_convert.h"
#include "tim.h"
#include "pwm_control.h"
#include "spi_oled.h"

void task_start(void);
void adc_convert_task(void);
void uart_send_task(void);
void pwm_control_task(void);
void spi_oled_task(void);

#endif

