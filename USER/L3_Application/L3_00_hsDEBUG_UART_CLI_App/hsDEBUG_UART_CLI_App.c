/**
  ******************************************************************************
  * @file     
  * @author  
  * @version   
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "hsDEBUG_UART_CLI_App.h"

//HW Config
#include "port_hsDEBUG_UART_CLI_App.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "stdbool.h"
#include "stdlib.h"

#include "FreeRTOS_CLI.h"    

#include "BoTnLE525BLE_Drv.h"
        
//Public 
//Extern variable
extern const CLI_Command_Definition_t xCommandList[];

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

//OS
#define hsDEBUG_UART_CLI_TX_APP_MEM                128 // 128 x 4 = 512 Byte
#define hsDEBUG_UART_CLI_TX_APP_PRIO               (tskIDLE_PRIORITY + 0)  // 6 5 4 3 2 1 |0| 
#define hsDEBUG_UART_CLI_TX_APP_MAX_PRIO           (configMAX_PRIORITIES - 1) // |6| 5 4 3 2 1 0

#define hsDEBUG_UART_CLI_RX_APP_MEM                256 // 256 x 4 = 1024 Byte
#define hsDEBUG_UART_CLI_RX_APP_PRIO               (tskIDLE_PRIORITY + 1)  // 6 5 4 3 2 |1| 0 


#define hsDEBUG_UART_CLI_TX_QUE_WAIT_TIME       5    

#define hsDEBUG_UART_CLI_TX_CMPLT_TIME          100

#define hsDEBUG_UART_CLI_RX_QUE_WAIT_TIME       5

//Normal
#define MAX_INPUT_LENGTH                        50

#define hsDEBUG_UART_RX_QUE_SZ                  64
#define hsDEBUG_UART_TX_QUE_SZ                  128


/* Private macro -------------------------------------------------------------*/
#define hsDEBUG_ERROR_HANDLE            hsGlobalErrorBlocking_2nd
#define hsDEBUG_LOW_LEVEL_ERR_HANDLE    HW_LowCode_Error
    
/* Private variables ---------------------------------------------------------*/
//OS
SemaphoreHandle_t       g_xSemaDEBUG_UART_CLI_TX_CMLT_Signel;

QueueHandle_t           g_xQueDEBUG_UART_CLI_TX_DAT_Handle;
QueueHandle_t           g_xQueDEBUG_UART_CLI_RX_DAT_Handle;

TaskHandle_t            g_xDEBUG_UART_CLI_TX_App_Task;
TaskHandle_t            g_xDEBUG_UART_CLI_RX_App_Task;


//Normal

//DEBUG
uint8_t g_uDEBUG_RD_RX_DAT;

Rework_PP_Drv_t *p_tDEBUG_UART_PP_DRV;

uint8_t g_DEBUG_UART_TX_OUT_Mem[hsDEBUG_UART_TX_QUE_SZ];

BoTnLE523_Drv_t g_tBLE_Drv;

uint8_t g_uBLE_Read_Rx_Buff[BOTNLE523_RX_BUFF_SZ];

//CLI
int8_t cRxedChar;
char cOutputBuffer[configCOMMAND_INT_MAX_OUTPUT_SIZE], pcInputString[MAX_INPUT_LENGTH];

const char * cli_prompt = "\nHS> ";

/* CLI escape sequences*/
uint8_t backspace[] = "\b \b";
uint8_t backspace_tt[] = " \b";




/* Private function prototypes -----------------------------------------------*/
void hsDEBUG_UART_CLI_TX_App(void *p_vParam);
void hsDEBUG_UART_CLI_RX_App(void *p_vParam);


//DEBUG Func

int32_t hsDEBUG_UART_TX_String(char *p_uStringData);

//CLI Func
void vRegisterCLICommands(void);
void cliWrite(const char *str);
void CLI_HandleNewline(const char *const pcInputString, char *cOutputBuffer, uint8_t *cInputIndex);
void CLI_HandleBackspace(uint8_t *cInputIndex, char *pcInputString);
void CLI_HandleCharacterInput(uint8_t *cInputIndex, char *pcInputString);

/* Private functions ---------------------------------------------------------*/

//------------------------------------------------------------------------------
//IRQ Callback Function 
/**
  * @brief  
  * @param  None
  * @retval None
  */
void hsDEBUG_UART_CLI_TX_DMA_CMPLT_Callback(uint32_t dwPortNum)
{
  if(HW_ReadPin_BLE_ConnectStatus())
  {
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    
    xSemaphoreGiveFromISR(g_xSemaDEBUG_UART_CLI_TX_CMLT_Signel, &xHigherPriorityTaskWoken);
    
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
  else
  {
    BoTLE_UART_TX_IRQ_Handler(dwPortNum);
  }
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void hsDEBUG_UART_CLI_RX_DMA_CMPLT_Callback(uint32_t dwPortNum)
{
  if(HW_ReadPin_BLE_ConnectStatus())
  {
    BaseType_t xToken;
    BaseType_t xHigherPriorityTaskWoken;
    int32_t dwCheck = 0;
    
    xHigherPriorityTaskWoken = pdFALSE;
    
    xToken = xQueueSendFromISR(g_xQueDEBUG_UART_CLI_RX_DAT_Handle, &g_uDEBUG_RD_RX_DAT, &xHigherPriorityTaskWoken);
    if(xToken == pdFALSE)
    {
      xQueueReset(g_xQueDEBUG_UART_CLI_RX_DAT_Handle);
    }
    
    if(p_tDEBUG_UART_PP_DRV != NULL)
    {
      dwCheck = port_hsDEBUG_UART_RX_Run(p_tDEBUG_UART_PP_DRV->p_PP_Drv, &g_uDEBUG_RD_RX_DAT, 1);
      if(dwCheck) hsDEBUG_LOW_LEVEL_ERR_HANDLE(dwCheck);
    }
  }
  else
  {
    BoTLE_UART_RX_IRQ_Handler(dwPortNum);
  }
}


//------------------------------------------------------------------------------
//DEBUG UART Config
/**
  * @brief  
  * @param  None
  * @retval None
  */

int32_t hsDEBUG_UART_CLI_App_Config(void)
{
  int32_t dwCheck = 0;
  
  p_tDEBUG_UART_PP_DRV = Read_Rework_PP_Drv(RWRK_USART1_PP);
  
  port_hsDEBUS_UART_CLI_IRQ_Linker( (uint32_t)hsDEBUG_UART_CLI_RX_DMA_CMPLT_Callback,
                                    (uint32_t)hsDEBUG_UART_CLI_TX_DMA_CMPLT_Callback);
  
  dwCheck = port_hsDEBUG_UART_CLI_Config(p_tDEBUG_UART_PP_DRV->p_PP_Drv, ENABLE);
  if(dwCheck) return dwCheck;
  
  //RX Config
  g_xQueDEBUG_UART_CLI_RX_DAT_Handle = xQueueCreate(hsDEBUG_UART_RX_QUE_SZ,  sizeof(uint8_t));
  dwCheck = port_hsDEBUG_UART_RX_Run(p_tDEBUG_UART_PP_DRV->p_PP_Drv, &g_uDEBUG_RD_RX_DAT, 1);
  if(dwCheck) return dwCheck;
  
  //TX Config
  g_xQueDEBUG_UART_CLI_TX_DAT_Handle = xQueueCreate(hsDEBUG_UART_TX_QUE_SZ,  sizeof(uint8_t));
  g_xSemaDEBUG_UART_CLI_TX_CMLT_Signel = xSemaphoreCreateBinary();
  
  xTaskCreate(hsDEBUG_UART_CLI_TX_App, "CLI_TX",  hsDEBUG_UART_CLI_TX_APP_MEM, 
              (void *)p_tDEBUG_UART_PP_DRV, hsDEBUG_UART_CLI_TX_APP_PRIO, &g_xDEBUG_UART_CLI_TX_App_Task);
  
  xTaskCreate(hsDEBUG_UART_CLI_RX_App, "CLI_RX",  hsDEBUG_UART_CLI_RX_APP_MEM, 
              (void *)p_tDEBUG_UART_PP_DRV, hsDEBUG_UART_CLI_RX_APP_PRIO, &g_xDEBUG_UART_CLI_RX_App_Task);
  
  
  hsDebug_MSG_Config((void *)hsDEBUG_UART_TX_String);
  
  return dwCheck;
}


//------------------------------------------------------------------------------
//Application
/**
  * @brief  
  * @param  None
  * @retval None
  */
void hsDEBUG_UART_CLI_TX_App(void *p_vParam)
{
  //DEBUG Val
  BaseType_t xToken;
  UBaseType_t xCnt;
  Rework_PP_Drv_t *p_tPP_Drv;
  uint32_t dwCnt, dwWaitTime;
  int32_t dwCheck = 0;
  
  //DEBUG Config
  p_tPP_Drv = (Rework_PP_Drv_t *)p_vParam;
    
  for(;;)
  {
    if(HW_ReadPin_BLE_ConnectStatus())
    {
      //--------------------------------------------------------------------------
      //DEBUG TX Send
      //dwWaitTime = hsDEBUG_UART_CLI_TX_QUE_WAIT_TIME;
      dwWaitTime = portMAX_DELAY;
      for(dwCnt = 0; dwCnt < hsDEBUG_UART_TX_QUE_SZ; dwCnt++) g_DEBUG_UART_TX_OUT_Mem[dwCnt] = 0;
      dwCheck = 0;
      
      for(dwCnt = 0; dwCnt < hsDEBUG_UART_TX_QUE_SZ; dwCnt++)
      {
        xToken = xQueueReceive(g_xQueDEBUG_UART_CLI_TX_DAT_Handle, &g_DEBUG_UART_TX_OUT_Mem[dwCnt], dwWaitTime);
        if(xToken == pdFALSE)
        {
          //dwCheck = OS_QUE_GET_FAIL;
          break;
        }
        
        dwWaitTime = hsDEBUG_UART_CLI_TX_QUE_WAIT_TIME;
      }
      
      if(dwCnt > 0)
      {
        xCnt = uxSemaphoreGetCount(g_xSemaDEBUG_UART_CLI_TX_CMLT_Signel);
        if(xCnt > 0) xSemaphoreTake(g_xSemaDEBUG_UART_CLI_TX_CMLT_Signel, 1);
        
        dwCheck = port_hsDEBUS_UART_TX_Run(p_tPP_Drv->p_PP_Drv, g_DEBUG_UART_TX_OUT_Mem, dwCnt);
        if(dwCheck) 
        {
          hsDEBUG_LOW_LEVEL_ERR_HANDLE(dwCheck);
        }
        
        //hsDEBUG_UART_CLI_TX_CMPLT_TIME
        xToken = xSemaphoreTake(g_xSemaDEBUG_UART_CLI_TX_CMLT_Signel, hsDEBUG_UART_CLI_TX_CMPLT_TIME);
        if(xToken == pdFALSE)
        {
          dwCheck = OS_SIG_TIMOUT;
          hsDEBUG_LOW_LEVEL_ERR_HANDLE(dwCheck);
        }
      }
      //--------------------------------------------------------------------------
    }
    else
    {
    }
    
  }
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void hsDEBUG_UART_CLI_RX_App(void *p_vParam)
{
  //CLI Val
  uint8_t cInputIndex = 0; 
  uint8_t cReceivedValue;
  char uTempStr[2] = {0};
  
  BaseType_t xToken;
  uint32_t dwWaitTime;
  int32_t dwCheck = 0;
  
  dwCheck = BotnLE_BLE_Config(&g_tBLE_Drv, RWRK_USART1_PP, PORT_NUM_USART1, 19200);
  if(dwCheck) hsDEBUG_ERROR_HANDLE(dwCheck);
  
  HW_BLE_Power_ONnOFF(ON);

  
  //CLI Config
  vRegisterCLICommands();
  hsDebug_MSG("*** Micro Optics Test Command Line Interface ***\n");
  cliWrite(cli_prompt);
  
  for(;;)
  {
    if(HW_ReadPin_BLE_ConnectStatus())
    {
    //--------------------------------------------------------------------------
    //CLI RX 
    dwWaitTime = portMAX_DELAY;
    xToken = xQueueReceive(g_xQueDEBUG_UART_CLI_RX_DAT_Handle, &cReceivedValue, dwWaitTime);
    if(xToken == pdTRUE)
    {
      cRxedChar = cReceivedValue & 0xFF;
      uTempStr[0] = cRxedChar;
      
      cliWrite(uTempStr); //echo recevied char
      
      if( (cRxedChar == '\r') || (cRxedChar == '\n') )
      {
        CLI_HandleNewline(pcInputString, cOutputBuffer, &cInputIndex); //// user pressed enter, process the command
      }
      else
      {
        CLI_HandleCharacterInput(&cInputIndex, pcInputString); //user pressed a character add it to the input string    
      }
    }
    //--------------------------------------------------------------------------
    }
    else
    {
#if 0
      dwCheck = BoTnLE_AT_Command_Response(&g_tBLE_Drv, g_uBLE_Read_Rx_Buff, 500);
      if(dwCheck == 0)
      {
        hsDebug_MSG("%s\n", g_uBLE_Read_Rx_Buff);
        for(dwCnt = 0; dwCnt< BOTNLE523_RX_BUFF_SZ;dwCnt++) g_uBLE_Read_Rx_Buff[dwCnt] = 0;
      }
#endif      
    }
  }
}

//------------------------------------------------------------------------------
//DEBUG_USART_CLI_Function
/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t hsDEBUG_UART_TX_Char(char uDat)
{
  BaseType_t xToken;
  UBaseType_t xCnt;
  int32_t dwCheck = 0;
  
  xCnt = uxQueueSpacesAvailable(g_xQueDEBUG_UART_CLI_TX_DAT_Handle);
  if(xCnt < 2)
  {
    vTaskPrioritySet(g_xDEBUG_UART_CLI_TX_App_Task, hsDEBUG_UART_CLI_TX_APP_MAX_PRIO);
    vTaskDelay(hsDEBUG_UART_CLI_TX_CMPLT_TIME);
    vTaskPrioritySet(g_xDEBUG_UART_CLI_TX_App_Task, hsDEBUG_UART_CLI_TX_APP_PRIO);
  }
  
  if(uDat == '\n')
  {
    uDat = '\r';
    xToken = xQueueSend(g_xQueDEBUG_UART_CLI_TX_DAT_Handle, &uDat, 0);
    if(xToken == pdTRUE)
    {
      uDat = '\n';
      xToken = xQueueSend(g_xQueDEBUG_UART_CLI_TX_DAT_Handle, &uDat, 0);
    }
  }
  else
  {
    xToken = xQueueSend(g_xQueDEBUG_UART_CLI_TX_DAT_Handle, &uDat, 0);
  }
  
  if(xToken == pdFALSE)
  {
    dwCheck = OS_QUE_PUT_FAIL;
  }
  
  return dwCheck;
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t hsDEBUG_UART_TX_String(char *p_uStringData)
{
  int32_t dwCheck = 0;
  
  while(*p_uStringData)
  {
    dwCheck = hsDEBUG_UART_TX_Char(*p_uStringData++);
    if(dwCheck)
    {
      hsDEBUG_ERROR_HANDLE(dwCheck);
      break;
    }
  }
  
  return dwCheck;
}



/**
  * @brief  
  * @param  None
  * @retval None
  */

/**
  * @brief  
  * @param  None
  * @retval None
  */

/**
  * @brief  
  * @param  None
  * @retval None
  */

/**
  * @brief  
  * @param  None
  * @retval None
  */

//------------------------------------------------------------------------------
//CLI Function
/**
  * @brief  
  * @param  None
  * @retval None
  */
void vRegisterCLICommands(void)
{
  int32_t dwCnt;
  
  for(dwCnt = 0; xCommandList[dwCnt].pcCommand != NULL ; dwCnt++) FreeRTOS_CLIRegisterCommand(&xCommandList[dwCnt]);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
void cliWrite(const char *str)
{
  hsDebug_MSG("%s", str);
}



/**
  * @brief  
  * @param  None
  * @retval None
  */
void CLI_HandleNewline(const char *const pcInputString, char *cOutputBuffer, uint8_t *cInputIndex)
{
  uint32_t dwCnt;
  BaseType_t xMoreDataToFollow;
  
  cliWrite("\n");
  
  do
  {
    xMoreDataToFollow = FreeRTOS_CLIProcessCommand(pcInputString, cOutputBuffer, configCOMMAND_INT_MAX_OUTPUT_SIZE);
    
   cliWrite(cOutputBuffer);
   
  }while(xMoreDataToFollow != pdFALSE);
  
  cliWrite(cli_prompt);
  
  *cInputIndex = 0;
  
  for(dwCnt = 0; dwCnt < MAX_INPUT_LENGTH; dwCnt++) ((uint8_t *)pcInputString)[dwCnt] = 0x00;
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void CLI_HandleBackspace(uint8_t *cInputIndex, char *pcInputString)
{
  uint8_t right[] = "\x1b\x5b\x43";
  
  if(*cInputIndex > 0)
  {
    (*cInputIndex)--;
    pcInputString[*cInputIndex] = '\0';
    cliWrite((char *)backspace_tt);
  }
  else
  {
    cliWrite((char *)right);
  }
}


/**
  * @brief  
  * @param  None
  * @retval None
  */
void CLI_HandleCharacterInput(uint8_t *cInputIndex, char *pcInputString)
{
  if(cRxedChar ==  '\r') return;
  else if( (cRxedChar == (uint8_t)0x08)||(cRxedChar==(uint8_t)0x7F) )
  {
    CLI_HandleBackspace(cInputIndex, pcInputString);
  }
  else
  {
    if( *cInputIndex < MAX_INPUT_LENGTH)
    {
      pcInputString[*cInputIndex] = cRxedChar;
      (*cInputIndex)++;
    }
  }
}


/**
  * @brief  
  * @param  None
  * @retval None
  */

/**
  * @brief  
  * @param  None
  * @retval None
  */

/**
  * @brief  
  * @param  None
  * @retval None
  */
