
/**
  ******************************************************************************
  * @file           : 
  * @brief          : 
  ******************************************************************************
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
/* Includes ------------------------------------------------------------------*/
#include "IDDD_BAT_ADC_Drv.h"

//HW
//#include "PP_BD_v2_0_HW.h"

//Define 
#define IDDD_BAT_ADC_ACCESS_TIM_OUT             250


/* Extern --------------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

//OS Var


//Normal Var
Rework_PP_Drv_t *gp_IDDD_BAT_ADC_Rework_Drv;

/* Private function prototypes -----------------------------------------------*/



//Macro Function



/*Function -------------------------------------------------------------------*/

//----------------------------------------------------------------------------

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_BAT_ADC_Lock(void)
{
  int32_t dwCheck = 0;
  
  gp_IDDD_BAT_ADC_Rework_Drv = Read_Rework_PP_Drv(RWRK_ADC1_PP);
  
  dwCheck = Lock_Rework_PP_Drv(gp_IDDD_BAT_ADC_Rework_Drv, IDDD_BAT_ADC_ACCESS_TIM_OUT);
  if(dwCheck) return dwCheck;
  
  return dwCheck;
  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void IDDD_BAT_ADC_Unlock(void)
{
  if(gp_IDDD_BAT_ADC_Rework_Drv != NULL)
  {
    Unlock_Rework_PP_Drv(gp_IDDD_BAT_ADC_Rework_Drv);
  }
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_BAT_ADC_Config(void)
{
  ADC_HandleTypeDef *phadc1;
  ADC_ChannelConfTypeDef sConfig = {0};
  int32_t dwCheck = 0;

  gp_IDDD_BAT_ADC_Rework_Drv = Read_Rework_PP_Drv(RWRK_ADC1_PP);
  if(gp_IDDD_BAT_ADC_Rework_Drv == NULL) return RWRK_PP_SYS_ERR;

  phadc1 = (ADC_HandleTypeDef *)gp_IDDD_BAT_ADC_Rework_Drv->p_PP_Drv;

  // ADC1 기본 설정
  phadc1->Instance = ADC1;
  phadc1->Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  phadc1->Init.Resolution = ADC_RESOLUTION_12B;
  phadc1->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  phadc1->Init.ScanConvMode = ADC_SCAN_DISABLE;
  phadc1->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  phadc1->Init.LowPowerAutoWait = DISABLE;
  phadc1->Init.LowPowerAutoPowerOff = DISABLE;
  phadc1->Init.ContinuousConvMode = DISABLE;
  phadc1->Init.NbrOfConversion = 1;
  phadc1->Init.DiscontinuousConvMode = DISABLE;
  phadc1->Init.ExternalTrigConv = ADC_SOFTWARE_START;  // 소프트웨어 트리거
  phadc1->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  phadc1->Init.DMAContinuousRequests = DISABLE;
  phadc1->Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  phadc1->Init.SamplingTimeCommon1 = ADC_SAMPLETIME_3CYCLES_5;
  phadc1->Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  phadc1->Init.OversamplingMode = DISABLE;
  phadc1->Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;

  dwCheck = HAL_ADC_Init(phadc1);
  if(dwCheck) return dwCheck;

  // 채널4(PA0) 설정
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  dwCheck = HAL_ADC_ConfigChannel(phadc1, &sConfig);
  if(dwCheck)
  {
      HAL_ADC_DeInit(phadc1);
      return dwCheck;
  }

  return dwCheck;  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_BAT_ADC_ReadLowFunc(uint16_t *p_wADC_Val)
{
  ADC_HandleTypeDef *phadc1;
  int32_t dwCheck = 0;

  if(gp_IDDD_BAT_ADC_Rework_Drv == NULL) return RWRK_PP_SYS_ERR;
  phadc1 = (ADC_HandleTypeDef *)gp_IDDD_BAT_ADC_Rework_Drv->p_PP_Drv;

  dwCheck = HAL_ADC_Start(phadc1);
  if(dwCheck) return dwCheck;

  dwCheck = HAL_ADC_PollForConversion(phadc1, HAL_MAX_DELAY);
  if (dwCheck) 
  {
      HAL_ADC_Stop(phadc1);
      return dwCheck;
  }

  *p_wADC_Val = HAL_ADC_GetValue(phadc1);

  dwCheck = HAL_ADC_Stop(phadc1);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_BAT_ADC_Read(uint16_t *p_wADC_Val)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_BAT_ADC_Lock();
  if(dwCheck) return dwCheck;
  
  dwCheck = IDDD_BAT_ADC_Config();
  if(dwCheck) goto IDDD_BAT_ADC_READ_EXIT;
  
  dwCheck = IDDD_BAT_ADC_ReadLowFunc(p_wADC_Val);
  if(dwCheck) goto IDDD_BAT_ADC_READ_EXIT;
  
IDDD_BAT_ADC_READ_EXIT:
  
  IDDD_BAT_ADC_Unlock();
  
  return dwCheck;
}




/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
