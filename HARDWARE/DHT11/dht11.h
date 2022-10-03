/*
 * @Description: 
 * @Author: TOTHTOT
 * @Date: 2022-10-03 16:05:51
 * @LastEditTime: 2022-10-03 16:48:09
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\JieDan\KongTiaoController\STM32\MCU_STM32F103C8x_source\HARDWARE\DHT11\dht11.h
 */
#ifndef __DHT11_H
#define	__DHT11_H

#include "gpio.h"

#define PA7(a)	if (a)	\
					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13, GPIO_PIN_SET);\
					else		\
					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13, GPIO_PIN_RESET);
					
#define PA7_IN   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)
					
void DHT11_IO_OUT(void);
int DHT11_Check(void);
int DHT11_Read_Data(int *temp,int *humi);
void DHT11_Init(void);
#endif

