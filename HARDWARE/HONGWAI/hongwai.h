/*
 * @Description: ºìÍâÄ£¿éÇý¶¯
 * @Author: TOTHTOT
 * @Date: 2022-10-07 20:35:44
 * @LastEditTime: 2022-10-07 21:16:07
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\JieDan\KongTiaoController\STM32\MCU_STM32F103C8x_source\HARDWARE\HONGWAI\hongwai.h
 */

#ifndef __HONGWAI_H
#define __HONGWAI_H

#include "usart.h"

void HongWai_Init(void);

void HW_Send_Data(char *data);
void u3_printf(char *format, ...);

#endif

