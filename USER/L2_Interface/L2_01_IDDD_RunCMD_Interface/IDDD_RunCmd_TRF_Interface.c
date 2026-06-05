
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
#include "IDDD_RunCmd_TRF.h"

//HW
#include "IDDD_ADC_SCAN_Drv.h"
#include "IDDD_LED_CTRL_Drv.h"
#include "IDDD_TRF_Timer_CTRL.h"

//Interface & Handle
#include "IDDD_DBA_ROM_Interface.h"
#include "IDDD_DBA_RAM_Interface.h"


//-----------------------------------------------------------------------------
//define 

//OS Config

//Other

#define TRF_READ_BUF_SZ         15
#define TRF_TIME_OUT_CNT        30 //100 msec x 20 =  2 sec 

/* Timing profile for software GPIO sequence (unit: us) */
#define TRF_TIMING_PROFILE_LED_ON_US            1000U
#define TRF_TIMING_PROFILE_RESIDUAL_WAIT_US      200U
#define TRF_TIMING_PROFILE_ADC_WINDOW_US         700U
#define TRF_TIMING_PROFILE_STABILIZE_US          100U
#define TRF_TIMING_PROFILE_CYCLE_US             2000U

#define TRF_RUN_TIMEOUT_MS               (TRF_TIME_OUT_CNT * 100U)

//Typedef
/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//OS Val

//Other 
uint16_t g_wADC_Buf[TRF_READ_BUF_SZ];

/* Private function prototypes -----------------------------------------------*/
static int32_t TRF_SequenceTimingProfile_Validate(void);
static void TRF_SequenceTimer_Start(void);
static void TRF_SequenceTimer_Stop(void);
static void TRF_SequenceSignal_SetLow(void);
static void TRF_SequenceWaitUntilUs(uint32_t dwTargetTimeUs);
static void TRF_RunOneSequenceCycle(void);

/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_RunCmd_TRF_Config(void)
{
  int32_t dwCheck = 0;
  
  IDDD_LED_Channel_Select(OPT_LED_CH_OFF);
  IDDD_PD_ADC_Channel_Select(OPT_PD_ADC_CH_REG);
    
  return dwCheck;
}

static int32_t TRF_SequenceTimingProfile_Validate(void)
{
  uint32_t dwActiveSumUs;

  dwActiveSumUs = TRF_TIMING_PROFILE_LED_ON_US
                + TRF_TIMING_PROFILE_RESIDUAL_WAIT_US
                + TRF_TIMING_PROFILE_ADC_WINDOW_US
                + TRF_TIMING_PROFILE_STABILIZE_US;

  if(dwActiveSumUs > TRF_TIMING_PROFILE_CYCLE_US) return DAT_ERR_PARAM_DATA;

  /* TIM3 period is 9999 in current profile, keep cycle within 1 counter lap */
  if(TRF_TIMING_PROFILE_CYCLE_US > 9999U) return DAT_ERR_PARAM_DATA;

  return RETURN_OK;
}

static void TRF_SequenceTimer_Start(void)
{
  TIM3->CNT = 0;
  TIM3->CR1 |= TIM_CR1_CEN;
}

static void TRF_SequenceTimer_Stop(void)
{
  TIM3->CR1 &= ~(TIM_CR1_CEN);
  TIM3->CNT = 0;
}

static void TRF_SequenceSignal_SetLow(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
}

static void TRF_SequenceWaitUntilUs(uint32_t dwTargetTimeUs)
{
  while(Read_IDDD_PD_ADC_Complete_Flag() == SET)
  {
    if(TIM3->CNT >= dwTargetTimeUs) break;
  }
}

static void TRF_RunOneSequenceCycle(void)
{
  uint32_t phase_led_on_end_us;
  uint32_t phase_residual_wait_end_us;
  uint32_t phase_adc_window_end_us;
  uint32_t phase_stabilize_end_us;
  uint32_t phase_cycle_end_us;

  phase_led_on_end_us = TRF_TIMING_PROFILE_LED_ON_US;
  phase_residual_wait_end_us = phase_led_on_end_us + TRF_TIMING_PROFILE_RESIDUAL_WAIT_US;
  phase_adc_window_end_us = phase_residual_wait_end_us + TRF_TIMING_PROFILE_ADC_WINDOW_US;
  phase_stabilize_end_us = phase_adc_window_end_us + TRF_TIMING_PROFILE_STABILIZE_US;
  phase_cycle_end_us = TRF_TIMING_PROFILE_CYCLE_US;

  TRF_SequenceSignal_SetLow();
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);

  TRF_SequenceTimer_Start();

  /* LED ON */
  TRF_SequenceWaitUntilUs(phase_led_on_end_us);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
  if(Read_IDDD_PD_ADC_Complete_Flag() == RESET) goto RUNCYCLE_EXIT;

  /* Residual wait */
  TRF_SequenceWaitUntilUs(phase_residual_wait_end_us);
  if(Read_IDDD_PD_ADC_Complete_Flag() == RESET) goto RUNCYCLE_EXIT;

  /* ADC window start: rising edge on PA7 */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
  TRF_SequenceWaitUntilUs(phase_adc_window_end_us);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
  if(Read_IDDD_PD_ADC_Complete_Flag() == RESET) goto RUNCYCLE_EXIT;

  /* Stabilize and cycle tail */
  TRF_SequenceWaitUntilUs(phase_stabilize_end_us);
  if(Read_IDDD_PD_ADC_Complete_Flag() == RESET) goto RUNCYCLE_EXIT;

  TRF_SequenceWaitUntilUs(phase_cycle_end_us);

RUNCYCLE_EXIT:
  TRF_SequenceTimer_Stop();
  TRF_SequenceSignal_SetLow();
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
  * @retval None pdTRUE 
  */


//-----------------------------------------------------------------------------
//TRF Run Interface

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_RunCmd_SyncTRF_Run(uint32_t dwOPT_Ch, uint32_t dwLED_ON_Tim, uint32_t dwADC_Trigger_Tim, uint32_t dwLEDCurrent)
{
  int32_t dwCheck = 0;
  uint32_t dwCnt, dwStartTickMs;
  uint32_t dwADC_out[15];
  
  dwCheck = IDDD_PD_ADC_Lock();
  if(dwCheck) return dwCheck;
  
  //TRF Config ---------------------------------------
  dwCheck = IDDD_PD_ADC_Config();
  if(dwCheck) goto RUNCMD_SYNCTRF_EXIT;
  
  dwCheck = IDDD_TRF_TImer_Config(dwLED_ON_Tim, dwADC_Trigger_Tim);
  if(dwCheck) goto RUNCMD_SYNCTRF_EXIT;
  IDDD_TRF_PWM_Channel_Enanle();
  
  dwCheck = IDDD_LED_Current_Set(dwLEDCurrent);
  if(dwCheck) goto RUNCMD_SYNCTRF_EXIT;
  
  IDDD_LED_Channel_Select(dwOPT_Ch);
  IDDD_PD_ADC_Channel_Select(dwOPT_Ch);
    
  vTaskDelay(50);
  
  
  //TRF Run ----------------------------------
  dwCheck = TRF_SequenceTimingProfile_Validate();
  if(dwCheck) goto RUNCMD_SYNCTRF_EXIT;

  IDDD_PD_ADC_Complete_Flag_CTRL(SET);
  
  dwCheck = IDDD_PD_ADC_DMA_Start(g_wADC_Buf, TRF_READ_BUF_SZ - 3); //Data Size 12 cnt
  if(dwCheck) goto RUNCMD_SYNCTRF_EXIT;

  dwStartTickMs = HAL_GetTick();

  while(Read_IDDD_PD_ADC_Complete_Flag() == SET)
  {
    if((HAL_GetTick() - dwStartTickMs) >= TRF_RUN_TIMEOUT_MS)
    {
      dwCheck = DEV_CMPLT_TIMOUT;
      goto RUNCMD_SYNCTRF_EXIT;
    }

    TRF_RunOneSequenceCycle();
  }
  
  hsDebug_MSG("-----   TRF ADC ------\n");
  
  for(dwCnt = 1; dwCnt < (TRF_READ_BUF_SZ - 4) ;  dwCnt++)
  {
    dwADC_out[dwCnt] = g_wADC_Buf[dwCnt];
    
    hsDebug_MSG("Num[%d] : [%d]\n", dwCnt - 1, dwADC_out[dwCnt]);
  }
  
  
RUNCMD_SYNCTRF_EXIT:

  TRF_SequenceSignal_SetLow();
  
  IDDD_PD_ADC_DMA_Stop();
  IDDD_TRF_PWM_Stop();
  
  IDDD_PD_ADC_Unlock();
  IDDD_PD_ADC_Channel_Select(OPT_PD_ADC_CH_REG);
  IDDD_LED_Channel_Select(OPT_LED_CH_OFF);
  
  return dwCheck;
}

//g_wADC_Buf

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
#if 0
int32_t IDDD_RunCmd_AnsyncTRF_Run(uint32_t dwLED_Ch, uint32_t dwPD_Ch, uint32_t dwLED_ON_Tim, uint32_t dwADC_Trigger_Tim)
{
  int32_t dwCheck = 0;
  
  
  return dwCheck;
}
#endif

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

//----------------------------------------------------------------------------
//DBA Interface Function
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_OPT_LED_CurrentData_Write_Interface(uint32_t dwCh, uint32_t dwWriteData)
{
  int32_t dwCheck = 0;
  uint32_t dwRegAddress = IDDD_DBA_REG_ROM_LED1;
  
  dwRegAddress = dwRegAddress + dwCh;
  
  dwCheck = IDDD_DBA_REG_ROM_Single_Write(dwRegAddress, dwWriteData);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_OPT_LED_Current_Read_Interface(uint32_t dwCh, uint32_t *p_dwReadDataMem)
{
  int32_t dwCheck = 0;
  uint32_t dwRegAddress = IDDD_DBA_REG_ROM_LED1;
  
  dwRegAddress = dwRegAddress + dwCh;
  
  dwCheck = IDDD_DBA_REG_ROM_Single_Read(dwRegAddress, p_dwReadDataMem);
  
  return dwCheck;
}
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_OPT_LED_ON_Time_Write_Interface(uint32_t dwWriteData)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_ROM_Single_Write(IDDD_DBA_REG_LED_ON_TIME, dwWriteData);
  
  return dwCheck;  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_OPT_LED_ON_Time_Read_Interface(uint32_t *p_dwReadDataMem)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_ROM_Single_Read(IDDD_DBA_REG_LED_ON_TIME, p_dwReadDataMem);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_OPT_PD_Scan_Time_Write_Interface(uint32_t dwWriteData)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_ROM_Single_Write(IDDD_DBA_REG_PD_SCAN_TIME, dwWriteData);
  
  return dwCheck;    
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_OPT_PD_ScanTime_Read_Interface(uint32_t *p_dwReadDataMem)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_ROM_Single_Read(IDDD_DBA_REG_PD_SCAN_TIME, p_dwReadDataMem);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_OPT_Channel_SEL_Write_Interface(uint32_t dwWriteData)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_RAM_Single_Write(IDDD_DBA_REG_ADDRESS_RAM_SEL_OPT_CH, dwWriteData);
  
  return dwCheck;    
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_OPT_OPT_Channel_SEL_Read_Interface(uint32_t *p_dwReadDataMem)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_RAM_Single_Read(IDDD_DBA_REG_ADDRESS_RAM_SEL_OPT_CH, p_dwReadDataMem);
  
  return dwCheck;
}


#if 1
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_LED_Channel_SEL_Write_Interface(uint32_t dwWriteData)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_RAM_Single_Write(IDDD_DBA_REG_ADDRESS_RAM_SEL_LED_CH, dwWriteData);
  
  return dwCheck;    
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_LED_Channel_SEL_Read_Interface(uint32_t *p_dwReadDataMem)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_RAM_Single_Read(IDDD_DBA_REG_ADDRESS_RAM_SEL_LED_CH, p_dwReadDataMem);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_PD_Channel_SEL_Write_Interface(uint32_t dwWriteData)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_RAM_Single_Write(IDDD_DBA_REG_ADDRESS_RAM_SEL_PD_CH, dwWriteData);
  
  return dwCheck;    
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_PD_Channel_SEL_Read_Interface(uint32_t *p_dwReadDataMem)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_RAM_Single_Read(IDDD_DBA_REG_ADDRESS_RAM_SEL_PD_CH, p_dwReadDataMem);
  
  return dwCheck;
}
#endif


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
