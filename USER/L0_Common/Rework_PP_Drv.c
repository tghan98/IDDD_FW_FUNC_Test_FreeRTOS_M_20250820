
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
#include "main.h"
#include "Rework_PP_Drv.h"

//Define
#define RWRK_BUF_SZ             6 //MCU Used Peripheral Max Count, 

/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
Rework_PP_Drv_t g_Rework_PP_Buf[RWRK_BUF_SZ];

/* Private function prototypes -----------------------------------------------*/


//Macro Function


/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t Rework_PP_Drv_Config(void *p_PP_Drv, uint32_t dwPP_Drv_ID)
{
  if(dwPP_Drv_ID >= RWRK_BUF_SZ) return RWRK_PP_SYS_ERR;
  
  g_Rework_PP_Buf[dwPP_Drv_ID].p_PP_Drv = p_PP_Drv;
  g_Rework_PP_Buf[dwPP_Drv_ID].xMutex = xSemaphoreCreateMutex();
  xSemaphoreGive(g_Rework_PP_Buf[dwPP_Drv_ID].xMutex);
  
  return 0;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval : Drv Address
  */
Rework_PP_Drv_t *Read_Rework_PP_Drv(uint32_t dwRework_PP_ID)
{
  if(dwRework_PP_ID >= RWRK_BUF_SZ) return NULL;
  
  return &g_Rework_PP_Buf[dwRework_PP_ID];
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t Lock_Rework_PP_Drv(Rework_PP_Drv_t *p_tDrv, uint32_t xWaitTime)
{
  BaseType_t xToken;
  
  xToken = xSemaphoreTake(p_tDrv->xMutex, xWaitTime);
  
  if(xToken == pdFALSE) return RWRK_PP_SYS_ERR;
  
  return 0;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void Unlock_Rework_PP_Drv(Rework_PP_Drv_t *p_tDrv)
{
  xSemaphoreGive(p_tDrv->xMutex); 
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
