/*
 * @Description: ºìÍâÄ£¿éÇý¶¯
 * @Author: TOTHTOT
 * @Date: 2022-10-07 20:35:44
 * @LastEditTime: 2022-10-08 12:06:45
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\JieDan\KongTiaoController\STM32\MCU_STM32F103C8x_source\HARDWARE\HONGWAI\hongwai.h
 */

#ifndef __HONGWAI_H
#define __HONGWAI_H

#include "usart.h"

#define FRAME_START		0x68
#define FRAME_END		0x16
#define MODULE_ADDR		0xff

enum State_handle
{
    learn_mode,
    default_mode
};
extern enum State_handle run_states;

void HongWai_Init(void);

void HW_Send_Data(char *data);
void u3_printf(char *format, ...);
static uint8_t Get_Check(uint8_t *data, uint16_t len);
uint16_t IR_Learn_Pack(uint8_t *data, uint8_t index);
uint16_t IR_Send_Pack(uint8_t *data, uint8_t index);

#endif

