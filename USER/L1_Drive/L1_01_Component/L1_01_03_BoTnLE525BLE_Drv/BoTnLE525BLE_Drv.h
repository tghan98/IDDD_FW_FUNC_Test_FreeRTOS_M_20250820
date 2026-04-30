/**
  ******************************************************************************
  * @file           : 
  * @brief          : 
  *                   
  ******************************************************************************
  ** 
  * 
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOTNLE525BLE_DRV_H__
#define __BOTNLE525BLE_DRV_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
    
//------------------------------------------    

#define BOTNLE523_RX_BUFF_SZ            64
#define BOTNLE523_TX_BUFF_SZ            64

#define BOTNLE523_RX_QUE_SZ             4

typedef struct
{
  Rework_PP_Drv_t *p_tReWork_HAL_Drv;
  uint32_t dwPortNum;
  
  uint32_t dwRx_Use_Flag;
  uint32_t dwRx_Index;
  uint8_t uRx_Byte;
  uint8_t uRx_Buff[BOTNLE523_RX_BUFF_SZ];
  
  
  uint32_t dwTx_Sz;  
  uint8_t uTx_Buff[BOTNLE523_TX_BUFF_SZ];
  
  
  SemaphoreHandle_t xSemaTxCMPL_Signal;
  QueueHandle_t xQue_RxHandle;
  
}BoTnLE523_Drv_t;

//------------------------------------------


//function
int32_t BotnLE_BLE_Config(BoTnLE523_Drv_t *p_tDrv, uint32_t dwReWork_ID, uint32_t dwPort_Num, uint32_t dwdwBaudRate);
void BoTnLE_BLE_UART_Link(BoTnLE523_Drv_t *p_tDrv);

int32_t BoTnLE_AT_Command_Send(BoTnLE523_Drv_t *p_tDrv, char *fmt, ...);
int32_t BoTnLE_AT_Command_Response(BoTnLE523_Drv_t *p_tDrv, uint8_t *p_uReadResponStr, uint32_t dwWaitTime);


void BoTLE_UART_RX_IRQ_Handler(uint32_t dwPortNum);
void BoTLE_UART_TX_IRQ_Handler(uint32_t dwPortNum);


#endif /* __BOTNLE525BLE_DRV_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
