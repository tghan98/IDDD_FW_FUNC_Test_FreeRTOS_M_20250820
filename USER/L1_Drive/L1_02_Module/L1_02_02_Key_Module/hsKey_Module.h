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
#ifndef __HS_KEY_MODULE_H__
#define __HS_KEY_MODULE_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
    
//------------------------------------------    
enum HS_KEY_RUN_STA
{
  HS_KEY_STA_NON_ACT = 0,
  HS_KEY_STA_SHORT_ACT,
  HS_KEY_STA_LONG_ACT,
  
  HS_KEY_STA_MAX
};
    
//------------------------------------------


//function
int32_t hsKey_Module_Config(uint32_t dwFp_PushCheckFunc);

int32_t hsKey_ActiveCheck(void);
int32_t hsKey_ActiveReset(void);
int32_t hsKey_Status_Read(uint32_t *p_dwRead);

#endif /* __HS_KEY_MODULE_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
