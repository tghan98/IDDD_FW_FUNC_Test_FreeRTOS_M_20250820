
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
#include "IDDD_RunCommand_Interface.h"

//HW

//Interface & Handle


//-----------------------------------------------------------------------------
//define 

//OS Config

//Other

//Typedef
/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//OS Val
QueueHandle_t g_xQueIDDD_RunCmdCOM_Handle;

/* Private function prototypes -----------------------------------------------*/

/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_RunCmd_Interface_Config(void)
{
  int32_t dwCheck = 0;
  
  g_xQueIDDD_RunCmdCOM_Handle = xQueueCreate(IDDD_RUN_CMD_QUE_SZ, sizeof(IDDD_Run_Cmd_COM_t));
  
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
  * @retval None
  */
int32_t comIDDD_RunCmd_Send(IDDD_Run_Cmd_COM_t *p_tSendDataMem, uint32_t dwWaitTime)
{
  int32_t dwCheck = 0;
  BaseType_t xToken;
  
  xToken = xQueueSend(g_xQueIDDD_RunCmdCOM_Handle, p_tSendDataMem, dwWaitTime);
  if(xToken == pdFALSE) dwCheck = OS_QUE_PUT_FAIL;
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None pdTRUE 
  */
int32_t comIDDD_RunCmd_Get(IDDD_Run_Cmd_COM_t *p_tGetDataMem, uint32_t dwWaitTime)
{
  int32_t dwCheck = 0;
  BaseType_t xToken;
  
  xToken = xQueueReceive(g_xQueIDDD_RunCmdCOM_Handle, p_tGetDataMem, dwWaitTime);
  if(xToken == pdFALSE) dwCheck = OS_QUE_PUT_FAIL;
  
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
