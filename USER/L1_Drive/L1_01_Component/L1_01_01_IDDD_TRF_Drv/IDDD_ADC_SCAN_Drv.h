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
#ifndef __IDDD_ADC_SCAN_DRV_H__
#define __IDDD_ADC_SCAN_DRV_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"


enum OPT_PD_ADC_CH
{
  OPT_PD_ADC_CH1 = 0,
  OPT_PD_ADC_CH2,
  OPT_PD_ADC_CH3,
  OPT_PD_ADC_CH4,
  OPT_PD_ADC_CH5,
  OPT_PD_ADC_CH6,
  OPT_PD_ADC_CH7,
  OPT_PD_ADC_CH_REG,
  
  OPT_PD_ADC_CH_MAX
};


//Function ----------------------------------------------------

int32_t IDDD_PD_ADC_Lock(void);

void IDDD_PD_ADC_Unlock(void);

int32_t IDDD_PD_ADC_Config(void);

int32_t IDDD_PD_ADC_Config_Stage1Meas(void);

void IDDD_PD_ADC_Channel_Select(uint32_t dwCH);

int32_t IDDD_PD_ADC_DMA_Start(uint16_t *p_DMA_Buff, uint32_t dwBuff_SZ);

int32_t IDDD_PD_ADC_DMA_Stop(void);

void IDDD_PD_ADC_Complete_Flag_CTRL(uint32_t dwSET_nRESET);

uint32_t Read_IDDD_PD_ADC_Complete_Flag(void);

//Stage1 measurement accessors
void IDDD_TRF_Meas_Mode_CTRL(uint32_t dwSET_nRESET);

void IDDD_TRF_Meas_Done_Flag_CTRL(uint32_t dwSET_nRESET);

uint32_t Read_IDDD_TRF_Meas_Done_Flag(void);

uint32_t Read_IDDD_TRF_Meas_End_Cnt(void);

#endif /* __IDDD_ADC_SCAN_DRV_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
