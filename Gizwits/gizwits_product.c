/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related       hardware initialization
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/

#include <stdio.h>
#include <string.h>
#include "hal_key.h"
#include "gizwits_product.h"
#include "common.h"
#include "led.h"
#include "hongwai.h"
#include "stmflash.h"
#include "oled.h"
static uint32_t timerMsCount;
uint8_t aRxBuffer;
uint8_t aRxBuffer_1;
uint8_t aRxBuffer_3;
/** User area the current device state structure*/
dataPoint_t currentDataPoint;
extern keysTypedef_t keys;

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
uint8_t USART1_RX_BUF[200];
uint8_t USART3_RX_BUF[200];
uint16_t USART1_RX_STA = 0;
uint16_t USART3_RX_STA = 0;
uint8_t usart_send_state = DIS_USEND;
/**@} */
/**@name Gizwits User Interface
 * @{
 */

/**
 * @name: my_eventprocess
 * @msg: 事件处理函数
 * @param {EVENT_TYPE_T} event_type 事件类型
 * @return {*}
 */
void my_eventprocess(EVENT_TYPE_T event_type)
{
    uint8_t buf[1024];
    uint8_t hw_index;
    uint16_t tt1 = 0;
    char t_t = 0;
    switch (event_type)
    {
    case EVENT_windspeed:
        if (currentDataPoint.valuewindspeed == 0)
        {
            hw_index = 2;
            KT_run_state.fs_dfault_flag = 1;
            KT_run_state.windspeed = low;
            HW_Send_Data((uint8_t *)buf, IR_Send_Pack(buf, hw_index));
        }
        else if (currentDataPoint.valuewindspeed == 1)
        {
            hw_index = 3;
            KT_run_state.fs_dfault_flag = 2;
            KT_run_state.windspeed = mid;
            HW_Send_Data((uint8_t *)buf, IR_Send_Pack(buf, hw_index));
        }
        else if (currentDataPoint.valuewindspeed == 2)
        {
            hw_index = 4;
            KT_run_state.fs_dfault_flag = 0;
            KT_run_state.windspeed = high;
            HW_Send_Data((uint8_t *)buf, IR_Send_Pack(buf, hw_index));
        }
        break;
    case EVENT_work_mod: //工作模式选择,制冷(0)或者制热(1)
        if (currentDataPoint.valuework_mod == 0)
        {
            hw_index = 5;
            KT_run_state.mod_dfault_flag = 1;
            KT_run_state.workmod = cool;
            HW_Send_Data((uint8_t *)buf, IR_Send_Pack(buf, hw_index));
        }
        else if (currentDataPoint.valuework_mod == 1)
        {
            hw_index = 6;
            KT_run_state.mod_dfault_flag = 0;
            KT_run_state.workmod = warm;
            HW_Send_Data((uint8_t *)buf, IR_Send_Pack(buf, hw_index));
        }
        break;
    case EVENT_wemdu_kongzhi: // 根据温度发送相关的温度指令 20到30度
        KT_run_state.kt_temp = currentDataPoint.valuewemdu_kongzhi;
        if (KT_run_state.kt_temp > KT_TEMP_MAX)
        {
            KT_run_state.kt_temp = KT_TEMP_MAX;
            currentDataPoint.valuewemdu_kongzhi = KT_TEMP_MAX;
        }
        else if (KT_run_state.kt_temp < KT_TEMP_MIN)
        {
            KT_run_state.kt_temp = KT_TEMP_MIN;
            currentDataPoint.valuewemdu_kongzhi = KT_TEMP_MIN;
        }
        t_t = KT_run_state.kt_temp - KT_TEMP_MIN;

        // 增加控制
        STMFLASH_Read(KT_TEMP_20_FLASH_ADDR + (KT_TEMP_ADDER_INCREASE * t_t), &tt1, 2); // 读取长度
        if (tt1 > KT_READ_MAX_LENTH)
        {
            OLED_Clear();
            OLED_ShowString(0, 0, "Error:read lenth to big!!", 8);
            tt1 = 512;
        }
        STMFLASH_Read(KT_TEMP_20_FLASH_ADDR + (KT_TEMP_ADDER_INCREASE * t_t) + 2, (uint16_t *)buf, 99);

        printf("\r\n***************buflen:%d, temp:%d, add:0x%x***************\r\n", tt1, t_t + 20, (KT_TEMP_20_FLASH_ADDR + (KT_TEMP_ADDER_INCREASE * t_t) + 2));
        HW_Send_Data((uint8_t *)buf, tt1);
        memset(buf, 0, sizeof(buf));
        break;
    case EVENT_power:
        if (currentDataPoint.valuepower == 0)
        {
            // 关机
            STMFLASH_Read(KT_POWER_OFF_FLASH_ADDR, &tt1, 2); // 读取长度
            if (tt1 > KT_READ_MAX_LENTH)
            {
                tt1 = 512;
                OLED_Clear();
                OLED_ShowString(0, 0, "Error:read lenth to big!!", 8);
            }
            printf("\r\n***************buflen:%d***************\r\n", tt1);
            STMFLASH_Read(KT_POWER_OFF_FLASH_ADDR + 2, (uint16_t *)buf, tt1);

            HW_Send_Data((uint8_t *)buf, tt1);
            memset(buf, 0, sizeof(buf));
            currentDataPoint.valuepower = 0;
            KT_run_state.kt_power = 0;
            KT_run_state.kt_power = 0;
            currentDataPoint.valuepower = 0; /*  */
            // HW_Send_Data(buf, IR_Send_Pack(buf, 5));
        }
        else if (currentDataPoint.valuepower == 1)
        {
            // 开机
            STMFLASH_Read(KT_POWER_ON_FLASH_ADDR, &tt1, 2); // 读取长度
            if (tt1 > KT_READ_MAX_LENTH)
            {
                tt1 = 512;
                OLED_Clear();
                OLED_ShowString(0, 0, "Error:read lenth to big!!", 8);
            }
            STMFLASH_Read(KT_POWER_ON_FLASH_ADDR + 2, (uint16_t *)buf, tt1);

            // u3_printf("\r\nbuflen:%d\r\n", tt1);
            HW_Send_Data((uint8_t *)buf, tt1);
            memset(buf, 0, sizeof(buf));
            currentDataPoint.valuepower = 1;
            KT_run_state.kt_power = 1;
            // HW_Send_Data(buf, IR_Send_Pack(buf, 5));
        }
        break;
    default:
        break;
    }
}

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
    uint8_t i = 0;
    dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
    moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
    protocolTime_t *ptime = (protocolTime_t *)gizdata;

#if MODULE_TYPE
    gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
    moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

    if ((NULL == info) || (NULL == gizdata))
    {
        return -1;
    }

    for (i = 0; i < info->num; i++)
    {
        switch (info->event[i])
        {
        case EVENT_power:
            currentDataPoint.valuepower = dataPointPtr->valuepower;
            GIZWITS_LOG("Evt: EVENT_power %d \n", currentDataPoint.valuepower);
            if (0x01 == currentDataPoint.valuepower)
            {
                // user handle
                printf("\r\n***********KTPower ON***********\r\n");
                my_eventprocess(EVENT_power);
            }
            else
            {
                printf("\r\n***********KTPower OFF***********\r\n");
                my_eventprocess(EVENT_power);
                // user handle
            }
            break;

        case EVENT_windspeed:
            currentDataPoint.valuewindspeed = dataPointPtr->valuewindspeed;
            GIZWITS_LOG("Evt: EVENT_windspeed %d\n", currentDataPoint.valuewindspeed);
            switch (currentDataPoint.valuewindspeed)
            {
            case windspeed_VALUE0:
                // user handle
                printf("\r\n***********WindSpeed Low***********\r\n");
                my_eventprocess(EVENT_windspeed);
                break;
            case windspeed_VALUE1:
                // user handle
                printf("\r\n***********WindSpeed Mid***********\r\n");
                my_eventprocess(EVENT_windspeed);
                break;
            case windspeed_VALUE2:
                // user handle
                printf("\r\n***********WindSpeed High***********\r\n");
                my_eventprocess(EVENT_windspeed);
                break;
            default:
                break;
            }
            break;
        case EVENT_work_mod:
            currentDataPoint.valuework_mod = dataPointPtr->valuework_mod;
            GIZWITS_LOG("Evt: EVENT_work_mod %d\n", currentDataPoint.valuework_mod);
            switch (currentDataPoint.valuework_mod)
            {
            case work_mod_VALUE0:
                printf("\r\n***********WorkMod Cool***********\r\n");
                my_eventprocess(EVENT_work_mod);
                // user handle
                break;
            case work_mod_VALUE1:
                printf("\r\n***********WorkMod Warm***********\r\n");
                my_eventprocess(EVENT_work_mod);
                // user handle
                break;
            default:
                break;
            }
            break;

        case EVENT_wemdu_kongzhi:
            currentDataPoint.valuewemdu_kongzhi = dataPointPtr->valuewemdu_kongzhi;
            GIZWITS_LOG("Evt:EVENT_wemdu_kongzhi %d\n", currentDataPoint.valuewemdu_kongzhi);
            printf("\r\n***********WorkMod Warm***********\r\n");
            my_eventprocess(EVENT_wemdu_kongzhi);
            // user handle
            break;

        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER:

            break;
        case WIFI_DISCON_ROUTER:

            break;
        case WIFI_CON_M2M:

            break;
        case WIFI_DISCON_M2M:
            break;
        case WIFI_RSSI:
            GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
            break;
        case TRANSPARENT_DATA:
            GIZWITS_LOG("TRANSPARENT_DATA \n");
            // user handle , Fetch data from [data] , size is [len]
            break;
        case WIFI_NTP:
            GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n", ptime->year, ptime->month, ptime->day, ptime->hour, ptime->minute, ptime->second, ptime->ntp);
            break;
        case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
#if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            // Format By gprsInfo_t
#else
            GIZWITS_LOG("WIF MODULE ...\n");
            // Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n", ptModuleInfo->moduleType);
#endif
            break;
        default:
            break;
        }
    }

    return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
    /*
       currentDataPoint.valuewendu = ;//Add Sensor Data Collection
       currentDataPoint.valueshidu = ;//Add Sensor Data Collection

       */
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
// void userInit(void)
// {
//     memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));

//     /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/
//     /*
//       currentDataPoint.valuepower = ;
//       currentDataPoint.valuewindspeed = ;
//       currentDataPoint.valuework_mod = ;
//       currentDataPoint.valuewendu = ;
//       currentDataPoint.valuewemdu_kongzhi = ;
//       currentDataPoint.valueshidu = ;
//     */

// }

void userInit(void)
{
    memset((uint8_t *)&currentDataPoint, 0, sizeof(dataPoint_t));
    HAL_GPIO_WritePin(ESP_RST_GPIO_Port, ESP_RST_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ESP_IO0_GPIO_Port, ESP_IO0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ESP_IO2_GPIO_Port, ESP_IO2_Pin, GPIO_PIN_SET);
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/

    currentDataPoint.valuepower = 0;
    currentDataPoint.valuewindspeed = 0;
    currentDataPoint.valuework_mod = 0;
    currentDataPoint.valuewendu = 0;
    currentDataPoint.valuewemdu_kongzhi = 26;
    currentDataPoint.valueshidu = 0;
}

/**
* @brief Millisecond timing maintenance function, milliseconds increment, overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief Read millisecond count

* @param none
* @return millisecond count
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief MCU reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
}

/**@} */

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART1 and Loop until the end of transmission */
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim2)
    {
        keyHandle((keysTypedef_t *)&keys);
        gizTimerMs();
    }
}

/**
* @brief Timer TIM3 init function

* @param none
* @return none
*/
void timerInit(void)
{
    HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief  This function handles USART IDLE interrupt.
 */
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef*UartHandle)
// {
//     if(UartHandle->Instance == USART2)
//     {
// 				gizPutData((uint8_t *)&aRxBuffer, 1);

//         HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断
//     }
// }
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    static uint8_t receive_buf[1024], head_flag = 0;
    static uint16_t i = 0;
    // 不能在串口里同时收发 会造成死锁,改用标志
    if (UartHandle->Instance == USART2)
    {
        gizPutData((uint8_t *)&aRxBuffer, 1);
        HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1); //开启下一次接收中断
    }
    else if (UartHandle->Instance == USART3)
    {
        usart_send_state = EN_U3SEND;
        if (KT_run_state.learn_outer != 0) // 开启外部学习模式时将数据储存到 FLASH
        {
            if (aRxBuffer_3 == 0x68) // 起始符
            {
                head_flag++;
                i = 0;
                receive_buf[i] = aRxBuffer_3;
                i++;
            }
            else if (aRxBuffer_3 == 0x16 && head_flag == 2)
            {
                receive_buf[i] = aRxBuffer_3;
                i++;
                if (KT_run_state.learn_outer == learn_on) // 学习开机
                {
                    printf("\r\n************save data len:%d************\r\n", i);
                    STMFLASH_Write(KT_POWER_ON_FLASH_ADDR, &i, 2);
                    STMFLASH_Write(KT_POWER_ON_FLASH_ADDR + 2, (uint16_t *)receive_buf, i);
                    i = 0;
                    head_flag = 0;
                    memset(receive_buf, 0, sizeof(receive_buf));
                }
                else if (KT_run_state.learn_outer == learn_off) // 学习关机
                {
                    printf("\r\n************save data len:%d************\r\n", i);
                    STMFLASH_Write(KT_POWER_OFF_FLASH_ADDR, &i, 2);
                    STMFLASH_Write(KT_POWER_OFF_FLASH_ADDR + 2, (uint16_t *)receive_buf, i);
                    i = 0;
                    head_flag = 0;
                    memset(receive_buf, 0, sizeof(receive_buf));
                }
                else if (KT_run_state.learn_outer == learn_temp) //学习温度
                {
                    i++;
                    receive_buf[i] = KT_run_state.learn_temp_flag + 20;
                    i--;
                    printf("\r\n************save data len:%d************\r\n", i);
                    STMFLASH_Write(KT_TEMP_20_FLASH_ADDR + (KT_TEMP_ADDER_INCREASE * KT_run_state.learn_temp_flag), &i, 2);
                    STMFLASH_Write(KT_TEMP_20_FLASH_ADDR + (KT_TEMP_ADDER_INCREASE * KT_run_state.learn_temp_flag) + 2, (uint16_t *)receive_buf, i);
                    if (KT_run_state.learn_temp_flag == KT_TEMP_DATA_NUM) // 学习完成时
                    {
                        printf("learn temp success\r\n");
                        KT_run_state.learn_temp_flag = -1;
                    }
                    i = 0;
                    head_flag = 0;
                    memset(receive_buf, 0, sizeof(receive_buf));
                }
                else
                {
                    u3_printf("unknow\r\n");
                }
                KT_run_state.learn_outer = learn_default;
            }
            else
            {
                receive_buf[i] = aRxBuffer_3;
                i++;
            }
            if (aRxBuffer_3 == 0x16)
            {
                LED0_TOGGLE;
            }
        }
        while (HAL_UART_Receive_IT(&huart3, (uint8_t *)&aRxBuffer_3, 1) != HAL_OK) //开启下一次接收中断
        {
            /* HAL_UART_Receive_IT 内部会锁住 */
            huart3.RxState = HAL_UART_STATE_READY;
            __HAL_UNLOCK(&huart3);
        }
    }
    else if (UartHandle->Instance == USART1)
    {
        // 串口1 中断
        if ((USART1_RX_STA & 0x8000) == 0) //接收未完成
        {
            if (USART1_RX_STA & 0x4000) //接收到了0x0d
            {
                if (aRxBuffer_1 != 0x0a)
                    USART1_RX_STA = 0; //接收错误,重新开始
                else
                    USART1_RX_STA |= 0x8000; //接收完成了
            }
            else //还没收到0X0D
            {
                if (aRxBuffer_1 == 0x0d)
                    USART1_RX_STA |= 0x4000;
                else
                {
                    USART1_RX_BUF[USART1_RX_STA & 0X3FFF] = aRxBuffer_1;
                    USART1_RX_STA++;
                    if (USART1_RX_STA > (200 - 1))
                        USART1_RX_STA = 0; //接收数据错误,重新开始接收
                }
            }
            // usart_send_state = 4;
        }
        if (USART1_RX_STA & 0x8000)
        {
            usart_send_state = EN_U1SEND;
        }
        while (HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer_1, 1) != HAL_OK) //开启下一次接收中断
        {
            /* HAL_UART_Receive_IT 内部会锁住 */
            huart1.RxState = HAL_UART_STATE_READY;
            __HAL_UNLOCK(&huart1);
        }
    }
}
/**
* @brief USART init function

* Serial communication between WiFi modules and device MCU
* @param none
* @return none
*/
// void uartInit(void)
// {
// 	HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断
// }
void uartInit(void)
{
    // huart1.pRxBuffPtr = (uint8_t *)malloc(1);
    // huart3.pRxBuffPtr = (uint8_t *)malloc(1);
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer_1, 1); //开启下一次接收中断
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);   //开启下一次接收中断
    HAL_UART_Receive_IT(&huart3, (uint8_t *)&aRxBuffer_3, 1); //开启下一次接收中断
}
/**
 * @brief Serial port write operation, send data to WiFi module
 *
 * @param buf      : buf address
 * @param len      : buf length
 *
 * @return : Return effective data length;-1，return failure
 */
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint8_t crc[1] = {0x55};
    uint32_t i = 0;

    if (NULL == buf)
    {
        return -1;
    }

    for (i = 0; i < len; i++)
    {
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)&buf[i], 1);
        while (huart2.gState != HAL_UART_STATE_READY)
            ; // Loop until the end of transmission

        if (i >= 2 && buf[i] == 0xFF)
        {
            HAL_UART_Transmit_IT(&huart2, (uint8_t *)&crc, 1);
            while (huart2.gState != HAL_UART_STATE_READY)
                ; // Loop until the end of transmission
        }
    }

#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for (i = 0; i < len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);

        if (i >= 2 && buf[i] == 0xFF)
        {
            GIZWITS_LOG("%02x ", 0x55);
        }
    }
    GIZWITS_LOG("\n");
#endif

    return len;
}
