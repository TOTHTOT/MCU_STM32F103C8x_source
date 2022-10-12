/*
 * @Description: stm32 FLAS模拟EEPROM
 * @Author: TOTHTOT
 * @Date: 2022-10-12 11:38:14
 * @LastEditTime: 2022-10-12 12:17:39
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\JieDan\KongTiaoController\STM32\MCU_STM32F103C8x_source\HARDWARE\STMFLASH\stmflash.h
 */
#ifndef __STMFLASH_H__
#define __STMFLASH_H__
// #include 
#include "delay.h"
#include "usart.h"
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 	64 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 	1              	//使能FLASH写入(0，不是能;1，使能)
#define FLASH_WAITETIME  	50000          	//FLASH等待超时时间
#define KT_POWER_ON_FLASH_ADDR 0X08009000
#define KT_POWER_OFF_FLASH_ADDR 0X08009800
//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 		//STM32 FLASH的起始地址

u8 STMFLASH_GetStatus(void);				  //获得状态
u8 STMFLASH_WaitDone(uint16_t time);				  //等待操作结束
u8 STMFLASH_ErasePage(uint32_t paddr);			  //擦除页
u8 STMFLASH_WriteHalfWord(uint32_t faddr, uint16_t dat);//写入半字
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //读出半字  
void STMFLASH_WriteLenByte(uint32_t WriteAddr,uint32_t DataToWrite,uint16_t Len);	//指定地址开始写入指定长度的数据
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr,uint16_t Len);						//指定地址开始读取指定长度数据
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   		//从指定地址开始读出指定长度的数据

//测试写入
void Test_Write(uint32_t WriteAddr,uint16_t WriteData);								   
#endif

















