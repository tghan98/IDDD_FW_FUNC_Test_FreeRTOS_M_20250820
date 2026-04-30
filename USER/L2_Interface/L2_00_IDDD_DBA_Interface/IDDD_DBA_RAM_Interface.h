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
#ifndef __IDDD_DBA_RAM_IF_H__
#define __IDDD_DBA_RAM_IF_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "IDDD_DBA_RegisterAddress.h"        
//------------------------------------------    

//------------------------------------------


//function
int32_t IDDD_DBA_RAM_Interface_Config(void);


int32_t IDDD_DBA_REG_RAM_Single_Read(uint32_t dwDBA_REG_Address, uint32_t *p_dwReadMem);
int32_t IDDD_DBA_REG_RAM_Single_Write(uint32_t dwDBA_REG_Address, uint32_t dwWriteData);

int32_t IDDD_DBA_REG_RAM_Block_Read(uint32_t dwDBA_REG_StartAddress, uint32_t dwReadSize, uint32_t *p_dwReadMem);
int32_t IDDD_DBA_REG_RAM_Block_Write(uint32_t dwDBA_REG_StartAddress, uint32_t dwWriteSize, uint32_t *p_dwWriteMem);


#endif /* __IDDD_DBA_RAM_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
