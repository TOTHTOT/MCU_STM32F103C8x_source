/*
 * @Description: 红外模块驱动
 * @Author: TOTHTOT
 * @Date: 2022-10-07 20:35:44
 * @LastEditTime: 2022-10-16 17:44:43
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\JieDan\KongTiaoController\STM32\MCU_STM32F103C8x_source\HARDWARE\HONGWAI\hongwai.h
 */

#ifndef __HONGWAI_H
#define __HONGWAI_H

#include "usart.h"

#define FRAME_START 0x68
#define FRAME_END 0x16
#define MODULE_ADDR 0xff

typedef enum
{
    learn_mode,
    default_mode
} State_Handle;

typedef enum
{
    low,
    mid,
    high
} WindSpeed_Handle;

typedef enum
{
    cool,
    warm
} WorkMode_Handle;

typedef enum
{
    learn_default,
    learn_on,
    learn_off,
    learn_temp,
    all_learn_type
} Outer_Learn_Handel; // 外部学习标志
typedef struct
{
    WindSpeed_Handle windspeed;
    WorkMode_Handle workmod;
    State_Handle run_mode;
    uint8_t kt_power;
    char kt_temp;
    int learn_temp_flag;
    Outer_Learn_Handel learn_outer;
    uint8_t fs_flag, mod_flag, power_flag, fs_dfault_flag, mod_dfault_flag;
} KT_State_Handle;
extern KT_State_Handle KT_run_state;

// extern  State_Handle run_states;
extern uint8_t inside_learn_code[7][8];
extern uint8_t inside_exit_learn_code[];
extern uint8_t inside_send_code[7][8];
extern uint8_t inside_read_code[][8];
void HongWai_Init(void);

void HW_Send_Data(uint8_t *data, uint16_t len);
void u3_printf(char *format, ...);
static uint8_t Get_Check(uint8_t *data, uint16_t len);
uint16_t IR_Learn_Pack(uint8_t *data, uint8_t index);
uint16_t IR_Send_Pack(uint8_t *data, uint8_t index);
uint16_t IR_Read_Pack(uint8_t *data, uint8_t index);
uint16_t IR_Learn_Outer_Pack(uint8_t *data);

#endif
