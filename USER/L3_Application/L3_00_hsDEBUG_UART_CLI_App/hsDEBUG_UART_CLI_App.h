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
#ifndef __hsDEBUG_UART_CLI_App_H
#define __hsDEBUG_UART_CLI_App_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t hsDEBUG_UART_CLI_App_Config(void);
int32_t hsDEBUG_UART_TX_BlockingStart(void);
int32_t hsDEBUG_UART_TX_BlockingString(char *p_uStringData);
void hsDEBUG_UART_TX_BlockingEnd(void);


#endif /* __hsDEBUG_UART_CLI_App_H */

/* Includes ------------------------------------------------------------------*/
