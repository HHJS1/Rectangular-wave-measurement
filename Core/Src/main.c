/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "oled.h"
#include "stdio.h"
#include "timer_capture.h"
#include "adc_voltage.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t dan=0;
char *Hz[] = {"Hz", "kHz", "MHz"};
float get_capture_Fre(void) {
    float fre;
    TimerCapture_Start();
    fre=TimerCapture_CalculateFrequency();
    if (fre<1000.0f){
        dan=0;
    }
     if (fre>1000.0f){
        fre=fre/1000.0f;
        dan=1;

    }
    else if (fre>1000000.0f){
         fre/1000000.0f;
        dan=2;
    }
     return fre;
}

float get_capture_duty(void) {
    double duty;
    TimerCapture_Start();
    duty=TimerCapture_CalculateDutyCycle();
    return duty;
}

float get_RMS(void) {
    float RMS;
    ADCVoltage_Start();
    RMS=ADCVoltage_GetRMSVoltage()*1000.0; // Convert to mV
    return RMS;
}




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
    HAL_Delay(20);
    TimerCapture_Init(&htim1);
    OLED_Init();
    OLED_NewFrame();
    ADCVoltage_Init(&hadc1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


    while (1)
    {
        OLED_NewFrame();
        OLED_PrintASCIIString(0, 0, "FRE:", &afont8x6, OLED_COLOR_NORMAL);
        //OLED_ShowFrame();
        OLED_PrintFloat(30, 0,get_capture_Fre(), 2, &afont8x6, OLED_COLOR_NORMAL);
        //OLED_ShowFrame();
        OLED_PrintASCIIString(80, 0,  Hz[dan], &afont8x6, OLED_COLOR_NORMAL);
        //OLED_ShowFrame();

        HAL_Delay(200);
        OLED_PrintASCIIString(0, 22, "Duty:", &afont8x6, OLED_COLOR_NORMAL);
        //OLED_ShowFrame();
        HAL_Delay(200);
        OLED_PrintFloat(30, 22,get_capture_duty(), 2, &afont8x6, OLED_COLOR_NORMAL);
        //OLED_ShowFrame();
        OLED_PrintASCIIString(80, 22, "%", &afont8x6, OLED_COLOR_NORMAL);
        //OLED_ShowFrame();

        HAL_Delay(200);
        OLED_PrintASCIIString(0, 44, "RMS:", &afont8x6, OLED_COLOR_NORMAL);
        //OLED_ShowFrame();
        HAL_Delay(200);
        OLED_PrintFloat(30, 44,get_RMS(), 2, &afont8x6, OLED_COLOR_NORMAL);
        //OLED_ShowFrame();
        OLED_PrintASCIIString(80, 44, "mv", &afont8x6, OLED_COLOR_NORMAL);
        OLED_ShowFrame();


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM1) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) {
            // �����ز���
            TimerCapture_RisingCallback(htim);
        } else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
            // �½��ز���
            TimerCapture_FallingCallback(htim);
        }
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
