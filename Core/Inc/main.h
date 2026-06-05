/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
  

#include "Rework_PP_Drv.h"  
#include "hsDebug_MSG.h"
#include "hsCommon_Utill.h"
#include "hsGlobalErrorHandle.h"


#include "Common_HAL.h"
  
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MPW_ONOFF_Pin GPIO_PIN_13
#define MPW_ONOFF_GPIO_Port GPIOC
#define CC_IN_CK_Pin GPIO_PIN_14
#define CC_IN_CK_GPIO_Port GPIOC
#define CC_PW_nONOFF_Pin GPIO_PIN_15
#define CC_PW_nONOFF_GPIO_Port GPIOC
#define BLE_C_CK_Pin GPIO_PIN_0
#define BLE_C_CK_GPIO_Port GPIOF
#define BLE_PW_nONOFF_Pin GPIO_PIN_1
#define BLE_PW_nONOFF_GPIO_Port GPIOF
#define BAT_LV_ADC1_IN4_Pin GPIO_PIN_0
#define BAT_LV_ADC1_IN4_GPIO_Port GPIOA
#define EX_PW_CK_Pin GPIO_PIN_5
#define EX_PW_CK_GPIO_Port GPIOA
#define COVID_G_Pin GPIO_PIN_4
#define COVID_G_GPIO_Port GPIOC
#define COVID_R_Pin GPIO_PIN_5
#define COVID_R_GPIO_Port GPIOC
#define OPT_LED_SEL1_Pin GPIO_PIN_0
#define OPT_LED_SEL1_GPIO_Port GPIOB
#define OPT_LED_SEL2_Pin GPIO_PIN_1
#define OPT_LED_SEL2_GPIO_Port GPIOB
#define OPT_LED_SEL3_Pin GPIO_PIN_2
#define OPT_LED_SEL3_GPIO_Port GPIOB
#define nSTA_CHG_OD_Pin GPIO_PIN_10
#define nSTA_CHG_OD_GPIO_Port GPIOB
#define INSERT_SW_Pin GPIO_PIN_12
#define INSERT_SW_GPIO_Port GPIOB
#define Prog_R1_Pin GPIO_PIN_15
#define Prog_R1_GPIO_Port GPIOB
#define Prog_G1_Pin GPIO_PIN_6
#define Prog_G1_GPIO_Port GPIOC
#define Prog_G2_Pin GPIO_PIN_7
#define Prog_G2_GPIO_Port GPIOC
#define Prog_G3_Pin GPIO_PIN_8
#define Prog_G3_GPIO_Port GPIOC
#define CC_CK_G_Pin GPIO_PIN_9
#define CC_CK_G_GPIO_Port GPIOC
#define PC_SW_SIG_Pin GPIO_PIN_8
#define PC_SW_SIG_GPIO_Port GPIOA
#define FLU_A_G_Pin GPIO_PIN_11
#define FLU_A_G_GPIO_Port GPIOA
#define FLU_A_R_Pin GPIO_PIN_12
#define FLU_A_R_GPIO_Port GPIOA
#define PW_G_Pin GPIO_PIN_15
#define PW_G_GPIO_Port GPIOA
#define BAT_G1_Pin GPIO_PIN_10
#define BAT_G1_GPIO_Port GPIOC
#define BAT_G2_Pin GPIO_PIN_11
#define BAT_G2_GPIO_Port GPIOC
#define BAT_G3_Pin GPIO_PIN_12
#define BAT_G3_GPIO_Port GPIOC
#define BLE_G_Pin GPIO_PIN_2
#define BLE_G_GPIO_Port GPIOD
#define OPT_PD_SEL1_Pin GPIO_PIN_3
#define OPT_PD_SEL1_GPIO_Port GPIOB
#define OPT_PD_SEL2_Pin GPIO_PIN_4
#define OPT_PD_SEL2_GPIO_Port GPIOB
#define OPT_PD_SEL3_Pin GPIO_PIN_5
#define OPT_PD_SEL3_GPIO_Port GPIOB
#define FLU_B_G_Pin GPIO_PIN_8
#define FLU_B_G_GPIO_Port GPIOB
#define FLU_B_R_Pin GPIO_PIN_9
#define FLU_B_R_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
