
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
#include "hsCLI_Command.h"

#include "FreeRTOS_CLI.h"    
    
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#include "BoTnLE525BLE_Drv.h"

#include "IDDD_RunCommand_Interface.h"

#include "IDDD_ADC_SCAN_Drv.h"
#include "IDDD_LED_CTRL_Drv.h"
#include "IDDD_TRF_Timer_CTRL.h"

#include "IDDD_DBA_ROM_Interface.h"
#include "IDDD_DBA_RAM_Interface.h"

#include "IDDD_RunCmd_TRF.h"
    
//define
#define CLI_OUT_BUF_SZ                  configCOMMAND_INT_MAX_OUTPUT_SIZE


//OS Config

 //Typedef

//Macro
#define CLI_CMD_ERR                     hsGlobalErrorBlocking_2nd

/* Extern --------------------------------------------------------------------*/
extern BoTnLE523_Drv_t g_tBLE_Drv;

/* Private variables ---------------------------------------------------------*/
//OS

//Normal

/* Private function prototypes -----------------------------------------------*/
/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
BaseType_t cmd_clearScreen(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
  uint32_t dwCnt;
  
  (void)pcCommandString;
  (void)xWriteBufferLen;
  
  for(dwCnt = 0; dwCnt < xWriteBufferLen; dwCnt++)((uint8_t *)pcWriteBuffer)[dwCnt] = 0x00;
  
  hsDebug_MSG("\033[2J\033[1;1H");
  
  return pdFALSE; //no more string output is needed
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
BaseType_t BLE_Test(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
  char *pcParameter1;
  BaseType_t  xParameter1StringLength;
  uint32_t dwNum;
  int32_t dwCheck = 0;
  
  pcParameter1 = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);
  
  dwNum = strtoul(pcParameter1, NULL, 10);
  
  switch(dwNum)
  {
  case 1:
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "AT\n");
    dwCheck = BoTnLE_AT_Command_Send(&g_tBLE_Drv, "AT\r");
    if(dwCheck) CLI_CMD_ERR(dwCheck);
    
    break;
  case 2:
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "ATZ\n");
    dwCheck = BoTnLE_AT_Command_Send(&g_tBLE_Drv, "ATZ\r");
    if(dwCheck) CLI_CMD_ERR(dwCheck);

    break;
  case 3:
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "AT+VER?\n");
    dwCheck = BoTnLE_AT_Command_Send(&g_tBLE_Drv, "AT+VER?\r");
    if(dwCheck) CLI_CMD_ERR(dwCheck);

    break;

  case 4:
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "AT+INFO?\n");
    dwCheck = BoTnLE_AT_Command_Send(&g_tBLE_Drv, "AT+INFO?\r");
    if(dwCheck) CLI_CMD_ERR(dwCheck);

    break;
    
  default:
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "Error! Number Range[1-5] [%d].\n", dwNum);
    break;
  }
  
    
  return pdFALSE;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
BaseType_t Led_Power(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
  char *pcParameter1, *pcParameter2;
  BaseType_t  xParameter1StringLength, xParameter2StringLength;
  int32_t dwChannel, dwCurrentx100uA;
  int32_t dwCheck = 0;

  pcParameter1 = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);
  pcParameter2 = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameter2StringLength);
  
  dwChannel = strtoul(pcParameter1, NULL, 10);
  dwCurrentx100uA = strtoul(pcParameter2, NULL, 10);  
  
  if( (dwChannel > 0) && (dwChannel < OPT_LED_CH_MAX ) )
  {
    dwChannel =  dwChannel - 1;
    
    if(dwCurrentx100uA <= 3000)
    {
      dwCheck = IDDD_OPT_LED_CurrentData_Write_Interface(dwChannel, dwCurrentx100uA);
      if(dwCheck)
      {
        snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "IDDD_OPT_LED_CurrentData_Write_Interface(%d, %d) ...Fail![%d]\n",
                    dwChannel+1, dwCurrentx100uA, dwCheck);
      }
      else
      {
        snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "IDDD_OPT_LED_CurrentData_Write_Interface(%d, %d) ...OK![%d]\n",
                    dwChannel+1, dwCurrentx100uA, dwCheck);        
      }
    }
    else
    {
      snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "Error! Current Max Range Out.(Current < 1000 (x100uA)) [%d].\n", dwCurrentx100uA);
    }
    
  }
  else
  {
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "Error! Channel Range Out(1-7) [%d].\n", dwChannel);
  }
  
  return pdFALSE;
}



/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */

BaseType_t Optics_SET_Select(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
  char *pcParameter1;
  BaseType_t  xParameter1StringLength;
  uint32_t dwChannel;
  int32_t dwCheck = 0;
  
  pcParameter1 = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);
  
  dwChannel = strtoul(pcParameter1, NULL, 10);
  
  if( (dwChannel> 0) && (dwChannel < OPT_LED_CH_MAX ) )
  {
    dwChannel =  dwChannel - 1;
    
    dwCheck = IDDD_OPT_Channel_SEL_Write_Interface(dwChannel);
    if(dwCheck)
    {
      snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "IDDD_OPT_Channel_SEL_Write_Interface(%d) ...Fail![%d]\n",
                  dwChannel+1, dwCheck);
    }
    else
    {
      snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "IDDD_OPT_Channel_SEL_Write_Interface(%d) ...OK![%d]\n",
                  dwChannel+1, dwCheck);        
    }    
  }
  else
  {
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "Error! Channel Range Out(1-7) [%d].\n", dwChannel);
  }
  
  return pdFALSE;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
BaseType_t Trf_TimeSet(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
  char *pcParameter1, *pcParameter2;
  BaseType_t  xParameter1StringLength, xParameter2StringLength;
  int32_t dwLED_Time, dwADC_Time;
  int32_t dwCheck = 0, dwCheck2 = 0;
  
  pcParameter1 = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);
  pcParameter2 = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameter2StringLength);
  
  dwLED_Time = strtoul(pcParameter1, NULL, 10);
  dwADC_Time = strtoul(pcParameter2, NULL, 10);  
  
  if( (dwLED_Time<= 9999) && (dwADC_Time <= 9999))
  {
    dwCheck = IDDD_OPT_LED_ON_Time_Write_Interface(dwLED_Time);
    dwCheck2 = IDDD_OPT_PD_Scan_Time_Write_Interface(dwADC_Time);
    
    if( (dwCheck > 0) || (dwCheck2 > 0))
    {
      snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "LED On Time [%d], PD Scan Timming [%d] Write ...Fail\n", dwCheck, dwCheck2);
    }
    else
    {
      snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "LED On Time [%d], PD Scan Timming [%d] Write ...OK\n", dwCheck, dwCheck2);
    }
    
  }
  else
  {
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "Error! Range Out(0~ 9999) LED Time[%d], ADC Time[%d]\n", dwLED_Time, dwADC_Time);
  }
  
  return pdFALSE;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
BaseType_t OpticInfor(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
  int32_t dwCheck = 0;
  IDDD_Run_Cmd_COM_t tSendCmd;
  
  (void)pcCommandString;
  (void)xWriteBufferLen;
  
  tSendCmd.wCTRL[ctrlIDDD_RUN_CMD_ATT] = attIDDD_RUN_CMD_RUN;
  tSendCmd.wCTRL[ctrlIDDD_RUN_CMD_CMD] = cmdIDDD_INFO;
  

  dwCheck = comIDDD_RunCmd_Send(&tSendCmd, 1000);
  if(dwCheck)
  {
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "Error! Command Run ..Fail! [%d]\n", dwCheck);
  }
  else
  {
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "Optics Information ..OK!\n");
  }
  
  return pdFALSE;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
BaseType_t Trf_Run(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
  int32_t dwCheck = 0;
  IDDD_Run_Cmd_COM_t tSendCmd;
  
  (void)pcCommandString;
  (void)xWriteBufferLen;
  
  tSendCmd.wCTRL[ctrlIDDD_RUN_CMD_ATT] = attIDDD_RUN_CMD_RUN;
  tSendCmd.wCTRL[ctrlIDDD_RUN_CMD_CMD] = cmdIDDD_TRF_RUN;
  

  dwCheck = comIDDD_RunCmd_Send(&tSendCmd, 1000);
  if(dwCheck)
  {
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "Error! Command Run ..Fail! [%d]\n", dwCheck);
  }
  else
  {
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "TRF Run ..OK!\n");
  }
  
  return pdFALSE;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
BaseType_t Trf_Save(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
  int32_t dwCheck = 0;
  IDDD_Run_Cmd_COM_t tSendCmd;
  
  (void)pcCommandString;
  (void)xWriteBufferLen;
  
  tSendCmd.wCTRL[ctrlIDDD_RUN_CMD_ATT] = attIDDD_RUN_CMD_RUN;
  tSendCmd.wCTRL[ctrlIDDD_RUN_CMD_CMD] = cmdIDDD_SAVE;
  

  dwCheck = comIDDD_RunCmd_Send(&tSendCmd, 1000);
  if(dwCheck)
  {
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "Error! Command Run ..Fail! [%d]\n", dwCheck);
  }
  else
  {
    snprintf(pcWriteBuffer, CLI_OUT_BUF_SZ, "TRF Save ..OK!\n");
  }
  
  return pdFALSE;
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
const CLI_Command_Definition_t xCommandList[] = {
  
    {
        .pcCommand = "cls", /* The command string to type. */
        .pcHelpString = "cls:\n Clears screen\n\n",
        .pxCommandInterpreter = cmd_clearScreen, /* The function to run. */
        .cExpectedNumberOfParameters = 0 /* No parameters are expected. */
    },
    
    {
        .pcCommand = "bletest", /* The command string to type. */
        .pcHelpString = "bletest [num 1-4]\n",
        .pxCommandInterpreter = BLE_Test, /* The function to run. */
        .cExpectedNumberOfParameters = 1 /* 1 parameters are expected. */
    },

    {
        .pcCommand = "ledpw", /* The command string to type. */
        .pcHelpString = "ledpw [channel 1-7] [Current x100uA]\n",
        .pxCommandInterpreter = Led_Power, /* The function to run. */
        .cExpectedNumberOfParameters = 2 /* 2 parameters are expected. */
    },

    {
        .pcCommand = "optsel", /* The command string to type. */
        .pcHelpString = "optsel [Channel 1-7]\n",
        .pxCommandInterpreter = Optics_SET_Select, /* The function to run. */
        .cExpectedNumberOfParameters = 1 /* 1 parameters are expected. */
    },

    {
        .pcCommand = "trftim", /* The command string to type. */
        .pcHelpString = "trftim [LED Time us 1 ~ 9990] [ADC Time us 1 ~ 9990]\n",
        .pxCommandInterpreter = Trf_TimeSet, /* The function to run. */
        .cExpectedNumberOfParameters = 2 /* 1 parameters are expected. */
    },
    
    {
        .pcCommand = "info", /* The command string to type. */
        .pcHelpString = "info\n",
        .pxCommandInterpreter = OpticInfor, /* The function to run. */
        .cExpectedNumberOfParameters = 0 /* 0 parameters are expected. */
    },
    
    {
        .pcCommand = "trfrun", /* The command string to type. */
        .pcHelpString = "trfrun\n",
        .pxCommandInterpreter = Trf_Run, /* The function to run. */
        .cExpectedNumberOfParameters = 0 /* 0 parameters are expected. */
    },

    {
        .pcCommand = "trfsave", /* The command string to type. */
        .pcHelpString = "trfsave\n",
        .pxCommandInterpreter = Trf_Save, /* The function to run. */
        .cExpectedNumberOfParameters = 0 /* 0 parameters are expected. */
    },
    
    {
        .pcCommand = NULL /* simply used as delimeter for end of array*/
    } 

};


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
