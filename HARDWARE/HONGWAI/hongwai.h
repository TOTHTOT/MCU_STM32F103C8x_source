/*
 * @Description: ºìÍâÄ£¿éÇý¶¯
 * @Author: TOTHTOT
 * @Date: 2022-10-07 20:35:44
 * @LastEditTime: 2022-10-11 21:30:20
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\JieDan\KongTiaoController\STM32\MCU_STM32F103C8x_source\HARDWARE\HONGWAI\hongwai.h
 */

#ifndef __HONGWAI_H
#define __HONGWAI_H

#include "usart.h"

#define FRAME_START		0x68
#define FRAME_END		0x16
#define MODULE_ADDR		0xff

typedef enum 
{
    learn_mode,
    default_mode
}State_Handle;

typedef enum 
{
    low,
    mid,
    high
}WindSpeed_Handle;

typedef enum 
{
    cool,
    warm
}WorkMode_Handle;

typedef struct 
{
	WorkMode_Handle workmod;
    WindSpeed_Handle windspeed;
    State_Handle run_mode;
    char kt_temp;
}KT_State_Handle;
extern KT_State_Handle KT_run_state;

// extern  State_Handle run_states;
extern uint8_t inside_learn_code[7][8];
extern uint8_t inside_exit_learn_code[];
extern uint8_t inside_send_code[7][8];
extern uint8_t inside_read_code[][8];
void HongWai_Init(void);

void HW_Send_Data(char *data, uint8_t len);
void u3_printf(char *format, ...);
static uint8_t Get_Check(uint8_t *data, uint16_t len);
uint16_t IR_Learn_Pack(uint8_t *data, uint8_t index);
uint16_t IR_Send_Pack(uint8_t *data, uint8_t index);
uint16_t IR_Read_Pack(uint8_t *data, uint8_t index);

#endif

