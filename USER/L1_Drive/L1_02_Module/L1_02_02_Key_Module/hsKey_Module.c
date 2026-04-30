
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
#include "hsKey_Module.h"

//HW

//Interface & Handle


//-----------------------------------------------------------------------------
//define 
#define KEY_VAL_ACCES_TIM_OUT           200

#define KEY_PUSH_STA_SHORT_CNT_MAX            3
#define KEY_PUSH_STA_LONG_CNT_MAX             50 // 50 x 100 =  5000 msec
#define KEY_POP_CNT_MAX                       3

//OS Config
SemaphoreHandle_t g_xKeyVal_Access_Mutex_Handle;

//Other

//Typedef
/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//OS Val

//Normal Val
uint32_t g_dwPush_STA_Cnt, g_dwPop_STA_Cnt, g_dwKeyActive_Flag;

uint32_t(*g_fpPushCheck)(void);

/* Private function prototypes -----------------------------------------------*/

/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsKey_Module_Config(uint32_t dwFp_PushCheckFunc)
{
  int32_t dwCheck = 0;
  
  g_xKeyVal_Access_Mutex_Handle = xSemaphoreCreateMutex();
  xSemaphoreGive(g_xKeyVal_Access_Mutex_Handle);
  
  g_fpPushCheck = (uint32_t(*)(void))dwFp_PushCheckFunc;
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
//-----------------------------------------------------------------------------
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None pdTRUE 
  */

int32_t hsKey_ActiveCheck(void)
{
  BaseType_t xToken;
  int32_t dwCheck = 0;
  uint32_t dwPinState;
  
  if(g_fpPushCheck == NULL) return OS_SYS_ERR;
    
  xToken = xSemaphoreTake(g_xKeyVal_Access_Mutex_Handle, KEY_VAL_ACCES_TIM_OUT);
  if(xToken == pdFALSE) return OS_ACCES_FAIL;
  
  dwPinState = g_fpPushCheck();
  
  if(dwPinState == SET)
  {
    g_dwPush_STA_Cnt++;
  }
  else
  {
    if( (g_dwPush_STA_Cnt < KEY_PUSH_STA_LONG_CNT_MAX) && (g_dwPush_STA_Cnt >= KEY_PUSH_STA_SHORT_CNT_MAX) )
    {
      g_dwPop_STA_Cnt++;
      
      if( g_dwPop_STA_Cnt >= KEY_POP_CNT_MAX)
      {
        g_dwKeyActive_Flag = HS_KEY_STA_SHORT_ACT;
        g_dwPush_STA_Cnt = 0;
        g_dwPop_STA_Cnt = 0;
      }
      else
      {
        //goto KEY_COUNT_RESET;
      }
    }
    else if(g_dwPush_STA_Cnt >= KEY_PUSH_STA_LONG_CNT_MAX)
    {
      g_dwPop_STA_Cnt++;

      if( g_dwPop_STA_Cnt >= KEY_POP_CNT_MAX)
      {
        g_dwKeyActive_Flag = HS_KEY_STA_LONG_ACT;
        g_dwPush_STA_Cnt = 0;
        g_dwPop_STA_Cnt = 0;
      }
      else
      {
        //goto KEY_COUNT_RESET;
      }      
    }
    else
    {
    //KEY_COUNT_RESET:
      g_dwPush_STA_Cnt = 0;
      g_dwPop_STA_Cnt = 0;
    }
  }
  
  xSemaphoreGive(g_xKeyVal_Access_Mutex_Handle);
  
  return dwCheck;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsKey_ActiveReset(void)
{
  BaseType_t xToken;
  int32_t dwCheck = 0;  

  xToken = xSemaphoreTake(g_xKeyVal_Access_Mutex_Handle, KEY_VAL_ACCES_TIM_OUT);
  if(xToken == pdFALSE) return OS_ACCES_FAIL;
  
  g_dwKeyActive_Flag = HS_KEY_STA_NON_ACT;
  
  xSemaphoreGive(g_xKeyVal_Access_Mutex_Handle);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t hsKey_Status_Read(uint32_t *p_dwRead)
{
  BaseType_t xToken;
  int32_t dwCheck = 0;  

  xToken = xSemaphoreTake(g_xKeyVal_Access_Mutex_Handle, KEY_VAL_ACCES_TIM_OUT);
  if(xToken == pdFALSE) return OS_ACCES_FAIL;
  
  *p_dwRead = g_dwKeyActive_Flag;
  
  xSemaphoreGive(g_xKeyVal_Access_Mutex_Handle);
  
  return dwCheck;  
}



//----------------------------------------------------------------------------
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
