
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
#include "Display_Interface.h"

//HW

//Interface & Handle
#include "IDDD_DBA_RAM_Interface.h"


//-----------------------------------------------------------------------------
//define 

//OS Config

//Other

//Typedef
/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//OS Val

/* Private function prototypes -----------------------------------------------*/

/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_Disp_BLE_Status_Write(uint32_t dwStatus)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_RAM_Single_Write(IDDD_DBA_REG_ADDRESS_RAM_BLE_STA, dwStatus);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_Disp_BLE_Status_Read(uint32_t *p_dwStatus)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_RAM_Single_Read(IDDD_DBA_REG_ADDRESS_RAM_BLE_STA, p_dwStatus);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_Disp_BAT_Status_Write(uint32_t dwStatus)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_RAM_Single_Write(IDDD_DBA_REG_ADDRESS_RAM_BAT_STA, dwStatus);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_Disp_BAT_Status_Read(uint32_t *p_dwStatus)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_RAM_Single_Read(IDDD_DBA_REG_ADDRESS_RAM_BAT_STA, p_dwStatus);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_Disp_Status_Write(uint32_t dwStatus)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_RAM_Single_Write(IDDD_DBA_REG_ADDRESS_RAM_DISP_STA, dwStatus);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_Disp_Status_Read(uint32_t *p_dwStatus)
{
  int32_t dwCheck = 0;
  
  dwCheck = IDDD_DBA_REG_RAM_Single_Read(IDDD_DBA_REG_ADDRESS_RAM_DISP_STA, p_dwStatus);
  
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
  * @retval None pdTRUE 
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
