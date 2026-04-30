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
#ifndef __BoTnLE525BLE_PORT_H__
#define __BoTnLE525BLE_PORT_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
    
//------------------------------------------    

//------------------------------------------


//function
int32_t port_hsBoTnL525BLE_UART_Config(void *p_PP_DRV, uint32_t dwBuadRate, uint32_t dwEN_nDIS);

void port_hsBoTnL525BLE_UART_IRQ_Linker(uint32_t dwPortNum, uint32_t dwFP_Rx, uint32_t dwFP_Tx);

int32_t port_hsBoTnL525BLE_UART_RX_Run(void *p_PP_DRV, uint8_t *p_uGetDataMem, uint32_t dwGetDataSize);
int32_t port_hsBoTnL525BLE_UART_TX_Run(void *p_PP_DRV, uint8_t *p_uPutDataMem, uint32_t dwPutDataSize);

int32_t port_hsBoTnL525BLE_UART_RX_Stop(void *p_PP_DRV);


#endif /* __BoTnLE525BLE_PORT_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
