/*
 * @Description: stm32 FLASģ��EEPROM
 * @Author: TOTHTOT
 * @Date: 2022-10-12 11:38:14
 * @LastEditTime: 2022-10-16 15:47:02
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\JieDan\KongTiaoController\STM32\MCU_STM32F103C8x_source\HARDWARE\STMFLASH\stmflash.h
 */
#ifndef __STMFLASH_H__
#define __STMFLASH_H__
// #include
#include "delay.h"
#include "usart.h"
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 64   //��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1    //ʹ��FLASHд��(0��������;1��ʹ��)
#define FLASH_WAITETIME 50000 // FLASH�ȴ���ʱʱ��
#define KT_POWER_ON_FLASH_ADDR 0X08009000
#define KT_POWER_OFF_FLASH_ADDR 0X08009154
#define KT_TEMP_MIN 20                    // ��С�¶�
#define KT_TEMP_MAX 30                    // ����¶�
#define KT_TEMP_ADDER_INCREASE 0x154     // ÿ��λ�Ƶĵ�ַ
#define KT_TEMP_DATA_NUM 10               // �ܹ�����Ŀ����¶ȵ�ָ�����
#define KT_TEMP_20_FLASH_ADDR 0X080092A8 // �� FLASH �д����¶ȵĻ�׼��ַ,����ÿ�ζ��� KT_TEMP_ADDER_INCREASE �� �ܼƴ��� KT_TEMP_DATA_NUM ��
// #define KT_TEMP_21_FLASH_ADDR 0X08009CF00
// #define KT_TEMP_22_FLASH_ADDR 0X08009E400
// #define KT_TEMP_23_FLASH_ADDR 0X08009F900
// #define KT_TEMP_24_FLASH_ADDR 0X080097000
// #define KT_TEMP_25_FLASH_ADDR 0X080097500
// #define KT_TEMP_26_FLASH_ADDR 0X080099000
// #define KT_TEMP_27_FLASH_ADDR 0X080099500
// #define KT_TEMP_28_FLASH_ADDR 0X080091500
// #define KT_TEMP_29_FLASH_ADDR 0X080091500
// #define KT_TEMP_230_FLASH_ADDR 0X080091500
#define KT_READ_MAX_LENTH 512 // ��ȡ flash ����󳤶�,��������ɿ���
// FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 // STM32 FLASH����ʼ��ַ

u8 STMFLASH_GetStatus(void);                                                        //���״̬
u8 STMFLASH_WaitDone(uint16_t time);                                                //�ȴ���������
u8 STMFLASH_ErasePage(uint32_t paddr);                                              //����ҳ
u8 STMFLASH_WriteHalfWord(uint32_t faddr, uint16_t dat);                            //д�����
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);                                     //��������
void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len); //ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len);                     //ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);    //��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead);       //��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(uint32_t WriteAddr, uint16_t WriteData);
#endif
