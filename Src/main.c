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
    uint8_t hw_buf[128];
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
    
    DHT11_Init();
    // OLED_Init();
    // main_page();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
        if (loop_times % 50 == 0)
        {
            // u3_printf("11%d\r\n", 1);
            DHT11_Read_Data(&currentDataPoint.valuewendu, &currentDataPoint.valueshidu);
            
            gizwitsHandle((dataPoint_t *)&currentDataPoint); //数据上报
            // main_page_data();
            // uartWrite("111\r\n", 5);
        }
        if (loop_times % 100 == 0)
        {
            LED0_TOGGLE; //每500ms led 闪烁一次
            // printf("温度:%d,湿度:%d\r\n", currentDataPoint.valuewendu, currentDataPoint.valueshidu);
            loop_times = 0;
        }
        userHandle();

        loop_times++;
        if (usart_send_state != DIS_USEND)
        {
            switch (usart_send_state)
            {
            case EN_U1SEND:
// LED0_TOGGLE;
                printf("data:%s", USART1_RX_BUF);
#if 0
                if (strcmp((char *)USART1_RX_BUF, "111") == 0)
                {
                    // printf("send data\r\n");
                    HW_Send_Data((char *)buf, IR_Learn_Pack(buf, 0));
                }
                else if (strcmp((char *)USART1_RX_BUF, "222") == 0)
                {
                    HW_Send_Data((char *)buf, IR_Send_Pack(buf, 0));
                }
                else if (strcmp((char *)USART1_RX_BUF, "333") == 0)
                {
                    HW_Send_Data((char *)inside_exit_learn_code, 9);
                }
                // u3_printf("data:%s\r\n",inside_learn_code[0][0]);
#endif
                if(strcmp(USART1_RX_BUF, "soft")==0)
                {
                   printf("\r\nairlink:%d\r\n", gizwitsSetMode(WIFI_SOFTAP_MODE));
                }
                if(strcmp(USART1_RX_BUF, "airlink")==0)
                {
                   printf("\r\nairlink:%d\r\n", gizwitsSetMode(WIFI_AIRLINK_MODE));
                }
                if(strcmp(USART1_RX_BUF, "hw_output_0")==0)
                {
                   HW_Send_Data(hw_buf, IR_Read_Pack(hw_buf, 0));
                }
                if(strcmp(USART1_RX_BUF, "hw_output_1")==0)
                {
                   HW_Send_Data(hw_buf, IR_Read_Pack(hw_buf, 1));
                }
                if(strcmp(USART1_RX_BUF, "hw_output_2")==0)
                {
                   HW_Send_Data(hw_buf, IR_Read_Pack(hw_buf, 2));
                }
                if(strcmp(USART1_RX_BUF, "hw_output_3")==0)
                {
                   HW_Send_Data(hw_buf, IR_Read_Pack(hw_buf, 3));
                }
                if(strcmp(USART1_RX_BUF, "hw_output_4")==0)
                {
                  HW_Send_Data(hw_buf, IR_Read_Pack(hw_buf, 4));
                }
                if(strcmp(USART1_RX_BUF, "hw_output_5")==0)
                {
                   HW_Send_Data(hw_buf, IR_Read_Pack(hw_buf, 5));
                }
                if(strcmp(USART1_RX_BUF, "hw_output_6")==0)
                {
                   HW_Send_Data(hw_buf, IR_Read_Pack(hw_buf, 6));
                }

                USART1_RX_STA = 0;
                memset(USART1_RX_BUF, 0, sizeof(USART1_RX_BUF));
                usart_send_state = DIS_USEND;
                break;
            case EN_U2SEND:
                usart_send_state = DIS_USEND;
                break;
            case EN_U3SEND:
                usart_send_state = DIS_USEND;
                break;
            case 4:
                printf("no compelite\r\n");
                usart_send_state = DIS_USEND;
                break;
            default:
                break;
            }
        }
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

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
  HAL_NVIC_SetPriority(USART2_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    //  uint16_t buf_len;
    uint8_t hw_index;
    static u8 fs_flag = 0, mod_flag = 0, fs_dfault_flag = 0,mod_dfault_flag = 0;
    /* 按下"学习"按钮进入学习模式,按如下顺序学习:
    1.进入学习模式OLED显示"Learning"
    2.按下"温度增加"红外模块绿灯亮起,空调遥控器对准孔外接收管,按下对应按键,红外模块LED熄灭表示学习完成;
    3.按下"温度降低",同上操作;
    4.按下"风速", 同上操作;
    5.按下"模式", 同上操作;
    6.在完成学习后再次按下学习按钮退出学习模式.
     */
    switch (GPIO_Pin)
    {
    case wd_up_Pin:
        delay_ms(10);
        if (HAL_GPIO_ReadPin(wd_up_GPIO_Port, wd_up_Pin) == 0)
        {
            if (KT_run_state.run_mode == default_mode) //默认工作模式
            {
                hw_index = 0;
                KT_run_state.kt_temp++;
                HW_Send_Data((char *)buf, IR_Send_Pack(buf, hw_index));
            }
            else if (KT_run_state.run_mode == learn_mode) //学习模式,按下开始学习遥控器
            {
                hw_index = 0;
                printf("学习增温\r\n");
                HW_Send_Data((char *)buf, IR_Learn_Pack(buf, hw_index));
            }
        }
        break;
    case wd_down_Pin:
        delay_ms(10);
        if (HAL_GPIO_ReadPin(wd_down_GPIO_Port, wd_down_Pin) == 0)
        {
            if (KT_run_state.run_mode == default_mode) //默认工作模式
            {
                hw_index = 1;
                KT_run_state.kt_temp--;
                HW_Send_Data((char *)buf, IR_Send_Pack(buf, hw_index));
            }
            else if (KT_run_state.run_mode == learn_mode) //学习模式,按下开始学习遥控器
            {
                hw_index = 1;
                printf("学习降温\r\n");
                HW_Send_Data((char *)buf, IR_Learn_Pack(buf, hw_index));
            }
        }
        break;
    case ch_fs_Pin:
        delay_ms(10);
        if (HAL_GPIO_ReadPin(ch_fs_GPIO_Port, ch_fs_Pin) == 0)
        {
            if (KT_run_state.run_mode == default_mode) //默认工作模式
            {
                if(fs_dfault_flag == 0) //等于0 发送一档风速
                {
                    hw_index = 2;
                    fs_dfault_flag = 1;
                    HW_Send_Data((char *)buf, IR_Send_Pack(buf, hw_index));
                }
                else if(fs_dfault_flag == 1) //等于1 发送二档风速
                {
                    hw_index = 3;
                    fs_dfault_flag = 2;
                    HW_Send_Data((char *)buf, IR_Send_Pack(buf, hw_index));
                }
                else if(fs_dfault_flag == 2) //等于2 发送三档风速
                {
                    hw_index = 4;
                    fs_dfault_flag =0;
                    HW_Send_Data((char *)buf, IR_Send_Pack(buf, hw_index));
                }
            }
            else if (KT_run_state.run_mode == learn_mode) //学习模式,按下开始学习遥控器
            {
                if(fs_flag == 0)    // 等于0 学习一档风速
                {
                    hw_index = 2;
                    fs_flag = 1;
                    printf("学习一档风速\r\n");
                    HW_Send_Data((char *)buf, IR_Learn_Pack(buf, hw_index));
                }
                else if(fs_flag == 1)   // 等于1 学习二档风速
                {
                    hw_index = 3;
                    fs_flag = 2;
                    printf("学习二档风速\r\n");
                    HW_Send_Data((char *)buf, IR_Learn_Pack(buf, hw_index));
                }
                else if(fs_flag == 2)   // 等于2 学习三档风速
                {
                    hw_index = 4;
                    fs_flag = 0;
                    printf("学习三档风速\r\n");
                    HW_Send_Data((char *)buf, IR_Learn_Pack(buf, hw_index));
                }
            }
        }
        break;
    case ch_mod_Pin:
        delay_ms(10);
        if (HAL_GPIO_ReadPin(ch_mod_GPIO_Port, ch_mod_Pin) == 0)
        {
            if (KT_run_state.run_mode == default_mode) //默认工作模式
            {
                if(mod_dfault_flag == 0)
                {
                    hw_index = 5;
                    HW_Send_Data((char *)buf, IR_Send_Pack(buf, hw_index));
                    mod_dfault_flag = 1;
                }
                else if(mod_dfault_flag == 1)
                {
                    hw_index = 6;
                    HW_Send_Data((char *)buf, IR_Send_Pack(buf, hw_index));
                    mod_dfault_flag = 0;
                }
            }
            else if (KT_run_state.run_mode == learn_mode) //学习模式,按下开始学习遥控器
            {
                if(mod_flag == 0)   // 等于0 学习制冷模式
                {
                    hw_index = 5;
                    mod_flag = 1;
                    printf("学习制冷\r\n");
                    HW_Send_Data((char *)buf, IR_Learn_Pack(buf, hw_index));
                }
                else if(mod_flag == 1)   // 等于1 学习制暖模式
                {
                    hw_index = 6;
                    mod_flag = 0;
                    printf("学习制暖\r\n");
                    HW_Send_Data((char *)buf, IR_Learn_Pack(buf, hw_index));
                }
            }
        }
        break;
    case learn_bt_Pin:
        delay_ms(10);
        if (HAL_GPIO_ReadPin(learn_bt_GPIO_Port, learn_bt_Pin) == 0)
        {
            if (KT_run_state.run_mode == default_mode)
            {
                KT_run_state.run_mode = learn_mode;
                printf("学习模式\r\n");
            }
            else if (KT_run_state.run_mode == learn_mode)
            {
                KT_run_state.run_mode = default_mode;
                printf("默认模式\r\n");
            }
            //main_page_data();
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
void _Error_Handler(char * file, int line)
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
void assert_failed(uint8_t* file, uint32_t line)
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
