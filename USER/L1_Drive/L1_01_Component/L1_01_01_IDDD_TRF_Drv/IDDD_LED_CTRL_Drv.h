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
#ifndef __IDDD_LED_CTRL_DRV_H__
#define __IDDD_LED_CTRL_DRV_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

    

enum OPT_LED_CH
{
  OPT_LED_CH1 = 0,
  OPT_LED_CH2,
  OPT_LED_CH3,
  OPT_LED_CH4,
  OPT_LED_CH5,
  OPT_LED_CH6,
  OPT_LED_CH7,
  OPT_LED_CH_OFF,
  
  OPT_LED_CH_MAX
};

//Function ----------------------------------------------------

void IDDD_LED_Channel_Select(uint32_t dwCH);

int32_t IDDD_LED_Current_Set(uint32_t dwCurrentVal_x100uA);

uint32_t Read_IDDD_LED_DAC_CountResult(void);




#endif /* __IDDD_LED_CTRL_DRV_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
