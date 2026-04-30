
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
#include "IDDD_DBA_RAM_Interface.h"

//HW

//Interface & Handle
#include "hsDataBlockAccess_Module.h"

//-----------------------------------------------------------------------------
//define 

//OS Config

//Other

//Typedef
/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//OS Val

//Normal
uint32_t g_dwIDDD_DBA_RAM_MEM_Block[IDDD_DBA_REG_ADDRESS_RAM_MAX -  IDDD_DBA_REG_ADDRESS_RAM_START];
hsDBA_t g_tIDDD_DBA_RAM_Drv;

/* Private function prototypes -----------------------------------------------*/

/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_DBA_RAM_Interface_Config(void)
{
  int32_t dwCheck = 0;
    
  dwCheck = hsDBA_Module_Config( &g_tIDDD_DBA_RAM_Drv, IDDD_DBA_REG_ADDRESS_RAM_START, IDDD_DBA_REG_ADDRESS_RAM_MAX, g_dwIDDD_DBA_RAM_MEM_Block);
  if(dwCheck) return dwCheck;
    
  return dwCheck;
}

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
int32_t IDDD_DBA_REG_RAM_Single_Read(uint32_t dwDBA_REG_Address, uint32_t *p_dwReadMem)
{
  int32_t dwCheck = 0;
  
  dwCheck = hsDBA_Read( &g_tIDDD_DBA_RAM_Drv, (uint16_t)dwDBA_REG_Address, p_dwReadMem);
  
  return dwCheck;
  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None pdTRUE 
  */
int32_t IDDD_DBA_REG_RAM_Single_Write(uint32_t dwDBA_REG_Address, uint32_t dwWriteData)
{
  int32_t dwCheck = 0;
  
  dwCheck = hsDBA_Write( &g_tIDDD_DBA_RAM_Drv, (uint16_t)dwDBA_REG_Address, dwWriteData);
  
  return dwCheck;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_DBA_REG_RAM_Block_Read(uint32_t dwDBA_REG_StartAddress, uint32_t dwReadSize, uint32_t *p_dwReadMem)
{
  int32_t dwCheck = 0;
  
  dwCheck = hsDBA_Block_Handling( &g_tIDDD_DBA_RAM_Drv, HS_DBA_ACCESS_ATT_RD, dwDBA_REG_StartAddress, dwReadSize, p_dwReadMem);
  
  return dwCheck;
  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_DBA_REG_RAM_Block_Write(uint32_t dwDBA_REG_StartAddress, uint32_t dwWriteSize, uint32_t *p_dwWriteMem)
{
  int32_t dwCheck = 0;
  
  dwCheck = hsDBA_Block_Handling( &g_tIDDD_DBA_RAM_Drv, HS_DBA_ACCESS_ATT_WR, dwDBA_REG_StartAddress, dwWriteSize, p_dwWriteMem);
  
  return dwCheck;
  
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

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */   

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
