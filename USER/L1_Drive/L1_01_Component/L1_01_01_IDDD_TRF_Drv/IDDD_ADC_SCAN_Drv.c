
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
#include "IDDD_ADC_SCAN_Drv.h"

//HW
//#include "PP_BD_v2_0_HW.h"

//Define 
#define IDDD_PD_ADC_ACCESS_TIM_OUT      1000

/* Extern --------------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

//OS Var


//Normal Var
uint32_t g_dwIDDD_PD_ADC_Mode;
uint32_t g_dwIDDD_PD_ADC_CMP_Flag;
Rework_PP_Drv_t *gp_PD_ADC_Rwork_Drv;

/* Stage1 measurement (runtime profile) */
volatile uint32_t g_dwTRF_Meas_Mode;       /* SET: Stage1 measurement active */
volatile uint32_t g_dwTRF_Meas_Done_Flag;  /* SET: 100-sample DMA burst complete */
volatile uint32_t g_dwTRF_Meas_End_Cnt;    /* TIM3 CNT captured at TC callback */

/* Private function prototypes -----------------------------------------------*/



//Macro Function


/*Function -------------------------------------------------------------------*/
//-----------------------------------------------------------------------------
//ISR
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance != ADC1) return;

  // 1) Stage1 measurement path: capture end timestamp, signal main loop
  if(g_dwTRF_Meas_Mode)
  {
    g_dwTRF_Meas_End_Cnt = TIM3->CNT;
    g_dwTRF_Meas_Done_Flag = SET;
    return;
  }

  // 2) Stage0 stable path (external trigger sequence)
  if(g_dwIDDD_PD_ADC_Mode)
  {
    TIM3->CNT = 0;
    TIM3->BDTR &= ~(TIM_BDTR_MOE);
    TIM3->CR1 &= ~(TIM_CR1_CEN);

    g_dwIDDD_PD_ADC_CMP_Flag =  RESET;
  }
}


//----------------------------------------------------------------------------
//Normal


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_PD_ADC_Lock(void)
{
  int32_t dwCheck = 0;
  
  gp_PD_ADC_Rwork_Drv = Read_Rework_PP_Drv(RWRK_ADC1_PP);
  
  dwCheck = Lock_Rework_PP_Drv(gp_PD_ADC_Rwork_Drv, IDDD_PD_ADC_ACCESS_TIM_OUT);
  if(dwCheck) return dwCheck;
  
  g_dwIDDD_PD_ADC_Mode = SET;
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void IDDD_PD_ADC_Unlock(void)
{
  Unlock_Rework_PP_Drv(gp_PD_ADC_Rwork_Drv);
  g_dwIDDD_PD_ADC_Mode = RESET;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_PD_ADC_Config(void)
{
  ADC_HandleTypeDef *pHL_ADC;
  ADC_ChannelConfTypeDef sConfig = {0};
  
  int32_t dwCheck = 0;
  
  pHL_ADC = gp_PD_ADC_Rwork_Drv->p_PP_Drv;

/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  pHL_ADC->Instance = ADC1;
  pHL_ADC->Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  pHL_ADC->Init.Resolution = ADC_RESOLUTION_12B;
  pHL_ADC->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  pHL_ADC->Init.ScanConvMode = ADC_SCAN_DISABLE;
  pHL_ADC->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  pHL_ADC->Init.LowPowerAutoWait = DISABLE;
  pHL_ADC->Init.LowPowerAutoPowerOff = DISABLE;
  pHL_ADC->Init.ContinuousConvMode = DISABLE;
  pHL_ADC->Init.NbrOfConversion = 1;
  pHL_ADC->Init.DiscontinuousConvMode = DISABLE;
  pHL_ADC->Init.ExternalTrigConv = ADC_EXTERNALTRIG_EXT_IT11;
  pHL_ADC->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  pHL_ADC->Init.DMAContinuousRequests = DISABLE;
  pHL_ADC->Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  pHL_ADC->Init.SamplingTimeCommon1 = ADC_SAMPLETIME_3CYCLES_5;
  pHL_ADC->Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  pHL_ADC->Init.OversamplingMode = DISABLE;
  pHL_ADC->Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  
  dwCheck = HAL_ADC_Init(pHL_ADC);
  if(dwCheck) return dwCheck;

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  dwCheck = HAL_ADC_ConfigChannel(pHL_ADC, &sConfig);
  
  return dwCheck;
}

/**
  * @brief  Stage1 measurement ADC profile: SW start + continuous + DMA continuous.
  *         Runtime-only reconfiguration. Stage0 config (IDDD_PD_ADC_Config) is untouched.
  * @retval HAL status (0 == OK)
  */
int32_t IDDD_PD_ADC_Config_Stage1Meas(void)
{
  ADC_HandleTypeDef *pHL_ADC;
  ADC_ChannelConfTypeDef sConfig = {0};

  int32_t dwCheck = 0;

  pHL_ADC = gp_PD_ADC_Rwork_Drv->p_PP_Drv;

  // 1) global features: continuous conversion driven by software start
  pHL_ADC->Instance = ADC1;
  pHL_ADC->Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  pHL_ADC->Init.Resolution = ADC_RESOLUTION_12B;
  pHL_ADC->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  pHL_ADC->Init.ScanConvMode = ADC_SCAN_DISABLE;
  pHL_ADC->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  pHL_ADC->Init.LowPowerAutoWait = DISABLE;
  pHL_ADC->Init.LowPowerAutoPowerOff = DISABLE;
  pHL_ADC->Init.ContinuousConvMode = ENABLE;
  pHL_ADC->Init.NbrOfConversion = 1;
  pHL_ADC->Init.DiscontinuousConvMode = DISABLE;
  pHL_ADC->Init.ExternalTrigConv = ADC_SOFTWARE_START;
  pHL_ADC->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  pHL_ADC->Init.DMAContinuousRequests = ENABLE;
  pHL_ADC->Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  pHL_ADC->Init.SamplingTimeCommon1 = ADC_SAMPLETIME_79CYCLES_5;  /* final */
  pHL_ADC->Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
  pHL_ADC->Init.OversamplingMode = DISABLE;
  pHL_ADC->Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;

  dwCheck = HAL_ADC_Init(pHL_ADC);
  if(dwCheck) return dwCheck;

  // 2) regular channel (same physical channel as Stage0)
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  dwCheck = HAL_ADC_ConfigChannel(pHL_ADC, &sConfig);

  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void IDDD_PD_ADC_Channel_Select(uint32_t dwCH)
{
  if(dwCH >= OPT_PD_ADC_CH_REG) dwCH = OPT_PD_ADC_CH_REG;
  
  switch(dwCH)
  {
  case OPT_PD_ADC_CH1:
    
    OPT_PD_SEL1_GPIO_Port->BRR = (uint32_t)OPT_PD_SEL1_Pin;
    OPT_PD_SEL2_GPIO_Port->BRR = (uint32_t)OPT_PD_SEL2_Pin;
    OPT_PD_SEL3_GPIO_Port->BRR = (uint32_t)OPT_PD_SEL3_Pin;
    
    break;
  case OPT_PD_ADC_CH2:

    OPT_PD_SEL1_GPIO_Port->BSRR = (uint32_t)OPT_PD_SEL1_Pin;
    OPT_PD_SEL2_GPIO_Port->BRR = (uint32_t)OPT_PD_SEL2_Pin;
    OPT_PD_SEL3_GPIO_Port->BRR = (uint32_t)OPT_PD_SEL3_Pin;

    break;
  case OPT_PD_ADC_CH3:

    OPT_PD_SEL1_GPIO_Port->BRR = (uint32_t)OPT_PD_SEL1_Pin;
    OPT_PD_SEL2_GPIO_Port->BSRR = (uint32_t)OPT_PD_SEL2_Pin;
    OPT_PD_SEL3_GPIO_Port->BRR = (uint32_t)OPT_PD_SEL3_Pin;

    break;
  case OPT_PD_ADC_CH4:

    OPT_PD_SEL1_GPIO_Port->BSRR = (uint32_t)OPT_PD_SEL1_Pin;
    OPT_PD_SEL2_GPIO_Port->BSRR = (uint32_t)OPT_PD_SEL2_Pin;
    OPT_PD_SEL3_GPIO_Port->BRR = (uint32_t)OPT_PD_SEL3_Pin;

    break;
  case OPT_PD_ADC_CH5:

    OPT_PD_SEL1_GPIO_Port->BRR = (uint32_t)OPT_PD_SEL1_Pin;
    OPT_PD_SEL2_GPIO_Port->BRR = (uint32_t)OPT_PD_SEL2_Pin;
    OPT_PD_SEL3_GPIO_Port->BSRR = (uint32_t)OPT_PD_SEL3_Pin;

    break;
  case OPT_PD_ADC_CH6:

    OPT_PD_SEL1_GPIO_Port->BSRR = (uint32_t)OPT_PD_SEL1_Pin;
    OPT_PD_SEL2_GPIO_Port->BRR = (uint32_t)OPT_PD_SEL2_Pin;
    OPT_PD_SEL3_GPIO_Port->BSRR = (uint32_t)OPT_PD_SEL3_Pin;

    break;
  case OPT_PD_ADC_CH7:

    OPT_PD_SEL1_GPIO_Port->BRR = (uint32_t)OPT_PD_SEL1_Pin;
    OPT_PD_SEL2_GPIO_Port->BSRR = (uint32_t)OPT_PD_SEL2_Pin;
    OPT_PD_SEL3_GPIO_Port->BSRR = (uint32_t)OPT_PD_SEL3_Pin;

    break;
  default:

    OPT_PD_SEL1_GPIO_Port->BSRR = (uint32_t)OPT_PD_SEL1_Pin;
    OPT_PD_SEL2_GPIO_Port->BSRR = (uint32_t)OPT_PD_SEL2_Pin;
    OPT_PD_SEL3_GPIO_Port->BSRR = (uint32_t)OPT_PD_SEL3_Pin;

    break;
  }
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_PD_ADC_DMA_Start(uint16_t *p_DMA_Buff, uint32_t dwBuff_SZ)
{
  int32_t dwCheck = 0;
  
  dwCheck = HAL_ADC_Start_DMA( gp_PD_ADC_Rwork_Drv->p_PP_Drv, (uint32_t *)p_DMA_Buff, dwBuff_SZ);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_PD_ADC_DMA_Stop(void)
{
  int32_t dwCheck = 0;
  
  dwCheck = HAL_ADC_Stop_DMA(gp_PD_ADC_Rwork_Drv->p_PP_Drv);
  
  return dwCheck;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void IDDD_PD_ADC_Complete_Flag_CTRL(uint32_t dwSET_nRESET)
{
  g_dwIDDD_PD_ADC_CMP_Flag = dwSET_nRESET;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
uint32_t Read_IDDD_PD_ADC_Complete_Flag(void)
{
  return g_dwIDDD_PD_ADC_CMP_Flag;
}

/* -------------------------------------------------------------------------- */
/* Stage1 measurement accessors                                               */
/* -------------------------------------------------------------------------- */

void IDDD_TRF_Meas_Mode_CTRL(uint32_t dwSET_nRESET)
{
  g_dwTRF_Meas_Mode = dwSET_nRESET;
}

void IDDD_TRF_Meas_Done_Flag_CTRL(uint32_t dwSET_nRESET)
{
  g_dwTRF_Meas_Done_Flag = dwSET_nRESET;
}

uint32_t Read_IDDD_TRF_Meas_Done_Flag(void)
{
  return g_dwTRF_Meas_Done_Flag;
}

uint32_t Read_IDDD_TRF_Meas_End_Cnt(void)
{
  return g_dwTRF_Meas_End_Cnt;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
