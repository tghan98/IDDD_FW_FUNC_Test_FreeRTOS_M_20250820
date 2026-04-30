/**
  ******************************************************************************
  * @file    main.h 
  * @author  KMAC Electronic Controll Team
  * @version V1.0.0
  * @date    2014 10 14
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  *
  *
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HS_EEPROM_PORT_H
#define __HS_EEPROM_PORT_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"



/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

int32_t port_hsEEPROM_Write(void *p_IF_Dev, uint16_t wDevAddress, uint16_t wMemAddress, uint16_t wMemAddSize, uint8_t *p_uWriteBuf, uint16_t wWriteData_Size);
int32_t port_hsEEPROM_Read(void *p_IF_Dev, uint16_t wDevAddress, uint16_t wMemAddress, uint16_t wMemAddSize, uint8_t *p_uReadBuf, uint16_t wReadData_Size);

#endif /* __HS_EEPROM_PORT_H */

/* Includes ------------------------------------------------------------------*/
