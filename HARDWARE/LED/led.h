/*
 * @Description: 
 * @Author: TOTHTOT
 * @Date: 2022-07-15 11:08:54
 * @LastEditTime: 2022-10-03 16:37:34
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\JieDan\KongTiaoController\STM32\MCU_STM32F103C8x_source\HARDWARE\LED\led.h
 */
#ifndef __LED_H
#define __LED_H

#include "gpio.h"

#define LED0_TOGGLE HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13)

#define LED0_OFF HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)
#define LED0_ON HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)


#endif



