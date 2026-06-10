
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
#include "IDDD_TRF_Timer_CTRL.h"

//HW
//#include "PP_BD_v2_0_HW.h"

//Define 



/* Extern --------------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

//OS Var


//Normal Var


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
int32_t IDDD_TRF_TImer_Config(uint32_t dwLED_ON_Time_us, uint32_t dwPD_ADC_Scan_Time_us)
{
  TIM_HandleTypeDef *p_HAL_Time;
  
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  
  int32_t dwCheck = 0;
  
  p_HAL_Time = Read_TRF_Timer_HalDrive();
  
  p_HAL_Time->Instance = TIM3;
  p_HAL_Time->Init.Prescaler = 55;
  p_HAL_Time->Init.CounterMode = TIM_COUNTERMODE_UP;
  p_HAL_Time->Init.Period = 9999;
  p_HAL_Time->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  p_HAL_Time->Init.RepetitionCounter = 0;
  p_HAL_Time->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  dwCheck = HAL_TIM_Base_Init(p_HAL_Time);
  if(dwCheck) return dwCheck;
    
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  dwCheck = HAL_TIM_ConfigClockSource(p_HAL_Time, &sClockSourceConfig);
  if(dwCheck) return dwCheck;
  
  dwCheck = HAL_TIM_PWM_Init(p_HAL_Time);
  if(dwCheck) return dwCheck;
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  dwCheck = HAL_TIMEx_MasterConfigSynchronization(p_HAL_Time, &sMasterConfig);
  if(dwCheck) return dwCheck;
  
  //PWM CH1 Init
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = dwLED_ON_Time_us;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCNPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  dwCheck = HAL_TIM_PWM_ConfigChannel(p_HAL_Time, &sConfigOC, TIM_CHANNEL_1);
  if(dwCheck) return dwCheck;
  
  //PWM CH2 Init
  sConfigOC.Pulse = dwPD_ADC_Scan_Time_us;
  dwCheck = HAL_TIM_PWM_ConfigChannel(p_HAL_Time, &sConfigOC, TIM_CHANNEL_2);
  if(dwCheck) return dwCheck;
  
  /* Stage0: PA6/PA7 are GPIO outputs, TIM3 PWM AF PostInit no longer required */
  /* HAL_TIM_MspPostInit(p_HAL_Time); */
  
  return dwCheck;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void IDDD_TRF_PWM_Start(void)
{
  TIM_HandleTypeDef *p_HAL_Time;
  
  p_HAL_Time = Read_TRF_Timer_HalDrive();
  
  TIM3->CNT = 0;
  
  __HAL_TIM_MOE_ENABLE(p_HAL_Time);
  __HAL_TIM_ENABLE(p_HAL_Time);
  
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void IDDD_TRF_PWM_Stop(void)
{
  TIM3->CNT = 0;
  TIM3->BDTR &= ~(TIM_BDTR_MOE);
  TIM3->CR1 &= ~(TIM_CR1_CEN);  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void IDDD_TRF_PWM_Channel_Enanle(void)
{
  TIM_CCxChannelCmd(TIM3, TIM_CHANNEL_1, TIM_CCx_ENABLE);
  TIM_CCxChannelCmd(TIM3, TIM_CHANNEL_2, TIM_CCx_ENABLE);
}


/**
  * @brief  Stage1 measurement timer: free-running 1us up-counter on TIM3.
  *         APB1 timer clock 56MHz / (55+1) = 1MHz -> 1 tick = 1us.
  * @retval None
  */
void IDDD_TRF_MeasTimer_Init(void)
{
  TIM_HandleTypeDef *p_HAL_Time;

  p_HAL_Time = Read_TRF_Timer_HalDrive();

  // 1) reconfigure TIM3 as a plain 16-bit free-running 1us counter
  p_HAL_Time->Instance = TIM3;
  p_HAL_Time->Init.Prescaler = 55;
  p_HAL_Time->Init.CounterMode = TIM_COUNTERMODE_UP;
  p_HAL_Time->Init.Period = 0xFFFF;
  p_HAL_Time->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  p_HAL_Time->Init.RepetitionCounter = 0;
  p_HAL_Time->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_Base_Init(p_HAL_Time);

  // 2) start free-running
  TIM3->CNT = 0;
  TIM3->CR1 |= TIM_CR1_CEN;
}


/**
  * @brief  Stop the Stage1 measurement free-running counter.
  * @retval None
  */
void IDDD_TRF_MeasTimer_Stop(void)
{
  TIM3->CR1 &= ~(TIM_CR1_CEN);
  TIM3->CNT = 0;
}


/**
  * @brief  Overflow-safe elapsed time between two 16-bit TIM3 CNT samples.
  * @param  dwStartCnt: CNT captured at start
  * @param  dwEndCnt:   CNT captured at end
  * @retval Elapsed microseconds (single wrap-around safe)
  */
uint32_t IDDD_TRF_Timer_DiffUs(uint32_t dwStartCnt, uint32_t dwEndCnt)
{
  return (uint32_t)((dwEndCnt - dwStartCnt) & 0xFFFFU);
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
