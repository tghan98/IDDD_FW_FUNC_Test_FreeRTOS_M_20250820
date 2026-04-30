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
#ifndef __HS_EEPROM_DRV_H__
#define __HS_EEPROM_DRV_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

typedef struct
{
  //Remapping
  //MCU_ReMapping_Interface_Type *p_Remapping_Dev_Handle;
  Rework_PP_Drv_t *p_Rework_PP_Drv;
  uint32_t dwDev_ID;
  
  //EEPROM Infor
  uint8_t uROM_DEV_Address;
  uint32_t dwROM_Page_Size;
  uint32_t dwROM_Max_Size;
  uint32_t dwROM_Address_Size;
  uint32_t dwROM_Page_Wrie_Delay_MS;
  
  //OS
  
  //Control 
  uint8_t *p_uBuf;
  uint32_t dwBuf_Cnt;
  uint32_t dwRealROM_Address_Cnt;
  uint32_t dwAccessData_Cnt;
  uint32_t dwRealROM_Address_Cnt_Max;
  uint32_t dwWriteMemAddress;
  
}hsEEPROM_DRV_Type;
    

//Function ----------------------------------------------------

int32_t hsEEROM_DRV_Config(hsEEPROM_DRV_Type *p_tDRV, uint32_t dwDev_ID, 
                        uint8_t uROM_DEV_Address, uint32_t dwPage_Size, uint32_t dwROM_MAX_Size, uint32_t dwROM_Add_Size, uint32_t dwPage_WR_Delay);

int32_t hsEEROM_Write(hsEEPROM_DRV_Type *p_tDRV, uint16_t wStore_StartAddress, uint8_t *p_uWrite, uint16_t wWrite_Size);

int32_t hsEEPROM_Read(hsEEPROM_DRV_Type *p_tDRV, uint16_t wRead_StartAddress, uint8_t *p_uReadBuf, uint16_t wRead_Size);




#endif /* __HS_EEPROM_DRV_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
