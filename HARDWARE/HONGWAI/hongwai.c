/*
 * @Description:
 * @Author: TOTHTOT
 * @Date: 2022-10-07 20:35:33
 * @LastEditTime: 2022-10-08 12:24:34
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\JieDan\KongTiaoController\STM32\MCU_STM32F103C8x_source\HARDWARE\HONGWAI\hongwai.c
 */
#include "hongwai.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

enum State_handle run_states;
run_states = default_mode;
/**
 * @name: u3_printf
 * @msg: �Զ���printf
 * @param {char} *format
 * @return {*}
 */
void u3_printf(char *format, ...)
{
    char buffer[100];
    uint16_t i = 0;
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsnprintf(buffer, 100, format, arg_ptr);
    while (i < 99 && buffer[i])
    {
        HAL_UART_Transmit(&huart3, (uint8_t *)&buffer[i], 1, 0xFFFF);
        i++;
    }
    va_end(arg_ptr);
}

/**
 * @name: Get_Check
 * @msg: ��У���
 * @param {uint8_t} *data ҪУ�������
 * @param {uint16_t} len ����
 * @return {*} sum ���
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
 * @msg: ��������ѧϰָ�����
 * @param {uint8_t} *data Ҫ���͵�����
 * @param {uint8_t} index ����(0~6)
 * @return {*} ����
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
    *p++ = '\0'; //���Ͻ�����
    return p - data;
}

/**
 * @name: IR_Send_Pack
 * @msg: �������뷢��ָ�����
 * @param {uint8_t} *data ����
 * @param {uint8_t} index 0~6
 * @return {*} ����
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

void HW_Send_Data(char *data)
{
    u3_printf("%s", data);
}
