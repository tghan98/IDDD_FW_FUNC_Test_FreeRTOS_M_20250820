
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
#include "BoTnLE525BLE_Drv.h"

//HW
#include "BoTnLE525BLE_Port.h"


#include <string.h>
#include <stdio.h>
#include <stdarg.h>

//Interface & Handle


//-----------------------------------------------------------------------------
//define 

//OS Config

//Other
#define BOTNLE_LOW_LV_ERR_HANDLE                        HW_LowCode_Error

#define BOTNLE_BLE_CR                                       '\r'

#define BOTNLE_BLE_SEND_TIME_OUT                        1000

//Typedef
/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//OS Val

//Normale Val
BoTnLE523_Drv_t *gp_tBotLE_Drv;

/* Private function prototypes -----------------------------------------------*/

/* Interrupt  ----------------------------------------------------------------*/
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void BoTLE_UART_RX_IRQ_Handler(uint32_t dwPortNum)
{  
  int32_t dwCheck = 0, dwCnt;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  
  if(gp_tBotLE_Drv != NULL)
  {
    if(gp_tBotLE_Drv->dwRx_Use_Flag)
    {
      if(gp_tBotLE_Drv->uRx_Byte == BOTNLE_BLE_CR)
      {
        xQueueSendFromISR(gp_tBotLE_Drv->xQue_RxHandle, gp_tBotLE_Drv->uRx_Buff, &xHigherPriorityTaskWoken);
        
        for(dwCnt = 0; dwCnt < BOTNLE523_RX_BUFF_SZ ; dwCnt++) gp_tBotLE_Drv->uRx_Buff[dwCnt] = 0;
        
        gp_tBotLE_Drv->dwRx_Index = 0;
      }
      else
      {
        if(gp_tBotLE_Drv->dwRx_Index < (BOTNLE523_RX_BUFF_SZ - 2))
        {
          gp_tBotLE_Drv->uRx_Buff[(gp_tBotLE_Drv->dwRx_Index)++] = gp_tBotLE_Drv->uRx_Byte;
        }
        else
        {
          for(dwCnt = 0; dwCnt < BOTNLE523_RX_BUFF_SZ ; dwCnt++) gp_tBotLE_Drv->uRx_Buff[dwCnt] = 0;
          gp_tBotLE_Drv->dwRx_Index = 0;
        }
      }
    }
    
    dwCheck = port_hsBoTnL525BLE_UART_RX_Run(gp_tBotLE_Drv->p_tReWork_HAL_Drv->p_PP_Drv, &(gp_tBotLE_Drv->uRx_Byte), sizeof(uint8_t));
    if(dwCheck) BOTNLE_LOW_LV_ERR_HANDLE(dwCheck);
    
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void BoTLE_UART_TX_IRQ_Handler(uint32_t dwPortNum)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  
  if(gp_tBotLE_Drv != NULL)
  {
    xSemaphoreGiveFromISR(gp_tBotLE_Drv->xSemaTxCMPL_Signal, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}


/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t BotnLE_BLE_Config(BoTnLE523_Drv_t *p_tDrv, uint32_t dwReWork_ID, uint32_t dwPort_Num, uint32_t dwdwBaudRate)
{
  int32_t dwCheck = 0;
  
  __disable_irq();
  
  if(p_tDrv->p_tReWork_HAL_Drv != NULL) port_hsBoTnL525BLE_UART_RX_Stop(p_tDrv->p_tReWork_HAL_Drv->p_PP_Drv);
  
  p_tDrv->p_tReWork_HAL_Drv = Read_Rework_PP_Drv(dwReWork_ID);
  p_tDrv->dwPortNum = dwPort_Num;
  
  //port_hsBoTnL525BLE_UART_IRQ_Linker(dwPort_Num, (uint32_t)BoTLE_UART_RX_IRQ_Handler, (uint32_t)BoTLE_UART_TX_IRQ_Handler);
  
  dwCheck = port_hsBoTnL525BLE_UART_Config(p_tDrv->p_tReWork_HAL_Drv->p_PP_Drv, dwdwBaudRate, ENABLE);
  if(dwCheck) goto BLE_CONFIG_EXIT;
  
  
  gp_tBotLE_Drv = p_tDrv;
  
  p_tDrv->xQue_RxHandle = xQueueCreate(BOTNLE523_RX_QUE_SZ, BOTNLE523_RX_BUFF_SZ);
  p_tDrv->xSemaTxCMPL_Signal = xSemaphoreCreateBinary();
  xSemaphoreGive(p_tDrv->xSemaTxCMPL_Signal);
  
  p_tDrv->dwRx_Use_Flag = SET;
  
   dwCheck = port_hsBoTnL525BLE_UART_RX_Run(p_tDrv->p_tReWork_HAL_Drv->p_PP_Drv, &(p_tDrv->uRx_Byte), sizeof(uint8_t));
   if(dwCheck) goto BLE_CONFIG_EXIT;
  
BLE_CONFIG_EXIT:
  
  __enable_irq();
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void BoTnLE_BLE_UART_Link(BoTnLE523_Drv_t *p_tDrv)
{
   port_hsBoTnL525BLE_UART_IRQ_Linker(p_tDrv->dwPortNum, (uint32_t)BoTLE_UART_RX_IRQ_Handler, (uint32_t)BoTLE_UART_TX_IRQ_Handler);
}

//-----------------------------------------------------------------------------

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t BoTnLE_AT_Command_Send(BoTnLE523_Drv_t *p_tDrv, char *fmt, ...)
{
  int32_t dwCheck = 0;
  uint32_t dwCnt;
  UBaseType_t xCnt;
  BaseType_t xToken;
  va_list ap;
  
  for(dwCnt = 0; dwCnt < BOTNLE523_TX_BUFF_SZ; dwCnt++) p_tDrv->uTx_Buff[dwCnt] = 0;
  
  va_start(ap, fmt);
  
  vsnprintf((char *)p_tDrv->uTx_Buff, BOTNLE523_TX_BUFF_SZ, fmt, ap); 
  
  va_end(ap);
  
  p_tDrv->dwTx_Sz = strlen((char *)p_tDrv->uTx_Buff);
  
  
  xCnt = uxSemaphoreGetCount(p_tDrv->xSemaTxCMPL_Signal);
  if(xCnt > 0) xSemaphoreTake(p_tDrv->xSemaTxCMPL_Signal, 5);
  
  dwCheck = port_hsBoTnL525BLE_UART_TX_Run(p_tDrv->p_tReWork_HAL_Drv->p_PP_Drv, p_tDrv->uTx_Buff, p_tDrv->dwTx_Sz );
  if(dwCheck) return dwCheck;
  
  xToken = xSemaphoreTake(p_tDrv->xSemaTxCMPL_Signal, BOTNLE_BLE_SEND_TIME_OUT);
  if(xToken == pdFALSE)
  {
    dwCheck = OS_SIG_TIMOUT;
    return dwCheck;
  }
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t BoTnLE_AT_Command_Response(BoTnLE523_Drv_t *p_tDrv, uint8_t *p_uReadResponStr, uint32_t dwWaitTime)
{
  int32_t dwCheck  = 0;
  BaseType_t xToken;
  
  xToken = xQueueReceive(p_tDrv->xQue_RxHandle, p_uReadResponStr, dwWaitTime);
  if(xToken == pdFALSE)
  {
    dwCheck = OS_QUE_GET_FAIL;
    
    return dwCheck;
  }
  
  
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
