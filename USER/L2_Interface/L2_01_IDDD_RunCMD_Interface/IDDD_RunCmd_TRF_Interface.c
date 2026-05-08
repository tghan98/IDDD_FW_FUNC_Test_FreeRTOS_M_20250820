
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

//Typedef
/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//OS Val

//Other 
uint16_t g_wADC_Buf[TRF_READ_BUF_SZ];

/* Private function prototypes -----------------------------------------------*/

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
  uint32_t dwReadFlag, dwCnt;
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
  IDDD_PD_ADC_Complete_Flag_CTRL(SET);
  
  dwCheck = IDDD_PD_ADC_DMA_Start(g_wADC_Buf, TRF_READ_BUF_SZ - 3); //Data Size 12 cnt
  if(dwCheck) goto RUNCMD_SYNCTRF_EXIT;
  
  IDDD_TRF_PWM_Start();
  
  dwCnt = 0;
  dwReadFlag = 0;
  
  while(1)
  {
    vTaskDelay(100);  // 10ms로 Delay를 주게되면 측정 주기가 짧아진다.
    
    if(dwCnt >= TRF_TIME_OUT_CNT)
    {
      dwCheck = DEV_CMPLT_TIMOUT;
      goto RUNCMD_SYNCTRF_EXIT;
    }
    
    dwReadFlag = Read_IDDD_PD_ADC_Complete_Flag();
    if(dwReadFlag == RESET) break;
    
    dwCnt++;
  }
  
  hsDebug_MSG("-----   TRF ADC ------\n");
  
  for(dwCnt = 1; dwCnt < (TRF_READ_BUF_SZ - 4) ;  dwCnt++)
  {
    dwADC_out[dwCnt] = g_wADC_Buf[dwCnt];
    
    hsDebug_MSG("Num[%d] : [%d]\n", dwCnt - 1, dwADC_out[dwCnt]);
  }
  
  
RUNCMD_SYNCTRF_EXIT:
  
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
