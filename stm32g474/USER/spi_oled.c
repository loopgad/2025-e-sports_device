#include "spi_oled.h"
#include "oledfont.h"  	 
#include "spi.h" // 导入 spi.h 以使用 hspi1 extern
#include "gpio.h" // 导入 gpio.h 以获取 GPIO 定义
#include <string.h> // 导入 string.h 用于 memset

extern SPI_HandleTypeDef hspi1; // 声明来自 spi.c 的 SPI 句柄

// 定义 OLED 帧缓冲区（8 页 x 128 列 = 128x64 像素）
// 结构为 OLED_GRAM[页][列]
// 每个字节的位从下到上对应8个像素（0-7行）
uint8_t OLED_GRAM[8][128];

// 软件延迟函数（如果可用且更倾向于使用 HAL_Delay，可以替换）
// 参数 ms: 延迟毫秒数
void delay_ms(unsigned int ms)
{                         
	HAL_Delay(ms); // 使用 HAL_Delay 以获得更好的可移植性和准确性（如果 HAL 已配置）
}

// 向 SSD1306 写入一个字节（命令或数据）
// 注意：此函数现在主要用于发送命令，数据发送将通过 OLED_Refresh_Gram 使用 DMA 完成。
// 参数 dat: 要写入的字节
// 参数 cmd: 0 为命令，1 为数据
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{	
	if(cmd) // 数据 (OLED_DATA)
	{
	  OLED_DC_Set();
	}
	else    // 命令 (OLED_CMD)
	{
	  OLED_DC_Clr();		  
	}
    HAL_SPI_Transmit(&hspi1, &dat, 1, HAL_MAX_DELAY); // 使用阻塞式传输发送命令或单个字节
}

// 打开 OLED 显示
void OLED_Display_On(void)
{
	OLED_WR_Byte(0x8D, OLED_CMD);  // 设置充电泵命令
	OLED_WR_Byte(0x14, OLED_CMD);  // 使能充电泵
	OLED_WR_Byte(0xAF, OLED_CMD);  // 显示开启
}

// 关闭 OLED 显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0x8D, OLED_CMD);  // 设置充电泵命令
	OLED_WR_Byte(0x10, OLED_CMD);  // 禁用充电泵
	OLED_WR_Byte(0xAE, OLED_CMD);  // 显示关闭
}

// 设置显示起始位置
// 参数 x: X坐标 (0-127)
// 参数 y: 页地址 (0-7)
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
	OLED_WR_Byte(0xB0 + y, OLED_CMD); // 设置页地址
	OLED_WR_Byte(((x & 0xF0) >> 4) | 0x10, OLED_CMD); // 设置列高位地址
	OLED_WR_Byte((x & 0x0F), OLED_CMD); // 设置列低位地址
} 

// 清除屏幕内容（清空帧缓冲区）
void OLED_Clear(void)  
{  
	memset(OLED_GRAM, 0x00, sizeof(OLED_GRAM)); // 将帧缓冲区所有字节清零
}

// 用点阵填充屏幕区域
// 参数 x1, y1: 起始坐标
// 参数 x2, y2: 结束坐标
// 参数 dot: 填充点的值 (0 或 1)
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot)  
{  
	uint8_t x, y;  
	for(x = x1; x <= x2; x++)
	{
		for(y = y1; y <= y2; y++)
			OLED_DrawPoint(x, y, dot);
	}	
}

// 绘制单个点到帧缓冲区
// 参数 x: X坐标 (0-127)
// 参数 y: Y坐标 (0-63)
// 参数 t: 点的值 (0: 清除, 1: 设置)
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
    if(x >= X_WIDTH || y >= Y_WIDTH) return; // 检查坐标是否越界

	uint8_t page = y / 8; // 计算所在的页 (0-7)
	uint8_t bit_pos = y % 8; // 计算在字节中的位位置 (0-7)

	if(t) // 设置点
		OLED_GRAM[page][x] |= (1 << bit_pos); // 访问方式改为 OLED_GRAM[页][列]
	else // 清除点
		OLED_GRAM[page][x] &= ~(1 << bit_pos); // 访问方式改为 OLED_GRAM[页][列]
}

// 显示字符到帧缓冲区
// 参数 x: X坐标 (0-127)
// 参数 y: 页地址 (0-7)
// 参数 chr: 要显示的字符
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr)
{      	
	uint8_t c = chr - ' '; // 计算字符在字体数组中的索引
	uint8_t i;
	if (x > (Max_Column - 6)) { x = 0; y = y + 1; } // 换行处理，y是页地址，所以加1表示下一页
	
	for(i = 0; i < 6; i++) // 遍历字符的每一列
	{
		// 直接写入帧缓冲区
		OLED_GRAM[y][x + i] = F6x8[c][i]; // 访问方式改为 OLED_GRAM[页][列]
	}
}

// 显示字符串到帧缓冲区
// 参数 x: X坐标 (0-127)
// 参数 y: 页地址 (0-7)
// 参数 chr: 字符串指针
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr)
{
	unsigned char j = 0;
	while (chr[j] != '\0')
	{		
		OLED_ShowChar(x, y, chr[j]);
		x += 6; // 对于 6x8 字体，每个字符宽 6 像素
		if(x > 120){x = 0; y += 1;} // 如果需要，换到下一页（y是页地址，加1）
		j++;
	}
}

// 显示 BMP 图像到帧缓冲区 (128x64)
// 参数 x0, y0_page: 起始 X 坐标和起始页地址
// 参数 x1, y1_page: 结束 X 坐标和结束页地址
// 参数 BMP: BMP 图像数据数组
void OLED_DrawBMP(uint8_t x0, uint8_t y0_page, uint8_t x1, uint8_t y1_page, uint8_t BMP[])
{ 	
	unsigned int i = 0;
	uint8_t x, y_p; // y_p 代表页地址

	for(y_p = y0_page; y_p <= y1_page; y_p++) // 遍历页
	{
		for(x = x0; x < x1; x++) // 遍历列
		{      
			// 确保不越界访问 BMP 数组
			if (i < (x1 - x0) * (y1_page - y0_page + 1)) 
			{
				OLED_GRAM[y_p][x] = BMP[i]; // 直接写入帧缓冲区，访问方式改为 OLED_GRAM[页][列]
				i++;
			}
		}
	}
} 

// 刷新帧缓冲区内容到 OLED 屏幕（通过 DMA）
void OLED_Refresh_Gram(void)
{
	uint8_t i;
	for(i = 0; i < 8; i++) // 遍历每一页
	{
		// 设置页地址和列地址，准备发送数据
		OLED_WR_Byte(0xB0 + i, OLED_CMD);    // 设置页地址 (0~7)
		OLED_WR_Byte(0x00, OLED_CMD);      // 设置低列地址 (列0)
		OLED_WR_Byte(0x10, OLED_CMD);      // 设置高列地址 (列0)   

		// 准备通过 DMA 发送整页数据 (128 字节)
		OLED_DC_Set();   // DC 高电平，表示发送数据
		// 使用 DMA 传输整行数据。OLED_GRAM[i] 现在直接是第 i 页128字节数据的起始地址。
//		 HAL_SPI_Transmit(&hspi1, OLED_GRAM[i], 128, HAL_MAX_DELAY);
		HAL_SPI_Transmit_DMA(&hspi1, OLED_GRAM[i], 128); 
		// 等待 DMA 传输完成，确保数据完整发送后再继续下一个操作
		while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
	}
}

// OLED 初始化（修改为6引脚设计，无CS引脚，有RST和DC引脚）
void OLED_Init(void)
{
	OLED_RST_Clr(); // 拉低 RST 引脚进行复位
	delay_ms(100);  // 复位延迟
	OLED_RST_Set(); // 释放复位
	delay_ms(100);

	OLED_WR_Byte(0xAE, OLED_CMD); // 显示关闭
	OLED_WR_Byte(0x20, OLED_CMD); // 设置内存寻址模式
	OLED_WR_Byte(0x02, OLED_CMD); // 0x02,设置页寻址模式 (RESET)
	OLED_WR_Byte(0xB0, OLED_CMD); // 设置页地址 (0-7)
	OLED_WR_Byte(0xC8, OLED_CMD); // 设置 COM/行扫描方向 0xc8正常
	OLED_WR_Byte(0x00, OLED_CMD); // 设置低列地址 (0)
	OLED_WR_Byte(0x10, OLED_CMD); // 设置高列地址 (0)
	OLED_WR_Byte(0x40, OLED_CMD); // 设置起始行地址 (0)
	OLED_WR_Byte(0x81, OLED_CMD); // 设置对比度控制寄存器
	OLED_WR_Byte(0xFF, OLED_CMD); // 设置 SEG 输出电流亮度 (0x00-0xFF)
	OLED_WR_Byte(0xA1, OLED_CMD); // 设置 SEG/列映射 0xa1正常
	OLED_WR_Byte(0xA6, OLED_CMD); // 设置正常显示
	OLED_WR_Byte(0xA8, OLED_CMD); // 设置多路复用率
	OLED_WR_Byte(0x3F, OLED_CMD); // 设置 MUX 比例为 63
	OLED_WR_Byte(0xD3, OLED_CMD); // 设置显示偏移
	OLED_WR_Byte(0x00, OLED_CMD); // 无偏移
	OLED_WR_Byte(0xD5, OLED_CMD); // 设置显示时钟分频比
	OLED_WR_Byte(0xF0, OLED_CMD); // 设置分频因子
	OLED_WR_Byte(0xD9, OLED_CMD); // 设置预充电周期
	OLED_WR_Byte(0xF1, OLED_CMD); // 设置预充电周期
	OLED_WR_Byte(0xDA, OLED_CMD); // 设置 COM 引脚配置
	OLED_WR_Byte(0x12, OLED_CMD); // 默认值
	OLED_WR_Byte(0xDB, OLED_CMD); // 设置 VCOMH 电压
	OLED_WR_Byte(0x30, OLED_CMD); // 设置 VCOMH 电压
	OLED_WR_Byte(0x8D, OLED_CMD); // 设置充电泵
	OLED_WR_Byte(0x14, OLED_CMD); // 启用充电泵
	OLED_WR_Byte(0xAF, OLED_CMD); // 打开 OLED
	OLED_Clear(); // 清空帧缓冲区
	OLED_Refresh_Gram(); // 刷新到屏幕，确保初始化时屏幕为空
}