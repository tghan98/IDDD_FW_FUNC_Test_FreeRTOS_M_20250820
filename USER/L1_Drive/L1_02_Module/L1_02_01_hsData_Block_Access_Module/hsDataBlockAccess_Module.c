
/**
  ******************************************************************************
  * @file           : 
  * @brief          : 
  ******************************************************************************
  * 0 to 10 are reserved with special registers.
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
#include "hsDataBlockAccess_Module.h"



//CONST Define
#define HS_DBA_ACCESS_TIM_OUT           250

//Default

//Other


/* Extern --------------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsDBA_Module_Config(hsDBA_t *p_tDBA_Handle, uint16_t wDBA_Start, uint16_t wDBA_End, uint32_t *p_dwMemBlock)
{
  int32_t dwCheck = 0;
  
  if( (wDBA_Start >= wDBA_End) || (p_dwMemBlock == NULL))
  {
    dwCheck = DAT_ERR_PARAM_DATA;
  }
  else
  {
    p_tDBA_Handle->wAccess_Range[HS_DBA_ACCESS_RNG_START] = wDBA_Start;
    p_tDBA_Handle->wAccess_Range[HS_DBA_ACCESS_RNG_END] = wDBA_End;
    
    p_tDBA_Handle->dwDataQuantity = (wDBA_End - wDBA_Start) + 1;
    
    p_tDBA_Handle->dwDataMemSizeToByte = p_tDBA_Handle->dwDataQuantity * 4;
    
    p_tDBA_Handle->p_DataBlockMem = p_dwMemBlock;
    
    p_tDBA_Handle->xMutex_Access_Key = xSemaphoreCreateMutex();
    xSemaphoreGive(p_tDBA_Handle->xMutex_Access_Key);
    
  }
  
  return dwCheck;
}

//----------------------------------------------------------------------------

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsDBA_Read(hsDBA_t *p_tDBA_Handle, uint16_t wAccess_Address, uint32_t *p_RD_Mem)
{
  BaseType_t xToken;
  uint16_t wIndex;
  
  int32_t dwCheck = 0;
  
  if( (wAccess_Address < p_tDBA_Handle->wAccess_Range[HS_DBA_ACCESS_RNG_START]) ||
      (wAccess_Address > p_tDBA_Handle->wAccess_Range[HS_DBA_ACCESS_RNG_END] ) )
  {
    return DAT_ERR_PARAM_DATA;
  }
  
  xToken = xSemaphoreTake(p_tDBA_Handle->xMutex_Access_Key, HS_DBA_ACCESS_TIM_OUT);
  if(xToken == pdFALSE) return OS_ACCES_FAIL;
  
  
  wIndex = wAccess_Address - p_tDBA_Handle->wAccess_Range[HS_DBA_ACCESS_RNG_START];
  
  *p_RD_Mem = p_tDBA_Handle->p_DataBlockMem[wIndex];
  
  xSemaphoreGive(p_tDBA_Handle->xMutex_Access_Key);
    
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsDBA_Write(hsDBA_t *p_tDBA_Handle, uint16_t wAccess_Address, uint32_t dwWR_Data)
{
  BaseType_t xToken;
  uint16_t wIndex;
  
  int32_t dwCheck = 0;
  
  if( (wAccess_Address < p_tDBA_Handle->wAccess_Range[HS_DBA_ACCESS_RNG_START]) ||
      (wAccess_Address > p_tDBA_Handle->wAccess_Range[HS_DBA_ACCESS_RNG_END] ) )
  {
    return DAT_ERR_PARAM_DATA;
  }
  
  xToken = xSemaphoreTake(p_tDBA_Handle->xMutex_Access_Key, HS_DBA_ACCESS_TIM_OUT);
  if(xToken == pdFALSE) return OS_ACCES_FAIL;
  
  
  wIndex = wAccess_Address - p_tDBA_Handle->wAccess_Range[HS_DBA_ACCESS_RNG_START];
  
  p_tDBA_Handle->p_DataBlockMem[wIndex] = dwWR_Data;
  
  xSemaphoreGive(p_tDBA_Handle->xMutex_Access_Key);
    
  return dwCheck;
}
  
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsDBA_Block_Handling(hsDBA_t *p_tDBA_Handle, uint32_t dwAccess_ATT_ReadWrite, 
                             uint32_t dwStartAccessAddress, uint32_t dwAccessSize, uint32_t *p_dwAccessMem)
{
  
  int32_t dwCheck = 0;
  uint32_t dwCnt, dwCntMax, dwIndex;
  
  dwCntMax = dwStartAccessAddress + dwAccessSize;
  dwIndex = 0;
  
  switch(dwAccess_ATT_ReadWrite)
  {
  case HS_DBA_ACCESS_ATT_WR:
    
    for(dwCnt = dwStartAccessAddress ; dwCnt < dwCntMax ; dwCnt++)
    {
      dwCheck = hsDBA_Write(p_tDBA_Handle, dwCnt, p_dwAccessMem[dwIndex++]);
      if(dwCheck) return dwCheck;
    }
    
    break;
    
  case HS_DBA_ACCESS_ATT_RD:
    
    for(dwCnt = dwStartAccessAddress ; dwCnt < dwCntMax ; dwCnt++)
    {
      dwCheck = hsDBA_Read(p_tDBA_Handle, dwCnt, &(p_dwAccessMem[dwIndex++]));
      if(dwCheck) return dwCheck;
    }
    
    break;
    
  default:
    dwCheck = DAT_ERR_PARAM_DATA;
    break;
  }
  
  return dwCheck;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsDBA_Data_Quantity_Read(hsDBA_t *p_tDBA_Handle, uint32_t *p_dwReadQuantity)
{
  BaseType_t xToken;
  int32_t dwCheck = 0;
  
  
  xToken = xSemaphoreTake(p_tDBA_Handle->xMutex_Access_Key, HS_DBA_ACCESS_TIM_OUT);
  if(xToken == pdFALSE) return OS_ACCES_FAIL;
  
  *p_dwReadQuantity = p_tDBA_Handle->dwDataQuantity;
  
  xSemaphoreGive(p_tDBA_Handle->xMutex_Access_Key);
    
  return dwCheck;
  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsDBA_DataBlockSizeByte_Read(hsDBA_t *p_tDBA_Handle, uint32_t *p_dwReadMemSZByte)
{
  BaseType_t xToken;
  int32_t dwCheck = 0;

  xToken = xSemaphoreTake(p_tDBA_Handle->xMutex_Access_Key, HS_DBA_ACCESS_TIM_OUT);
  if(xToken == pdFALSE) return OS_ACCES_FAIL;
  
  *p_dwReadMemSZByte = p_tDBA_Handle->dwDataMemSizeToByte;
  
  xSemaphoreGive(p_tDBA_Handle->xMutex_Access_Key);
    
  return dwCheck;
}
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
