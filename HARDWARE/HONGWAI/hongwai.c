/*
 * @Description:
 * @Author: TOTHTOT
 * @Date: 2022-10-07 20:35:33
 * @LastEditTime: 2022-10-07 21:19:11
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\JieDan\KongTiaoController\STM32\MCU_STM32F103C8x_source\HARDWARE\HONGWAI\hongwai.c
 */
#include "hongwai.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

/**
 * @name: u3_printf
 * @msg: ×Ô¶¨Òåprintf
 * @param {char} *format
 * @return {*}
 */
void u3_printf(char *format, ...)
{
    char buffer[100];
    uint16_t i=0;
    va_list arg_ptr;
    va_start(arg_ptr,format);
    vsnprintf(buffer,100,format,arg_ptr);
    while(i<99&&buffer[i])
    {
        HAL_UART_Transmit(&huart3,(uint8_t *)&buffer[i],1,0xFFFF);
        i++;
    }
    va_end(arg_ptr);
}

void HW_Send_Data(char *data)
{

    u3_printf();
}
