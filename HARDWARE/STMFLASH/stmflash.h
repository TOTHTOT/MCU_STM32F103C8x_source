/*
 * @Description: stm32 FLASģ��EEPROM
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
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 	64 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 	1              	//ʹ��FLASHд��(0��������;1��ʹ��)
#define FLASH_WAITETIME  	50000          	//FLASH�ȴ���ʱʱ��
#define KT_POWER_ON_FLASH_ADDR 0X08009000
#define KT_POWER_OFF_FLASH_ADDR 0X08009300
//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 		//STM32 FLASH����ʼ��ַ

u8 STMFLASH_GetStatus(void);				  //���״̬
u8 STMFLASH_WaitDone(uint16_t time);				  //�ȴ���������
u8 STMFLASH_ErasePage(uint32_t paddr);			  //����ҳ
u8 STMFLASH_WriteHalfWord(uint32_t faddr, uint16_t dat);//д�����
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //��������  
void STMFLASH_WriteLenByte(uint32_t WriteAddr,uint32_t DataToWrite,uint16_t Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr,uint16_t Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(uint32_t WriteAddr,uint16_t WriteData);								   
#endif

















