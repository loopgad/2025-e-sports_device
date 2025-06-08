#ifndef __OLED_H
#define __OLED_H 	


#include "gpio.h" // 包含 gpio.h 以获取 GPIO 定义
#include "spi.h"  // 包含 spi.h 以获取 SPI 定义

// 定义 OLED_MODE 为 4 线串行模式
#define OLED_MODE 0

// 定义 OLED_CMD 和 OLED_DATA
#define OLED_CMD  0 // 表示发送的是命令
#define OLED_DATA 1 // 表示发送的是数据

// OLED 引脚定义（根据 STM32 HAL 调整）
#define OLED_RST_GPIO_Port  GPIOB
#define OLED_RST_Pin        GPIO_PIN_5 // 对应 gpio.c 中的 PB5

#define OLED_DC_GPIO_Port   GPIOA
#define OLED_DC_Pin         GPIO_PIN_6 // 对应 gpio.c 中的 PA6

// 控制 OLED 引脚的宏定义
#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET)
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET)

#define OLED_DC_Clr()  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)
#define OLED_DC_Set()  HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64

// 定义 OLED 帧缓冲区（8 页 x 128 列 = 128x64 像素）
// 结构为 OLED_GRAM[页][列]
extern uint8_t OLED_GRAM[8][128];

//-----------------OLED 控制函数-----------------
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr);
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t); // 声明绘点函数
void OLED_Refresh_Gram(void); // 声明新的刷新屏幕函数

#endif