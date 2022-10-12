/*
 * @Description:
 * @Author: TOTHTOT
 * @Date: 2022-10-07 20:35:33
 * @LastEditTime: 2022-10-12 17:59:47
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\JieDan\KongTiaoController\STM32\MCU_STM32F103C8x_source\HARDWARE\HONGWAI\hongwai.c
 */
#include "hongwai.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
// #include ""
//  State_Handle run_states = default_mode;
KT_State_Handle KT_run_state ={low, cool, default_mode, 0, 26, 0};

// 内部学码
uint8_t inside_learn_code[7][8]={
    {0x68, 0x08, 0x00, 0xFF, 0x10, 0x00, 0x0F, 0x16}, 
    {0x68, 0x08, 0x00, 0xFF, 0x10, 0x01, 0x10, 0x16}, 
    {0x68, 0x08, 0x00, 0xFF, 0x10, 0x02, 0x11, 0x16}, 
    {0x68, 0x08, 0x00, 0xFF, 0x10, 0x03, 0x12, 0x16}, 
    {0x68, 0x08, 0x00, 0xFF, 0x10, 0x04, 0x13, 0x16}, 
    {0x68, 0x08, 0x00, 0xFF, 0x10, 0x05, 0x14, 0x16}, 
    {0x68, 0x08, 0x00, 0xFF, 0x10, 0x06, 0x15, 0x16}, 
};
// 退出内部学码
uint8_t inside_exit_learn_code[] = {0x68, 0x07, 0x00, 0xFF, 0x11, 0x10, 0x16};

// 内部学发码
uint8_t inside_send_code[7][8] ={
    {0x68, 0x08, 0x00, 0xFF, 0x12, 0x00, 0x11, 0x16}, 
    {0x68, 0x08, 0x00, 0xFF, 0x12, 0x01, 0x12, 0x16}, 
    {0x68, 0x08, 0x00, 0xFF, 0x12, 0x02, 0x13, 0x16}, 
    {0x68, 0x08, 0x00, 0xFF, 0x12, 0x03, 0x14, 0x16}, 
    {0x68, 0x08, 0x00, 0xFF, 0x12, 0x04, 0x15, 0x16}, 
    {0x68, 0x08, 0x00, 0xFF, 0x12, 0x05, 0x16, 0x16}, 
    {0x68, 0x08, 0x00, 0xFF, 0x12, 0x06, 0x17, 0x16}, 
};
uint8_t inside_read_code[][8] = {
    {0x68, 0x08, 0x00, 0xFF, 0x18, 0x00, 0x17, 0x16},
    {0x68, 0x08, 0x00, 0xFF, 0x18, 0x01, 0x18, 0x16},
    {0x68, 0x08, 0x00, 0xFF, 0x18, 0x02, 0x19, 0x16},
    {0x68, 0x08, 0x00, 0xFF, 0x18, 0x03, 0x20, 0x16},
    {0x68, 0x08, 0x00, 0xFF, 0x18, 0x04, 0x21, 0x16},
    {0x68, 0x08, 0x00, 0xFF, 0x18, 0x05, 0x22, 0x16},
    {0x68, 0x08, 0x00, 0xFF, 0x18, 0x06, 0x23, 0x16},
};


/**
 * @name: u3_printf
 * @msg: 自定义printf
 * @param {char} *format
 * @return {*}
 */
void u3_printf(char *format, ...)
{
    char buffer[100];
    uint16_t len; 
    uint16_t i = 0;
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsnprintf(buffer, 100, format, arg_ptr);
    len = vsnprintf((char *)buffer, sizeof(buffer), (char *)format, arg_ptr);
    va_end(arg_ptr);
    HAL_UART_Transmit(&huart3, (uint8_t *)&buffer[i], len, 0xFFFF);
}

/**
 * @name: Get_Check
 * @msg: 求校验和
 * @param {uint8_t} *data 要校验的数据
 * @param {uint16_t} len 长度
 * @return {*} sum 结果
 */
static uint8_t Get_Check(uint8_t *data, uint16_t len)
{
    uint8_t sum = 0;
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        sum += data[i];
    }
    return sum;
}

/**
 * @name: IR_Learn_Pack
 * @msg: 红外内码学习指令组包
 * @param {uint8_t} *data 要发送的数据
 * @param {uint8_t} index 索引(0~6)
 * @return {*} 长度
 */
uint16_t IR_Learn_Pack(uint8_t *data, uint8_t index)
{
    uint8_t *p = data;
    *p++ = FRAME_START;
    *p++ = 0x08;
    *p++ = 0x00;
    *p++ = MODULE_ADDR;
    *p++ = 0x10;
    *p++ = index;
    *p = Get_Check(&data[3], p - data - 3);
    p++;
    *p++ = FRAME_END;
    *p++ = '\0'; //补上结束符
    return p - data;
}

/**
 * @name: IR_Send_Pack
 * @msg: 红外内码发送指令组包
 * @param {uint8_t} *data 数据
 * @param {uint8_t} index 0~6
 * @return {*} 长度
 */
uint16_t IR_Send_Pack(uint8_t *data, uint8_t index)
{
    uint8_t *p = data;
    *p++ = FRAME_START;
    *p++ = 0x08;
    *p++ = 0x00;
    *p++ = MODULE_ADDR;
    *p++ = 0x12;
    *p++ = index;
    *p = Get_Check(&data[3], p - data - 3);
    p++;
    *p++ = FRAME_END;
    return p - data;
}

/**
 * @name: IR_Read_Pack
 * @msg: 红外内部读码指令组合包
 * @param {uint8_t} *data
 * @param {uint8_t} index
 * @return {*}
 */
uint16_t IR_Read_Pack(uint8_t *data, uint8_t index)
{
    uint8_t *p = data;
    *p++ = FRAME_START;
    *p++ = 0x08;
    *p++ = 0x00;
    *p++ = MODULE_ADDR;
    *p++ = 0x18;
    *p++ = index;
    *p = Get_Check(&data[3], p - data - 3);
    printf("码:%d\r\n",Get_Check(&data[3], p - data - 3));
    p++;
    *p++ = FRAME_END;
    return p - data;
}

/**
 * @name: IR_Learn_Outer_Pack
 * @msg: 外部学码指令包
 * @param {uint8_t} *data
 * @return {*}
 */
uint16_t IR_Learn_Outer_Pack(uint8_t *data)
{
    uint8_t *p = data;
    // 68 07 00 FF 20 1F 16
    *p++ = FRAME_START;
    *p++ = 0x07;
    *p++ = 0x00;
    *p++ = MODULE_ADDR;
    *p++ = 0x20;
    *p = 0x1f;
    p++;
    *p++ = FRAME_END;
    return p - data;
}

/**
 * @name: HW_Send_Data
 * @msg: 发送数据
 * @param {char} *data
 * @param {uint16_t} len
 * @return {*}
 */
void HW_Send_Data(uint8_t *data, uint16_t len)
{ 
    uint16_t i = 0;
    while (i<len)
    {
        /* code */
        u3_printf("%c", data[i]);
        // printf("%c", data[i]);
        i++;
    }
    
}
