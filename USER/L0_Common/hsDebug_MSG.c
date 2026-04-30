
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
#include "hsDebug_MSG.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

//define 
#define hsMSG_GEN_BUF_SZ                127

#define hsMSG_ACCESS_TIME               250
//OS Config

 //Typedef

/* Extern --------------------------------------------------------------------*/
//extern int hsvsnprintf(char *buf, size_t size, const char *fmt, va_list args);


/* Private variables ---------------------------------------------------------*/
char g_uMSG_GenBuf[hsMSG_GEN_BUF_SZ];
SemaphoreHandle_t g_xMSG_Mutex;
int32_t (*g_fpDebugMSG_Stream)(char *str);

/* Private function prototypes -----------------------------------------------*/


/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void hsDebug_MSG_Config(void *fpOutStream)
{
  g_xMSG_Mutex = xSemaphoreCreateMutex();
  xSemaphoreGive(g_xMSG_Mutex);
  
  g_fpDebugMSG_Stream = (int32_t(*)(char *))fpOutStream;
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
void hsDebug_MSG(char *fmt, ...)
{
  BaseType_t xToken;
  
  //int32_t dwlen, dwCnt;
  int32_t dwCnt;
  va_list  ap;
  
  xToken = xSemaphoreTake(g_xMSG_Mutex, hsMSG_ACCESS_TIME);
  if(xToken == pdFALSE) return;
  
  //memset(g_uMSG_GenBuf, 0, hsMSG_GEN_BUF_SZ);
  for(dwCnt = 0; dwCnt < hsMSG_GEN_BUF_SZ; dwCnt++) g_uMSG_GenBuf[dwCnt] = 0;
  
  //strcpy(g_uMSG_GenBuf, "[D_MSG] ");
  //dwlen = strlen(g_uMSG_GenBuf);
  
  va_start(ap, fmt);
  //vsnprintf(g_uMSG_GenBuf + dwlen, hsMSG_GEN_BUF_SZ - dwlen, fmt, ap);
  vsnprintf(g_uMSG_GenBuf, hsMSG_GEN_BUF_SZ, fmt, ap);
  va_end(ap);
  
  if(g_fpDebugMSG_Stream != NULL) g_fpDebugMSG_Stream(g_uMSG_GenBuf);
  
  xSemaphoreGive(g_xMSG_Mutex);
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


//----------------------------------------------------------------------------
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
