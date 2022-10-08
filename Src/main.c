/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 ** This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * COPYRIGHT(c) 2022 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "hal_key.h"
#include "gizwits_product.h"
#include "common.h"
#include "led.h"
#include "delay.h"
#include "dht11.h"
#include "oled.h"
#include "hongwai.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define GPIO_KEY_NUM 2                ///< Defines the total number of key member
keyTypedef_t singleKey[GPIO_KEY_NUM]; ///< Defines a single key member array pointer
keysTypedef_t keys;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
uint8_t buf[128];
/* USER CODE END 0 */

int main(void)
{

    /* USER CODE BEGIN 1 */
    uint16_t loop_times = 0;

    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM2_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    MX_I2C1_Init();

    /* Initialize interrupts */
    MX_NVIC_Init();

    /* USER CODE BEGIN 2 */
    delay_init(72);
    timerInit();
    uartInit();
    userInit();
    gizwitsInit();
    GIZWITS_LOG("MCU Init Success \n");
    // DHT11_Init();
    OLED_Init();
    main_page();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        if (loop_times % 100 == 0)
        {
            LED0_TOGGLE; //每500ms led 闪烁一次
            u3_printf("11%d\r\n", 1);
            DHT11_Read_Data(&currentDataPoint.valuewendu, &currentDataPoint.valueshidu);
            gizwitsHandle((dataPoint_t *)&currentDataPoint); //数据上报
            printf("温度:%d,湿度:%d\r\n", currentDataPoint.valuewendu, currentDataPoint.valueshidu);
            main_page_data();
        }
        userHandle();

        loop_times++;
        delay_ms(5);
    }
    /* USER CODE END 3 */
}

/** System Clock Configuration
 */
void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /**Configure the Systick interrupt time
     */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    /**Configure the Systick
     */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** NVIC Configuration
 */
static void MX_NVIC_Init(void)
{
    /* TIM2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    /* USART2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(USART2_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint16_t buf_len;
    uint8_t hw_index;
    switch (GPIO_Pin)
    {
    case wd_down_Pin:
        delay_ms(10);
        if (HAL_GPIO_ReadPin(wd_down_GPIO_Port, wd_down_Pin) == 0)
        {
            if (run_states == default_mode) //默认工作模式
            {
            }
            else if (run_states == learn_mode) //学习模式,按下开始学习遥控器
            {
            }
        }
        break;
    case wd_up_Pin:
        delay_ms(10);
        if (HAL_GPIO_ReadPin(wd_up_GPIO_Port, wd_up_Pin) == 0)
        {
            if (run_states == default_mode) //默认工作模式
            {
                hw_index = 1;
                buf_len = IR_Send_Pack(buf, hw_index);
                HW_Send_Data(buf);
            }
            else if (run_states == learn_mode) //学习模式,按下开始学习遥控器
            {
                hw_index = 1;
                buf_len = IR_Learn_Pack(buf, hw_index);
                HW_Send_Data(buf);
            }
        }
        break;
    case ch_fs_Pin:
        delay_ms(10);
        if (HAL_GPIO_ReadPin(ch_fs_GPIO_Port, ch_fs_Pin) == 0)
        {
            if (run_states == default_mode) //默认工作模式
            {
            }
            else if (run_states == learn_mode) //学习模式,按下开始学习遥控器
            {
            }
        }
        break;
    case ch_mod_Pin:
        delay_ms(10);
        if (HAL_GPIO_ReadPin(ch_mod_GPIO_Port, ch_mod_Pin) == 0)
        {
            if (run_states == default_mode) //默认工作模式
            {
            }
            else if (run_states == learn_mode) //学习模式,按下开始学习遥控器
            {
            }
        }
        break;
    case learn_bt_Pin:
        delay_ms(10);
        if (HAL_GPIO_ReadPin(learn_bt_GPIO_Port, learn_bt_Pin) == 0)
        {
            if (run_states == default_mode)
                run_states = learn_mode;
            else if (run_states == learn_mode)
                run_states = default_mode;
        }
        break;
    default:
        break;
    }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void _Error_Handler(char *file, int line)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}

#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
