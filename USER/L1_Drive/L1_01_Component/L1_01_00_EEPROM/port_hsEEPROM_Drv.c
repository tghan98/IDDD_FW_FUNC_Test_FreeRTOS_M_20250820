/**
  ******************************************************************************
  * @file    main.c 
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

/* Includes ------------------------------------------------------------------*/
#include "port_hsEEPROM_Drv.h"

//HW Config
//#include "PP_BD_v2_0_HW.h"


//Public 
//Extern variable


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define hsEEPROM_TIME_OUT       500


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t port_hsEEPROM_Write(void *p_IF_Dev, uint16_t wDevAddress, uint16_t wMemAddress, uint16_t wMemAddSize, uint8_t *p_uWriteBuf, uint16_t wWriteData_Size)
{
  int32_t dwCheck;
  
  dwCheck = HAL_I2C_Mem_Write(p_IF_Dev, wDevAddress, wMemAddress, wMemAddSize, p_uWriteBuf, wWriteData_Size, hsEEPROM_TIME_OUT);
  
  return dwCheck;
}


/**
  * @brief  
  * @param  None
  * @retval None  
*/
int32_t port_hsEEPROM_Read(void *p_IF_Dev, uint16_t wDevAddress, uint16_t wMemAddress, uint16_t wMemAddSize, uint8_t *p_uReadBuf, uint16_t wReadData_Size)
{
  int32_t dwCheck;
  
  dwCheck = HAL_I2C_Mem_Read(p_IF_Dev, wDevAddress, wMemAddress, wMemAddSize, p_uReadBuf, wReadData_Size, hsEEPROM_TIME_OUT);
  
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

