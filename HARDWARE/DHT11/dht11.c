/*
 * @Description: 
 * @Author: TOTHTOT
 * @Date: 2022-10-03 16:05:51
 * @LastEditTime: 2022-10-08 11:39:12
 * @LastEditors: TOTHTOT
 * @FilePath: \MDK-ARMe:\JieDan\KongTiaoController\STM32\MCU_STM32F103C8x_source\HARDWARE\DHT11\dht11.c
 */
#include "dht11.h"
#include "delay.h"
#include "usart.h"

void DHT11_IO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = PA7_Pin;			  //ѡ���Ӧ������
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //��ʼ��PC�˿�
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);	   //����
}
void DHT11_IO_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = PA7_Pin;			  //ѡ���Ӧ������
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //��ʼ��PC�˿�
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);	   //����
}
//��λDHT11
void DHT11_Rst(void)
{
	DHT11_IO_OUT(); //����Ϊ���
	PA7(0);			//����DQ
	delay_ms(20);	//��������18ms
	PA7(1);			// DQ=1
	delay_us(30);	//��������20~40us
    
}
// DHT11����Ƿ���ɳ�ʼ��
int DHT11_Check(void)
{
	int retry = 0;
	DHT11_IO_IN();
	while (PA7_IN && retry < 100) // DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};
	if (retry >= 100)
	{
		return 1;
	}
	else
		retry = 0;
	while (!PA7_IN && retry < 100) // DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if (retry >= 100)
	{
		return 1;
	}
	return 0;
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
int DHT11_Read_Bit(void)
{
	int retry = 0;
	while (PA7_IN && retry < 100) //�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry = 0;
	while (!PA7_IN && retry < 100) //�ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
	delay_us(40); //�ȴ�40us
	if (PA7_IN)
		return 1;
	else
		return 0;
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
int DHT11_Read_Byte(void)
{
	int i, dat;
	dat = 0;
	for (i = 0; i < 8; i++)
	{
		dat <<= 1;
		dat |= DHT11_Read_Bit();
	}
	return dat;
}
//��DHT11��ȡһ������
// temp:�¶�ֵ(��Χ:0~50��)
// humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
int DHT11_Read_Data(uint32_t *temp, uint32_t *humi)
{
	int buf[5];
	int i;
	DHT11_Rst();
	if (DHT11_Check() == 0)
	{
		for (i = 0; i < 5; i++) //��ȡ40λ����
		{
			buf[i] = DHT11_Read_Byte();
		}
		if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
		{
			*humi = buf[0];
			*temp = buf[2];
		}
	}
	else
		return 1;
	return 0;
}

void DHT11_Init(void)
{
	while (DHT11_Check())
	{
		DHT11_IO_OUT();
		PA7(0);
		delay_ms(20);
		PA7(1);
		delay_us(35);
		printf("DHT11�����......");
	}
	printf("DHT11���ɹ�");
}
