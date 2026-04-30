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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PORT_DEBUG_UART_CLI_H
#define __PORT_DEBUG_UART_CLI_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

int32_t port_hsDEBUG_UART_CLI_Config(void *p_PP_DRV, uint32_t dwEN_nDIS);

void port_hsDEBUS_UART_CLI_IRQ_Linker(uint32_t dwFP_Rx, uint32_t dwFP_Tx);

int32_t port_hsDEBUG_UART_RX_Run(void *p_PP_DRV, uint8_t *p_uGetDataMem, uint32_t dwGetDataSize);

int32_t port_hsDEBUS_UART_TX_Run(void *p_PP_DRV, uint8_t *p_uPutDataMem, uint32_t dwPutDataSize);

#endif /* __PORT_DEBUG_UART_CLI_H */

/* Includes ------------------------------------------------------------------*/
