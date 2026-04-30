
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
#include "hsGlobalErrorHandle.h"

//HW Include

//Define

/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint32_t g_dwGlobalErrorStaus;
SemaphoreHandle_t g_xGlobalErrorStatusAccessKey;

/* Private function prototypes -----------------------------------------------*/
void (*__g_fpErrorAction)(uint32_t dwOn_nOff);

//Macro Function


/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void hsGlobalErrorUseConfig(uint32_t dwFp_LEDHandle)
{
  UBaseType_t xCount;
  
  __g_fpErrorAction = (void(*)(uint32_t))dwFp_LEDHandle;
  
  //global error uxSemaphoreGetCount() 0 --> Not Used Release 
  g_xGlobalErrorStatusAccessKey = xSemaphoreCreateMutex();
  xCount = uxSemaphoreGetCount(g_xGlobalErrorStatusAccessKey);
  if(xCount == 0) xSemaphoreGive(g_xGlobalErrorStatusAccessKey);
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsGlobalErrorResetHandle(void)
{
  xSemaphoreTake(g_xGlobalErrorStatusAccessKey, portMAX_DELAY);
  
  g_dwGlobalErrorStaus = RESET;
  
  xSemaphoreGive(g_xGlobalErrorStatusAccessKey);
  
  return 0;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsGlobalErrorWriteAllHandle(uint32_t dwErrorStatus)
{
  xSemaphoreTake(g_xGlobalErrorStatusAccessKey, portMAX_DELAY);
  
  g_dwGlobalErrorStaus = dwErrorStatus;
  
  xSemaphoreGive(g_xGlobalErrorStatusAccessKey);
  
  return 0;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsGlobalErrorWriteHandle(uint8_t uLayerCode, uint8_t uUtilCode1, uint8_t uUtilCode2, uint8_t uErrorCode)
{
  xSemaphoreTake(g_xGlobalErrorStatusAccessKey, portMAX_DELAY);
  
  g_dwGlobalErrorStaus = (uLayerCode << 24) | (uUtilCode1 << 16) | (uUtilCode2 << 8) | uErrorCode;
  
  xSemaphoreGive(g_xGlobalErrorStatusAccessKey);
  
  return 0;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsGlobalErrorReadAllHandle(uint32_t *p_dwReadMem)
{
  xSemaphoreTake(g_xGlobalErrorStatusAccessKey, portMAX_DELAY);
  
  *p_dwReadMem = g_dwGlobalErrorStaus;
  
  xSemaphoreGive(g_xGlobalErrorStatusAccessKey);
  
  return 0;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsGlobalErrorReadHandle(uint8_t *p_uLayerCode, uint8_t *p_uUtilCode1, uint8_t *p_uUtilCode2, uint8_t *p_uErrorCode)
{
  xSemaphoreTake(g_xGlobalErrorStatusAccessKey, portMAX_DELAY);
  
  *p_uLayerCode = g_dwGlobalErrorStaus >> 24;
  *p_uUtilCode1 = g_dwGlobalErrorStaus >> 16;
  *p_uUtilCode2 = g_dwGlobalErrorStaus >> 8;
  *p_uErrorCode = g_dwGlobalErrorStaus;
  
  xSemaphoreGive(g_xGlobalErrorStatusAccessKey);
  
  return 0;
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
int32_t hsGlobalErrorBlocking_1st(uint8_t uLayerCode, uint8_t uUtilCode1, uint8_t uUtilCode2, uint8_t uErrorCode)
{
  uint32_t dwReadGBL_Error;
  
  hsGlobalErrorWriteHandle(uLayerCode, uUtilCode1, uUtilCode2, uErrorCode);
  
 do
 {
   vTaskDelay(250);
   if(__g_fpErrorAction != NULL) __g_fpErrorAction(SET);
   vTaskDelay(250);
   if(__g_fpErrorAction != NULL) __g_fpErrorAction(RESET);
   
   hsGlobalErrorReadAllHandle(&dwReadGBL_Error);
   
 }while(dwReadGBL_Error);
 
 return 0;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsGlobalErrorBlocking_2nd(uint32_t dwErrorStatus)
{
  uint32_t dwReadGBL_Error;
  
  hsGlobalErrorWriteAllHandle(dwErrorStatus);
  
 do
 {
   vTaskDelay(250);
   if(__g_fpErrorAction != NULL) __g_fpErrorAction(SET);
   vTaskDelay(250);
   if(__g_fpErrorAction != NULL) __g_fpErrorAction(RESET);
   
   hsGlobalErrorReadAllHandle(&dwReadGBL_Error);
   
 }while(dwReadGBL_Error);
 
 return 0;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
