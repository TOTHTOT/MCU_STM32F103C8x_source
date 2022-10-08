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
	GPIO_InitStruct.Pin = PA7_Pin;			  //选择对应的引脚
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //初始化PC端口
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);	   //拉低
}
void DHT11_IO_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = PA7_Pin;			  //选择对应的引脚
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //初始化PC端口
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);	   //拉低
}
//复位DHT11
void DHT11_Rst(void)
{
	DHT11_IO_OUT(); //设置为输出
	PA7(0);			//拉低DQ
	delay_ms(20);	//拉低至少18ms
	PA7(1);			// DQ=1
	delay_us(30);	//主机拉高20~40us
    
}
// DHT11检查是否完成初始化
int DHT11_Check(void)
{
	int retry = 0;
	DHT11_IO_IN();
	while (PA7_IN && retry < 100) // DHT11会拉低40~80us
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
	while (!PA7_IN && retry < 100) // DHT11拉低后会再次拉高40~80us
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

//从DHT11读取一个位
//返回值：1/0
int DHT11_Read_Bit(void)
{
	int retry = 0;
	while (PA7_IN && retry < 100) //等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry = 0;
	while (!PA7_IN && retry < 100) //等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40); //等待40us
	if (PA7_IN)
		return 1;
	else
		return 0;
}

//从DHT11读取一个字节
//返回值：读到的数据
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
//从DHT11读取一次数据
// temp:温度值(范围:0~50°)
// humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
int DHT11_Read_Data(uint32_t *temp, uint32_t *humi)
{
	int buf[5];
	int i;
	DHT11_Rst();
	if (DHT11_Check() == 0)
	{
		for (i = 0; i < 5; i++) //读取40位数据
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
		printf("DHT11检测中......");
	}
	printf("DHT11检测成功");
}
