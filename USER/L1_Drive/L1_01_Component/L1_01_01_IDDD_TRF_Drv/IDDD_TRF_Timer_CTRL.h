/**
  ******************************************************************************
  * @file           : 
  * @brief          : 
  *                   
  ******************************************************************************
  ** 
  * 
  *
  *
  * 
  *
  *
  * 
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IDDD_TRF_TIMER_CTRL_DRV_H__
#define __IDDD_TRF_TIMER_CTRL_DRV_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

    
//Function ----------------------------------------------------

int32_t IDDD_TRF_TImer_Config(uint32_t dwLED_ON_Time_us, uint32_t dwPD_ADC_Scan_Time_us);

void IDDD_TRF_PWM_Start(void);

void IDDD_TRF_PWM_Stop(void);

void IDDD_TRF_PWM_Channel_Enanle(void);

//Stage1 measurement timer
void IDDD_TRF_MeasTimer_Init(void);

void IDDD_TRF_MeasTimer_Stop(void);

uint32_t IDDD_TRF_Timer_DiffUs(uint32_t dwStartCnt, uint32_t dwEndCnt);

#endif /* __IDDD_TRF_TIMER_CTRL_DRV_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
