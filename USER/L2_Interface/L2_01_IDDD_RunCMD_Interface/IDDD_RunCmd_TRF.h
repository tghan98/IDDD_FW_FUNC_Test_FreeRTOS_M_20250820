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
#ifndef __IDDD_RUNCMD_TRF_IF_H__
#define __IDDD_RUNCMD_TRF_IF_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
    
//------------------------------------------    

#define RUN_CMDDEBU_MSG         0

//-----------------------------------------


//------------------------------------------

//function
//Config
int32_t IDDD_RunCmd_TRF_Config(void);

//TRF Run Interface
int32_t IDDD_RunCmd_SyncTRF_Run(uint32_t dwOPT_Ch, uint32_t dwLED_ON_Tim, uint32_t dwADC_Trigger_Tim, uint32_t dwLEDCurrent);

//Stage1 measurement (temporary)
int32_t IDDD_RunCmd_TRF_Meas(void);

//실측 검증 (temporary)
int32_t IDDD_RunCmd_TRF_Real(void);

//실측 검증 2-cycle (skeleton, temporary)
int32_t IDDD_RunCmd_TRF_Real2Cycle(void);

//실측 검증 2-cycle (ON1 측정 후 100ms 대기 뒤 ON2 분리 측정)
int32_t IDDD_RunCmd_TRF_Real2Cycle_Gap100ms(void);

//Stage8 실측 버스트 (OFF+150us 100샘플 DMA 버스트)
int32_t IDDD_RunCmd_TRF_Real_Burst(void);

 
//DBA Interface
int32_t IDDD_OPT_LED_CurrentData_Write_Interface(uint32_t dwCh, uint32_t dwWriteData);
int32_t IDDD_OPT_LED_Current_Read_Interface(uint32_t dwCh, uint32_t *p_dwReadDataMem);
 
int32_t IDDD_OPT_LED_ON_Time_Write_Interface(uint32_t dwWriteData);
int32_t IDDD_OPT_LED_ON_Time_Read_Interface(uint32_t *p_dwReadDataMem);
 
int32_t IDDD_OPT_PD_Scan_Time_Write_Interface(uint32_t dwWriteData);
int32_t IDDD_OPT_PD_ScanTime_Read_Interface(uint32_t *p_dwReadDataMem);

int32_t IDDD_OPT_Channel_SEL_Write_Interface(uint32_t dwWriteData);
int32_t IDDD_OPT_OPT_Channel_SEL_Read_Interface(uint32_t *p_dwReadDataMem);

#if 1
int32_t IDDD_LED_Channel_SEL_Write_Interface(uint32_t dwWriteData);
int32_t IDDD_LED_Channel_SEL_Read_Interface(uint32_t *p_dwReadDataMem);

int32_t IDDD_PD_Channel_SEL_Write_Interface(uint32_t dwWriteData);
int32_t IDDD_PD_Channel_SEL_Read_Interface(uint32_t *p_dwReadDataMem);
#endif
    

#endif /* __IDDD_RUNCMD_TRF_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
