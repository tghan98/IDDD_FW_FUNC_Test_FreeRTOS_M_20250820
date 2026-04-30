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
#ifndef __HS_DBA_MODULE_H__
#define __HS_DBA_MODULE_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
    
//------------------------------------------    

enum HS_DBA_ACCESS_RNG
{
  HS_DBA_ACCESS_RNG_START = 0,
  HS_DBA_ACCESS_RNG_END,
  
  HS_DBA_ACCESS_RNG_MAX
};


enum HS_DBA_ACCESS_ATT
{
  HS_DBA_ACCESS_ATT_WR = 1,
  HS_DBA_ACCESS_ATT_RD,
  
  HS_DBA_ACCESS_ATT_MAX
};

//------------------------------------------
typedef struct
{
  uint16_t wAccess_Range[HS_DBA_ACCESS_RNG_MAX];
  SemaphoreHandle_t xMutex_Access_Key;
  uint32_t dwDataQuantity;
  uint32_t dwDataMemSizeToByte;
  uint32_t *p_DataBlockMem;
}hsDBA_t;


//function
int32_t hsDBA_Module_Config(hsDBA_t *p_tDBA_Handle, uint16_t wDBA_Start, uint16_t wDBA_End, uint32_t *p_dwMemBlock);

int32_t hsDBA_Read(hsDBA_t *p_tDBA_Handle, uint16_t wAccess_Address, uint32_t *p_RD_Mem);
int32_t hsDBA_Write(hsDBA_t *p_tDBA_Handle, uint16_t wAccess_Address, uint32_t dwWR_Data);

int32_t hsDBA_Block_Handling(hsDBA_t *p_tDBA_Handle, uint32_t dwAccess_ATT_ReadWrite, 
                             uint32_t dwStartAccessAddress, uint32_t dwAccessSize, uint32_t *p_dwAccessMem);

int32_t hsDBA_Data_Quantity_Read(hsDBA_t *p_tDBA_Handle, uint32_t *p_dwReadQuantity);
int32_t hsDBA_DataBlockSizeByte_Read(hsDBA_t *p_tDBA_Handle, uint32_t *p_dwReadMemSZByte);



#endif /* __HS_DBA_MODULE_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
