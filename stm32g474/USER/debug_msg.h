// 防止头文件重复包含
#ifndef __DEBUG_MSG_H
#define __DEBUG_MSG_H

// 包含串口通信相关的头文件
#include "usart.h"

// 定义最大长度常量，用于后续数组大小的定义
#define MAX_LENGTH 8

// 定义一个联合体，用于将4字节的无符号字符数组和浮点数进行相互转换
typedef union {
    // 4字节的无符号字符数组
    uint8_t arr[4];
    // 浮点数数据
    float data;
}msg;

// 声明一个外部的msg类型数组，用于存储要发送的消息，数组大小为MAX_LENGTH
extern msg send_msg[MAX_LENGTH];
// 声明一个外部的无符号字符型数组，用于存储发送缓冲区，大小为MAX_LENGTH + 4
extern uint8_t send_buf[MAX_LENGTH*4+4];

/**
 * @brief 发送调试消息到串口
 * 
 * @param huart 串口句柄，指向UART_HandleTypeDef结构体的指针
 * @param debug_msg 指向要发送的调试消息数组的指针，类型为msg*
 * @param length 要发送的调试消息的长度
 */
void send_debug_msg(UART_HandleTypeDef *huart, msg* debug_msg, uint8_t length);

#endif



