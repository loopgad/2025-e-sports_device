// 包含调试消息的头文件
#include "debug_msg.h"

// 定义消息帧的起始字节 0
#define HEAD0 0xAA
// 定义消息帧的起始字节 1
#define HEAD1 0xAD
// 定义消息帧的结束字节 0
#define TAIL0 0x0A
// 定义消息帧的结束字节 1
#define TAIL1 0x0D

// 定义一个消息数组，用于存储待发送的消息，初始化为 0
msg send_msg[MAX_LENGTH] = {0};
// 定义一个发送缓冲区，长度为消息最大长度加 4 字节（2 字节头部和 2 字节尾部），初始化为 0
uint8_t send_buf[MAX_LENGTH*4+4] = {0};

/**
 * @brief 发送调试消息到指定的 UART 接口
 * @param huart 指向 UART 句柄的指针，用于指定要使用的 UART 接口
 * @param debug_msg 指向要发送的消息数组的指针
 * @param length 要发送的消息数量
 * @return 无
 */
void send_debug_msg(UART_HandleTypeDef *huart, msg* debug_msg, uint8_t length){
    // 设置发送缓冲区的起始字节 0
    send_buf[0] = HEAD0;
    // 设置发送缓冲区的起始字节 1
    send_buf[1] = HEAD1;
    // 设置发送缓冲区的结束字节 0
    send_buf[length*4+2] = TAIL0;
    // 设置发送缓冲区的结束字节 1
    send_buf[length*4+3] = TAIL1;
    // 遍历要发送的消息数组
    for(int i = 0; i < length; i++){
        // 遍历每个消息的 4 字节数据
        for(int j = 0; j < 4; j++){
            // 将消息数据复制到发送缓冲区
            send_buf[i*4+j+2] = debug_msg[i].arr[j];
        }
    }
    // 通过 UART 接口发送数据，超时时间设置为 HAL_MAX_DELAY
    HAL_UART_Transmit(huart, send_buf, length*4+4, HAL_MAX_DELAY);
}