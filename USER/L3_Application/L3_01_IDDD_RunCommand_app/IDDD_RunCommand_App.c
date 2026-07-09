
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
#include "IDDD_RunCommand_App.h"
    
//
#include "IDDD_RunCommand_Interface.h"
#include "IDDD_RunCmd_TRF.h"
    
#include "IDDD_DBA_ROM_Interface.h"
#include "IDDD_DBA_RAM_Interface.h"

//TRF Include

  
/* Const --------------------------------------------------------------------- */

//OS
#define IDDD_RUN_CMD_APP_MEM                    256 //256 x 4 = 1024 Byte
#define IDDD_RUN_CMD_APP_PRIO                   (tskIDLE_PRIORITY + 2) // 6 5 4 3 |2| 1 0

//Other


#define IDDD_RUNCMD_ERR                         hsGlobalErrorBlocking_2nd

/* Extern --------------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

//OS
TaskHandle_t g_xIDDD_Run_CMD_App_Task;
void *gp_vIDDD_RunCmd_Para;


//Normal


/* Private function prototypes -----------------------------------------------*/
void IDDD_RunCommand_App(void *p_vParam);


int32_t IDDD_RunCmd_Exec(IDDD_Run_Cmd_COM_t *p_tRunCommand);

int32_t CmdRun_TRF_Run(void);
int32_t CmdRun_RegData_Info(void);

/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_RunCommand_App_Config(void)
{
  int32_t dwCheck = 0;
  
  
  dwCheck = IDDD_RunCmd_Interface_Config();
  if(dwCheck) return dwCheck;
  
  //dwCheck = IDDD_DBA_ROM_Interface_Config();
  //if(dwCheck) return dwCheck;
    
  xTaskCreate(IDDD_RunCommand_App, "RUN_CMD", IDDD_RUN_CMD_APP_MEM, (void *)gp_vIDDD_RunCmd_Para,
              IDDD_RUN_CMD_APP_PRIO, &g_xIDDD_Run_CMD_App_Task);
  
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */

//------------------------------------------------------------------------------

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void IDDD_RunCommand_App(void *p_vParam)
{
  int32_t dwCheck = 0;
  IDDD_Run_Cmd_COM_t tGetCOM_Cmd;
  uint32_t dwWaitTime;
  
  dwCheck = IDDD_RunCmd_TRF_Config();
  if(dwCheck) IDDD_RUNCMD_ERR(dwCheck);
  
  vTaskDelay(100);
  
  dwCheck = IDDD_DBA_ROM_Interface_Config();
  if(dwCheck) IDDD_RUNCMD_ERR(dwCheck);
  
  dwCheck = IDDD_DBA_ROM_Loading();
  if(dwCheck) IDDD_RUNCMD_ERR(dwCheck);  
  
  dwCheck = IDDD_DBA_RAM_Interface_Config();
  if(dwCheck) IDDD_RUNCMD_ERR(dwCheck);  
  
  vTaskDelay(1100);
    
  dwWaitTime = portMAX_DELAY;
  
  for(;;)
  {
    //vTaskDelay(500);
    dwCheck = comIDDD_RunCmd_Get(&tGetCOM_Cmd, dwWaitTime);
    if(dwCheck == 0)
    {
      dwCheck = IDDD_RunCmd_Exec(&tGetCOM_Cmd);
      if(dwCheck)
      {
        hsDebug_MSG("IDDD_RunCmd_Exec().. Fail Error Code[%d]\n", dwCheck);
      }
    }
    else
    {
    }
  }
}


//----------------------------------------------------------------------------
//Run Exec Function
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_RunCmd_Exec(IDDD_Run_Cmd_COM_t *p_tRunCommand)
{
  int32_t dwCheck = 0;
  uint16_t wCtrl_Cmd;
  
  wCtrl_Cmd = p_tRunCommand->wCTRL[ctrlIDDD_RUN_CMD_CMD];
  
  switch(wCtrl_Cmd)
  {
#if 0    
  case cmdIDDD_LED_PW:
    
    dwCheck = IDDD_OPT_LED_CurrentData_Write_Interface(p_tRunCommand->dwData[0], p_tRunCommand->dwData[1]);
    if(dwCheck)
    {
      hsDebug_MSG("IDDD_OPT_LED_CurrentData_Write_Interface(%d, %d) ...Fail![%d]\n",
                  p_tRunCommand->dwData[0], p_tRunCommand->dwData[1], dwCheck);
    }
    
    break;
  case cmdIDDD_OPT_SET:
    
    dwCheck = IDDD_OPT_Channel_SEL_Write_Interface(p_tRunCommand->dwData[0]);
    if(dwCheck)
    {
      hsDebug_MSG("IDDD_OPT_Channel_SEL_Write_Interface(%d) ...Fail![%d]\n",
                  p_tRunCommand->dwData[0], dwCheck);
    }
    
    break;
    
  case cmdIDDD_TRF_TIM:
    
    dwCheck = IDDD_OPT_LED_ON_Time_Write_Interface(p_tRunCommand->dwData[0]);
    if(dwCheck)
    {
      hsDebug_MSG("IDDD_OPT_LED_ON_Time_Write_Interface(%d) ...Fail![%d]\n",
                  p_tRunCommand->dwData[0], dwCheck);
    }
    
    dwCheck = IDDD_OPT_PD_Scan_Time_Write_Interface(p_tRunCommand->dwData[1]);
    if(dwCheck)
    {
      hsDebug_MSG("IDDD_OPT_PD_Scan_Time_Write_Interface(%d) ...Fail![%d]\n",
                  p_tRunCommand->dwData[1], dwCheck);
    }    
    
    
    break;
#endif

  case cmdIDDD_TRF_RUN:
    vTaskDelay(100);
    
    /* Stage2 단발 원복용: dwCheck = IDDD_RunCmd_TRF_Real(); */
    dwCheck = IDDD_RunCmd_TRF_Real2Cycle();
    
    break;
  case cmdIDDD_INFO:
    vTaskDelay(100);
    
    dwCheck = CmdRun_RegData_Info();
    
    break;
  case cmdIDDD_SAVE:
    vTaskDelay(100);
    
    dwCheck = IDDD_DBA_REG_ROM_Save();
    
    break;
  default:
    dwCheck = DAT_ERR_PARAM_CMD;
    break;
  }
  
  return dwCheck;
}


//------------------------------------------------------------------------------
//Run Function
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t CmdRun_TRF_Run(void)
{
  int32_t dwCheck = 0;
  uint32_t dwOPT_Ch, dwLED_On_Tim, dwPD_Scan_Tim, dwLED_Current;
  
  dwCheck = IDDD_OPT_OPT_Channel_SEL_Read_Interface(&dwOPT_Ch);
  if(dwCheck) return dwCheck;
  
  dwCheck = IDDD_OPT_LED_ON_Time_Read_Interface(&dwLED_On_Tim);
  if(dwCheck) return dwCheck;
  
  dwCheck = IDDD_OPT_PD_ScanTime_Read_Interface(&dwPD_Scan_Tim);
  if(dwCheck) return dwCheck;
  
  dwCheck = IDDD_OPT_LED_Current_Read_Interface(dwOPT_Ch, &dwLED_Current);
  if(dwCheck) return dwCheck;
  
#if RUN_CMDDEBU_MSG
  hsDebug_MSG("ReadMsg OPT CH:[%d], LED On Time:[%d], PD Scan Time:[%d], LED Current [%d]\n",
              dwOPT_Ch, dwLED_On_Tim, dwPD_Scan_Tim, dwLED_Current);
#endif
  
  dwCheck = IDDD_RunCmd_SyncTRF_Run(dwOPT_Ch, dwLED_On_Tim, dwPD_Scan_Tim, dwLED_Current);
  if(dwCheck)
  {
    hsDebug_MSG("IDDD_RunCmd_SyncTRF_Run(%d, %d, %d, %d) ...Fail![%d]\n", 
                dwOPT_Ch, dwLED_On_Tim, dwPD_Scan_Tim, dwLED_Current, dwCheck);
  }
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t CmdRun_RegData_Info(void)
{
  int32_t dwCheck = 0;
  uint32_t dwCnt, dwOPT_Ch, dwLED_On_Tim, dwPD_Scan_Tim, dwLED_Current;
  
  dwCheck = IDDD_OPT_OPT_Channel_SEL_Read_Interface(&dwOPT_Ch);
  if(dwCheck) return dwCheck;
  
  dwCheck = IDDD_OPT_LED_ON_Time_Read_Interface(&dwLED_On_Tim);
  if(dwCheck) return dwCheck;
  
  dwCheck = IDDD_OPT_PD_ScanTime_Read_Interface(&dwPD_Scan_Tim);
  if(dwCheck) return dwCheck;
  
  hsDebug_MSG("*** IDDD TRF System Register INFO ***\n");
  vTaskDelay(100);
  hsDebug_MSG("Select OPT Channel: %d \n", dwOPT_Ch+1);
  vTaskDelay(100);
  hsDebug_MSG("LED On Time: %d uSec\n", dwLED_On_Tim);
  vTaskDelay(100);
  hsDebug_MSG("PD Scan Timming: %d uSec\n", dwPD_Scan_Tim);
  vTaskDelay(100);
  
  for(dwCnt = 0; dwCnt < 7 ; dwCnt++)
  {
    dwCheck = IDDD_OPT_LED_Current_Read_Interface(dwCnt, &dwLED_Current);
    if(dwCheck) return dwCheck;
    
    hsDebug_MSG("LED Current Channel[%d] : %d x100uA\n", dwCnt+1, dwLED_Current);
    
    vTaskDelay(50);
  }
  
  hsDebug_MSG("*** End ***\n");
  
  vTaskDelay(50);
  
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


//------------------------------------------------------------------------------

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
