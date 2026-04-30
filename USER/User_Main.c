
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
#include "User_Main.h"

#include "hsDEBUG_UART_CLI_App.h"
#include "Main_Communication_App.h"
#include "IDDD_RunCommand_App.h"

#include "Display_Application.h"
#include "Display_Interface.h"

#include "hsKey_Module.h"
//HW

//Interface & Handle


//-----------------------------------------------------------------------------
//define 

//OS Config
#define hsUSER_MAIN_TSK_MEM                           128 // 128 x 4 = 512 Byte
#define hsUSER_MAIN_TSK_PRIO                          (tskIDLE_PRIORITY + 1) // 6 5 4 3 2 |1| 0

//Other

//Typedef
/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//OS Val
TaskHandle_t            g_xUserMain_Task_Handle;
void                    *gp_UserMain_Param;

/* Private function prototypes -----------------------------------------------*/
void hsUserMain_Task(void *pvParam);

/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void hsMain_User_Config(void)
{
  int32_t dwCheck = 0;
  
  dwCheck = hsHW_Config();
  if(dwCheck) HW_LowCode_Error(dwCheck);
  
  dwCheck = hsDEBUG_UART_CLI_App_Config();
  if(dwCheck) HW_LowCode_Error(dwCheck);
  
  dwCheck = IDDD_RunCommand_App_Config();
  if(dwCheck) HW_LowCode_Error(dwCheck);

  dwCheck = IDDD_Display_App_Config();
  if(dwCheck) HW_LowCode_Error(dwCheck);  
  //dwCheck = Main_Com_App_Config();
  //if(dwCheck) HW_LowCode_Error(dwCheck);
  
  xTaskCreate(hsUserMain_Task, "Main", hsUSER_MAIN_TSK_MEM, 
              (void *)gp_UserMain_Param, hsUSER_MAIN_TSK_PRIO, &g_xUserMain_Task_Handle);
  
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
//-----------------------------------------------------------------------------
//Main Task
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None pdTRUE 
  */

void hsUserMain_Task(void *pvParam)
{
  int32_t dwCheckError = 0, dwCheck = 0;
  uint32_t dwDispSqMode, dwKeyStatus;
  
  vTaskDelay(1000);
  
  dwCheck = hsKey_Module_Config((uint32_t)HW_ReadPin_PWSW_Status);
  if(dwCheck) hsGlobalErrorBlocking_2nd(dwCheck);
  
  HW_All_LED_ONOFF(OFF);
  HW_PW_LED_ONnOFF(0,ON);
  
  HW_BAT_LED_ONnOFF(BAT_LED_CH1, ON);
  HW_BAT_LED_ONnOFF(BAT_LED_CH2, ON);
  HW_BAT_LED_ONnOFF(BAT_LED_CH3, ON);  
  
  //hsDebug_MSG("*** [MAIN] Application Start ***\n");  
  
  for(;;)
  {
    if(dwCheckError)
    {
      HW_BAT_LED_ONnOFF(BAT_LED_CH1, OFF);
      vTaskDelay(1000);
    }
    else
    {
      dwCheck = hsKey_ActiveCheck();
      if(dwCheck) hsGlobalErrorBlocking_2nd(dwCheck);
      
      dwCheck = hsKey_Status_Read(&dwKeyStatus);
      if(dwCheck) hsGlobalErrorBlocking_2nd(dwCheck);
      
      if(dwKeyStatus > 0)
      {
        dwCheck = IDDD_Disp_Status_Read(&dwDispSqMode);
        if(dwCheck) hsGlobalErrorBlocking_2nd(dwCheck);
        dwDispSqMode++;
        
        if(dwDispSqMode >= staDISP_MAX) dwDispSqMode = staDISP_BASE;
        
        dwCheck = IDDD_Disp_Status_Write(dwDispSqMode);
        if(dwCheck) hsGlobalErrorBlocking_2nd(dwCheck);
        
        dwCheck = hsKey_ActiveReset();
        if(dwCheck) hsGlobalErrorBlocking_2nd(dwCheck);
      }
      
      vTaskDelay(100);
    }
    
    hsGlobalErrorReadAllHandle((uint32_t *)&dwCheckError);
  }
}



#if 0
void hsUserMain_Task(void *pvParam)
{
  int32_t dwCheckError = 0, dwCheck = 0;
  
  vTaskDelay(1000);
  
  HW_All_LED_ONOFF(OFF);
  HW_PW_LED_ONnOFF(0,ON);
  
  HW_BAT_LED_ONnOFF(BAT_LED_CH1, ON);
  HW_BAT_LED_ONnOFF(BAT_LED_CH2, ON);
  HW_BAT_LED_ONnOFF(BAT_LED_CH3, ON);  
  
  hsDebug_MSG("*** [MAIN] Application Start ***\n");  
  for(;;)
  {
    if(dwCheckError)
    {
      HW_BAT_LED_ONnOFF(BAT_LED_CH1, OFF);
      vTaskDelay(1000);
    }
    else
    {
      //g_dwReadPinState, g_dwReadWrite_DispMode;
      g_dwReadPinState = HW_ReadPin_PWSW_Status();
      if(g_dwReadPinState == SET)
      {
        dwCheck = IDDD_Disp_Status_Read(&g_dwReadWrite_DispMode);
        if(dwCheck) hsGlobalErrorBlocking_2nd(dwCheck);
        
        g_dwReadWrite_DispMode++;
        
        if(g_dwReadWrite_DispMode >= staDISP_MAX)
        {
          g_dwReadWrite_DispMode = staDISP_BASE;
        }
        
        dwCheck = IDDD_Disp_Status_Write(g_dwReadWrite_DispMode);
        if(dwCheck) hsGlobalErrorBlocking_2nd(dwCheck);
      }
      
      //HW_BAT_LED_ONnOFF(BAT_LED_CH1, ON);
      //vTaskDelay(500);
      //HW_BAT_LED_ONnOFF(BAT_LED_CH1, OFF);
      //vTaskDelay(500);
      
      vTaskDelay(250);
    }
    
    hsGlobalErrorReadAllHandle((uint32_t *)&dwCheckError);
  }
}
#endif

//-----------------------------------------------------------------------------

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
