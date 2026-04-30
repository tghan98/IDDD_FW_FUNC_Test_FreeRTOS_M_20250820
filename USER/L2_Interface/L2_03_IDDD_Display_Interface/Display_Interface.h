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
#ifndef __DISP_IF_H__
#define __DISP_IF_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
    
//------------------------------------------    
enum BLE_STATUS
{
  staBLE_DISCON = 0,
  staBLE_CONEC,
  
  staBLE_MAX
};

enum BAT_STATUS
{
  staBAT_BLNK_1 = 0,
  staBAT_1,
  staBAT_2,
  staBAT_3,
  
  staBAT_MAX
};

enum DISP_STATUS
{
  staDISP_BASE = 0,
  
  staDISP_COD_IN,
  staDISP_CST_IN,
  staDISP_SMPL_IN,
  staDISP_INCU_RN,
  staDISP_RES_RN,
  
  staDISP_ERROR_RN,
  
  staDISP_MAX
};

enum RES_NEG_POS_STATUS
{
  staRES_NONE = 0,
  
  staRES_NEG,
  staRES_POS,
  
  staRES_MAX
};


//------------------------------------------


//function

int32_t IDDD_Disp_BLE_Status_Write(uint32_t dwStatus);
int32_t IDDD_Disp_BLE_Status_Read(uint32_t *p_dwStatus);

int32_t IDDD_Disp_BAT_Status_Write(uint32_t dwStatus);
int32_t IDDD_Disp_BAT_Status_Read(uint32_t *p_dwStatus);

int32_t IDDD_Disp_Status_Write(uint32_t dwStatus);
int32_t IDDD_Disp_Status_Read(uint32_t *p_dwStatus);

#endif /* __DISP_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
