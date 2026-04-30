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
#ifndef __IDDD_DBA_ROM_IF_H__
#define __IDDD_DBA_ROM_IF_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "IDDD_DBA_RegisterAddress.h"    
//------------------------------------------    


enum IDDD_DBA_ROM_ADD
{
  IDDD_DBA_ROM_ADD_START = 0,
  IDDD_DBA_ROM_ADD_2,
  IDDD_DBA_ROM_ADD_3,  
  IDDD_DBA_ROM_ADD_MAX
};
    
//------------------------------------------


//function
int32_t IDDD_DBA_ROM_Interface_Config(void);

int32_t IDDD_DBA_ROM_Loading(void);
int32_t IDDD_DBA_ROM_Default_Data_Setting(void);

int32_t IDDD_DBA_REG_ROM_Single_Read(uint32_t dwDBA_REG_Address, uint32_t *p_dwReadMem);
int32_t IDDD_DBA_REG_ROM_Single_Write(uint32_t dwDBA_REG_Address, uint32_t dwWriteData);
int32_t IDDD_DBA_REG_ROM_Block_Read(uint32_t dwDBA_REG_StartAddress, uint32_t dwReadSize, uint32_t *p_dwReadMem);
int32_t IDDD_DBA_REG_ROM_Block_Write(uint32_t dwDBA_REG_StartAddress, uint32_t dwWriteSize, uint32_t *p_dwWriteMem);

int32_t IDDD_DBA_REG_ROM_Single_Load(uint32_t dwDBA_REG_Address, uint32_t *p_dwLoadDatMem);
int32_t IDDD_DBA_REG_ROM_Single_Save(uint32_t dwDBA_REG_Address);
int32_t IDDD_DBA_REG_ROM_Load(void);
int32_t IDDD_DBA_REG_ROM_Save(void);


int32_t IDDD_DBA_ROM_Loading(void);
int32_t IDDD_DBA_ROM_Default_Data_Setting(void);


#endif /* __IDDD_DBA_ROM_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
