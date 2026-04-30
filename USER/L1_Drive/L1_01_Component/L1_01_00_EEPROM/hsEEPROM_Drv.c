
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
#include "hsEEPROM_Drv.h"

#include "port_hsEEPROM_Drv.h"

//HW
//#include "PP_BD_v2_0_HW.h"

//Define 

#define hsEEPROM_ACCESS_WAIT_TIME_MS            500
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
int32_t hsEEROM_DRV_Config(hsEEPROM_DRV_Type *p_tDRV, uint32_t dwDev_ID, 
                        uint8_t uROM_DEV_Address, uint32_t dwPage_Size, uint32_t dwROM_MAX_Size, uint32_t dwROM_Add_Size, uint32_t dwPage_WR_Delay)
{
  //Remapping
  //p_tDRV->p_Remapping_Dev_Handle = Read_MCU_Dev_ReMapping_Address(dwDev_ID);
  p_tDRV->p_Rework_PP_Drv = Read_Rework_PP_Drv(dwDev_ID);
  p_tDRV->dwDev_ID = dwDev_ID;
  
  //ROM Dev Infor
  p_tDRV->uROM_DEV_Address = uROM_DEV_Address;
  p_tDRV->dwROM_Page_Size = dwPage_Size;
  p_tDRV->dwROM_Max_Size = dwROM_MAX_Size;
  p_tDRV->dwROM_Address_Size = dwROM_Add_Size;
  p_tDRV->dwROM_Page_Wrie_Delay_MS = dwPage_WR_Delay;
  
  //OS
  
  //Control
  p_tDRV->p_uBuf = (uint8_t *)pvPortMalloc(dwPage_Size);
  if(p_tDRV->p_uBuf == NULL) return OS_SYS_ERR;
  
  return 0;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsEEROM_Write(hsEEPROM_DRV_Type *p_tDRV, uint16_t wStore_StartAddress, uint8_t *p_uWrite, uint16_t wWrite_Size)
{
  //BaseType_t xToken;
  int32_t dwCheck;
  
  dwCheck = Lock_Rework_PP_Drv(p_tDRV->p_Rework_PP_Drv, hsEEPROM_ACCESS_WAIT_TIME_MS);
  if(dwCheck) return dwCheck;
  
  p_tDRV->dwRealROM_Address_Cnt_Max = wStore_StartAddress + wWrite_Size;
  if(p_tDRV->dwRealROM_Address_Cnt_Max >= p_tDRV->dwROM_Max_Size)
  {
    p_tDRV->dwRealROM_Address_Cnt_Max = p_tDRV->dwROM_Max_Size;
  }
  
  p_tDRV->dwRealROM_Address_Cnt = wStore_StartAddress;
  p_tDRV->dwWriteMemAddress = p_tDRV->dwRealROM_Address_Cnt;
  p_tDRV->dwAccessData_Cnt = 0;
  p_tDRV->dwBuf_Cnt = 0;
  
  for(;;)
  {    
    p_tDRV->p_uBuf[p_tDRV->dwBuf_Cnt] = p_uWrite[p_tDRV->dwAccessData_Cnt];
    
    p_tDRV->dwBuf_Cnt = p_tDRV->dwBuf_Cnt + 1;
    p_tDRV->dwAccessData_Cnt = p_tDRV->dwAccessData_Cnt + 1;
    p_tDRV->dwRealROM_Address_Cnt = p_tDRV->dwRealROM_Address_Cnt + 1;
    
    if( (p_tDRV->dwRealROM_Address_Cnt)%(p_tDRV->dwROM_Page_Size) == 0)
    {
      dwCheck = port_hsEEPROM_Write(p_tDRV->p_Rework_PP_Drv->p_PP_Drv, p_tDRV->uROM_DEV_Address, 
                                     p_tDRV->dwWriteMemAddress, p_tDRV->dwROM_Address_Size, p_tDRV->p_uBuf, p_tDRV->dwBuf_Cnt);
      
      if(dwCheck) goto hsEEPROM_WR_EXIT;
      
      p_tDRV->dwBuf_Cnt = 0;
      p_tDRV->dwWriteMemAddress = p_tDRV->dwRealROM_Address_Cnt;
      
      vTaskDelay(p_tDRV->dwROM_Page_Wrie_Delay_MS);
    }
    
    if( p_tDRV->dwRealROM_Address_Cnt >= p_tDRV->dwRealROM_Address_Cnt_Max) break;
  }
  
  if( p_tDRV->dwBuf_Cnt > 0)
  {
    dwCheck = port_hsEEPROM_Write(p_tDRV->p_Rework_PP_Drv->p_PP_Drv, p_tDRV->uROM_DEV_Address, 
                                   p_tDRV->dwWriteMemAddress, p_tDRV->dwROM_Address_Size, p_tDRV->p_uBuf, p_tDRV->dwBuf_Cnt);
    
    if(dwCheck) goto hsEEPROM_WR_EXIT;    
    vTaskDelay(p_tDRV->dwROM_Page_Wrie_Delay_MS);
  }
  
hsEEPROM_WR_EXIT:
  
  Unlock_Rework_PP_Drv(p_tDRV->p_Rework_PP_Drv);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsEEPROM_Read(hsEEPROM_DRV_Type *p_tDRV, uint16_t wRead_StartAddress, uint8_t *p_uReadBuf, uint16_t wRead_Size)
{
  int32_t dwCheck;
  
  //xToken = xSemaphoreTake( *((SemaphoreHandle_t *)(p_tDRV->p_Remapping_Dev_Handle->p_Mutex)), hsEEPROM_ACCESS_WAIT_TIME_MS);
  //if(xToken == pdFALSE) return false;
  dwCheck = Lock_Rework_PP_Drv(p_tDRV->p_Rework_PP_Drv, hsEEPROM_ACCESS_WAIT_TIME_MS);
  if(dwCheck) return dwCheck;
  
  p_tDRV->dwRealROM_Address_Cnt = wRead_StartAddress;
  p_tDRV->dwRealROM_Address_Cnt_Max = wRead_StartAddress + wRead_Size;
  
  if(p_tDRV->dwRealROM_Address_Cnt_Max >= p_tDRV->dwROM_Max_Size)
  {
    p_tDRV->dwRealROM_Address_Cnt_Max = p_tDRV->dwROM_Max_Size;
    
    p_tDRV->dwRealROM_Address_Cnt_Max = p_tDRV->dwRealROM_Address_Cnt_Max - wRead_StartAddress;
  }
  else
  {
    p_tDRV->dwRealROM_Address_Cnt_Max = wRead_Size;
  }
  
  dwCheck = port_hsEEPROM_Read(p_tDRV->p_Rework_PP_Drv->p_PP_Drv, p_tDRV->uROM_DEV_Address, 
                                p_tDRV->dwRealROM_Address_Cnt, p_tDRV->dwROM_Address_Size, p_uReadBuf, p_tDRV->dwRealROM_Address_Cnt_Max);
  
  Unlock_Rework_PP_Drv(p_tDRV->p_Rework_PP_Drv);
      
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
  * @retval None
  */


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
