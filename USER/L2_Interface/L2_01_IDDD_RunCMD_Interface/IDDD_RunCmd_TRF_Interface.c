
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

/* Stage1 measurement (runtime profile) */
#define TRF_MEAS_SAMPLE_COUNT            100U   /* ADC samples per DMA burst */
#define TRF_MEAS_REPEAT_COUNT            100U   /* repeated bursts for statistics */
#define TRF_MEAS_TIMEOUT_MS              100U   /* per-burst completion timeout */

/* 실측 검증 (real measurement) — Stage2 TIM-CNT polling */
#define TRF_REAL_SAMPLE_COUNT           100U   /* samples across 0~2000us */
#define TRF_REAL_SAMPLE_INTERVAL_US      20U   /* uniform interval */
#define TRF_REAL_LED_ON_US              200U   /* TIM3 CNT threshold: LED ON */
#define TRF_REAL_LED_OFF_US            1200U   /* TIM3 CNT threshold: LED OFF */
#define TRF_REAL_DARK_COUNT              10U   /* first N samples treated as dark */
#define TRF_REAL_LED_CURRENT            500U   /* LED drive current (unit: x100uA) */
#define TRF_REAL_DMA_TIMEOUT_MS         100U   /* kept for Stage1Meas path; not used in Stage2 */

/* Stage2 sample record */
typedef struct
{
  uint16_t tim_us;
  uint16_t adc_val;
} TRF_Sample_t;

//Typedef
/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//OS Val

//Other 
uint16_t g_wADC_Buf[TRF_READ_BUF_SZ];

/* Stage1 measurement DMA buffer (used by IDDD_RunCmd_TRF_Meas) */
uint16_t g_wTRF_MeasBuf[TRF_MEAS_SAMPLE_COUNT];

/* Stage2 poll buffer (400 bytes) */
TRF_Sample_t g_TRF_Samples[TRF_REAL_SAMPLE_COUNT];

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
  * @brief  Stage1 ADC timing measurement.
  *         Repeats a 100-sample SW-start/DMA burst and reports the per-burst
  *         total time (avg/min/max, us). A marker pulse on TRF_MEAS_MARK (PC1)
  *         brackets each burst for oscilloscope cross-check.
  *         Runtime-only: Stage0 path and CubeMX ADC config are not modified.
  * @retval 0 on success, error code otherwise
  */
int32_t IDDD_RunCmd_TRF_Meas(void)
{
  int32_t dwCheck = 0;
  uint32_t dwRep;
  uint32_t dwStartCnt, dwEndCnt, dwDeltaUs;
  uint32_t dwStartTickMs;
  uint32_t dwSum = 0;
  uint32_t dwMin = 0xFFFFFFFFU;
  uint32_t dwMax = 0;
  uint32_t dwCount = 0;
  uint32_t dwAdcAccSum = 0;       /* per-burst ADC avg 누적 */
  uint32_t dwAdcAccMin = 0xFFFFFFFFU;
  uint32_t dwAdcAccMax = 0;

  dwCheck = IDDD_PD_ADC_Lock();
  if(dwCheck) return dwCheck;

  // 1) Stage1 ADC: SW start + continuous + DMA continuous
  dwCheck = IDDD_PD_ADC_Config_Stage1Meas();
  if(dwCheck) goto TRF_MEAS_EXIT;

  IDDD_PD_ADC_Channel_Select(OPT_PD_ADC_CH1);

  // 2) TIM3 as free-running 1us counter
  IDDD_TRF_MeasTimer_Init();

  // 3) enter measurement mode (callback captures end CNT + done flag)
  IDDD_TRF_Meas_Mode_CTRL(SET);

  for(dwRep = 0; dwRep < TRF_MEAS_REPEAT_COUNT; dwRep++)
  {
    // 3.1) arm: clear done flag, latch start timestamp
    IDDD_TRF_Meas_Done_Flag_CTRL(RESET);
    TIM3->CNT = 0;
    dwStartCnt = TIM3->CNT;

    // 3.2) marker High at burst start
    HAL_GPIO_WritePin(TRF_MEAS_MARK_GPIO_Port, TRF_MEAS_MARK_Pin, GPIO_PIN_SET);

    // 3.3) launch 100-sample DMA burst
    dwCheck = IDDD_PD_ADC_DMA_Start(g_wTRF_MeasBuf, TRF_MEAS_SAMPLE_COUNT);
    if(dwCheck)
    {
      HAL_GPIO_WritePin(TRF_MEAS_MARK_GPIO_Port, TRF_MEAS_MARK_Pin, GPIO_PIN_RESET);
      goto TRF_MEAS_EXIT;
    }

    // 3.4) poll completion flag with timeout
    dwStartTickMs = HAL_GetTick();
    while(Read_IDDD_TRF_Meas_Done_Flag() == RESET)
    {
      if((HAL_GetTick() - dwStartTickMs) >= TRF_MEAS_TIMEOUT_MS)
      {
        dwCheck = DEV_CMPLT_TIMOUT;
        break;
      }
    }

    // 3.5) marker Low right after flag recognition (per design decision)
    HAL_GPIO_WritePin(TRF_MEAS_MARK_GPIO_Port, TRF_MEAS_MARK_Pin, GPIO_PIN_RESET);

    IDDD_PD_ADC_DMA_Stop();

    if(dwCheck) goto TRF_MEAS_EXIT;

    // 3.6) total burst time = end CNT - start CNT (overflow-safe, us)
    dwEndCnt = Read_IDDD_TRF_Meas_End_Cnt();
    dwDeltaUs = IDDD_TRF_Timer_DiffUs(dwStartCnt, dwEndCnt);

    dwSum += dwDeltaUs;
    if(dwDeltaUs < dwMin) dwMin = dwDeltaUs;
    if(dwDeltaUs > dwMax) dwMax = dwDeltaUs;
    dwCount++;

    // 3.7) per-burst ADC value statistics
    {
      uint32_t adcSum = 0;
      uint32_t adcMin = 0xFFFFU;
      uint32_t adcMax = 0;
      uint32_t i;
      for(i = 0; i < TRF_MEAS_SAMPLE_COUNT; i++)
      {
        uint32_t v = g_wTRF_MeasBuf[i];
        adcSum += v;
        if(v < adcMin) adcMin = v;
        if(v > adcMax) adcMax = v;
      }
      // per-burst average only (min/max are per-sample extremes)
      dwAdcAccSum += (adcSum / TRF_MEAS_SAMPLE_COUNT);
      if(adcMin < dwAdcAccMin) dwAdcAccMin = adcMin;
      if(adcMax > dwAdcAccMax) dwAdcAccMax = adcMax;
    }
  }

TRF_MEAS_EXIT:

  // 4) leave measurement mode and restore peripherals
  IDDD_TRF_Meas_Mode_CTRL(RESET);
  IDDD_PD_ADC_DMA_Stop();
  IDDD_TRF_MeasTimer_Stop();
  IDDD_PD_ADC_Unlock();
  IDDD_PD_ADC_Channel_Select(OPT_PD_ADC_CH_REG);

  // 5) fixed log output (unit: 100-sample burst total time, us)
  hsDebug_MSG("----- TRF MEAS (100-sample burst total, us) -----\n");
  if(dwCount > 0)
  {
    uint32_t dwAvg = dwSum / dwCount;
    hsDebug_MSG("time avg:%d min:%d max:%d count:%d\n", dwAvg, dwMin, dwMax, dwCount);
    hsDebug_MSG("adc  avg:%d min:%d max:%d\n",
                dwAdcAccSum / dwCount, dwAdcAccMin, dwAdcAccMax);
  }
  else
  {
    hsDebug_MSG("no valid sample (check[%d])\n", dwCheck);
  }

  return dwCheck;
}

/**
  * @brief  Stage2 실측: TIM3 CNT 폴링으로 0~2000us 전 구간 ADC 단발 샘플링.
  *         100 samples at uniform 20us interval.
  *         LED ON @200us, OFF @1200us.
  * @retval 0 on success
  */
int32_t IDDD_RunCmd_TRF_Real(void)
{
  int32_t dwCheck = 0;
  uint32_t dwDarkAvg = 0;
  uint32_t i;
  uint32_t dwTim;
  uint16_t wVal;
  uint32_t dwOptCh;
  uint32_t dwLedCurr;
  ADC_HandleTypeDef *pADC;

  // 1) optsel 채널 + LED 전류 ROM에서 읽기
  dwCheck = IDDD_OPT_OPT_Channel_SEL_Read_Interface(&dwOptCh);
  if(dwCheck) return dwCheck;

  dwCheck = IDDD_OPT_LED_Current_Read_Interface(dwOptCh, &dwLedCurr);
  if(dwCheck) dwLedCurr = TRF_REAL_LED_CURRENT;  /* ROM 읽기 실패 시 define 값 fallback */

  // 2) ADC Lock + Stage2 Poll 설정
  dwCheck = IDDD_PD_ADC_Lock();
  if(dwCheck) return dwCheck;

  dwCheck = IDDD_PD_ADC_Config_Stage1Poll();
  if(dwCheck) goto REAL_EXIT;

  IDDD_PD_ADC_Channel_Select(dwOptCh);

  // 3) 완전 OFF 상태로 시작
  IDDD_LED_Channel_Select(OPT_LED_CH_OFF);
  TRF_SequenceSignal_SetLow();

  // 4) ADC 핸들 확보 (단발 변환용)
  pADC = IDDD_PD_ADC_GetHandle();

  // 5) TIM3 free counter 시작 (1us tick)
  IDDD_TRF_MeasTimer_Init();
  TIM3->CNT = 0;

  // 6) LED 상태 추적 플래그
  {
    uint8_t bLedOn = 0;

    // 7) 100샘플 폴링 루프
    for(i = 0; i < TRF_REAL_SAMPLE_COUNT; i++)
    {
      uint32_t dwTarget = i * TRF_REAL_SAMPLE_INTERVAL_US;

      // 7a) 목표 시각까지 대기
      while(TIM3->CNT < dwTarget);

      // 7b) 타임스탬프 캡처
      dwTim = TIM3->CNT;

      // 7c) LED ON 조건: 200us 도달, 아직 안 켰으면
      if((dwTim >= TRF_REAL_LED_ON_US) && (bLedOn == 0))
      {
        IDDD_LED_Current_Set(dwLedCurr);
        IDDD_LED_Channel_Select(dwOptCh);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
        bLedOn = 1;
      }

      // 7d) LED OFF 조건: 1200us 도달, 아직 안 껐으면
      if((dwTim >= TRF_REAL_LED_OFF_US) && (bLedOn == 1))
      {
        TRF_SequenceSignal_SetLow();
        IDDD_LED_Channel_Select(OPT_LED_CH_OFF);
        bLedOn = 0;
      }

      // 7e) ADC 단발 변환
      dwCheck = HAL_ADC_Start(pADC);
      if(dwCheck) goto REAL_EXIT;

      dwCheck = HAL_ADC_PollForConversion(pADC, 10);
      if(dwCheck) { HAL_ADC_Stop(pADC); goto REAL_EXIT; }

      wVal = HAL_ADC_GetValue(pADC);
      HAL_ADC_Stop(pADC);

      // 6f) 저장
      g_TRF_Samples[i].tim_us  = (uint16_t)dwTim;
      g_TRF_Samples[i].adc_val = wVal;
    }
  }

  // 7) dark avg: 첫 10샘플
  {
    uint32_t sum = 0;
    for(i = 0; i < TRF_REAL_DARK_COUNT; i++) sum += g_TRF_Samples[i].adc_val;
    dwDarkAvg = sum / TRF_REAL_DARK_COUNT;
  }

  // 8) 일괄 출력
  hsDebug_MSG("----- TRF REAL Stage2 (TIM-CNT poll) -----\n");
  hsDebug_MSG("ch:%d cur:%d dark:%d samples:%d interval:%dus\n",
              dwOptCh, dwLedCurr, dwDarkAvg,
              TRF_REAL_SAMPLE_COUNT, TRF_REAL_SAMPLE_INTERVAL_US);
  for(i = 0; i < TRF_REAL_SAMPLE_COUNT; i++)
  {
    hsDebug_MSG("[t=%dus]%d ", g_TRF_Samples[i].tim_us, g_TRF_Samples[i].adc_val);
  }
  hsDebug_MSG("\n");

REAL_EXIT:

  TRF_SequenceSignal_SetLow();
  IDDD_LED_Channel_Select(OPT_LED_CH_OFF);
  IDDD_TRF_MeasTimer_Stop();
  IDDD_PD_ADC_Unlock();
  IDDD_PD_ADC_Channel_Select(OPT_PD_ADC_CH_REG);

  return dwCheck;
}

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
