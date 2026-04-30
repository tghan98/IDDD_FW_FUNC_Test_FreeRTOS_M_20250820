
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
#include "IDDD_DBA_ROM_Interface.h"

//HW
#include "hsDataBlockAccess_Module.h"
#include "hsEEPROM_Drv.h"

//Interface & Handle


//-----------------------------------------------------------------------------
//define 
#define ROM_DEV_ADDRESS               (0xA2)  
#define ROM_SZ_MAX                    (32768L)


#define ROM_START_ADDRESS             (20L)


#define ROM_USE_CHECK_VAL             (250620L)
//OS Config
#define ROM_ACCESS_TIME_OUT           (500L)


//Other

//Typedef
/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//OS Val

//Normal Val
uint32_t g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ADDRESS_ROM_MAX -  IDDD_DBA_REG_ADDRESS_ROM_START];

hsDBA_t g_tIDDD_DBA_ROM_Drv;
hsEEPROM_DRV_Type g_tIDDD_EEPROM_Drv;

uint32_t g_dwIDDD_DBA_ROM_ADD_Arry[IDDD_DBA_ROM_ADD_MAX];

/* Private function prototypes -----------------------------------------------*/

/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_DBA_ROM_Interface_Config(void)
{
  int32_t dwCheck = 0;
  uint32_t dwROM_Size;
    
  dwCheck = hsDBA_Module_Config( &g_tIDDD_DBA_ROM_Drv, IDDD_DBA_REG_ADDRESS_ROM_START, IDDD_DBA_REG_ADDRESS_ROM_MAX, g_dwIDDD_DBA_ROM_MEM_Block);
  if(dwCheck) return dwCheck;
  
  
  dwCheck = hsEEROM_DRV_Config( &g_tIDDD_EEPROM_Drv, RWRK_I2C1_PP, ROM_DEV_ADDRESS, 64, ROM_SZ_MAX, 2, 10);
  if(dwCheck) return dwCheck;

  dwCheck = hsDBA_DataBlockSizeByte_Read( &g_tIDDD_DBA_ROM_Drv, &dwROM_Size);
  if(dwCheck) return dwCheck;
  
  g_dwIDDD_DBA_ROM_ADD_Arry[IDDD_DBA_ROM_ADD_START] = ROM_START_ADDRESS;
  g_dwIDDD_DBA_ROM_ADD_Arry[IDDD_DBA_ROM_ADD_2] = g_dwIDDD_DBA_ROM_ADD_Arry[IDDD_DBA_ROM_ADD_START] + dwROM_Size;
  
  return dwCheck;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_DBA_ROM_Loading(void)
{
  int32_t dwCheck = 0;
  uint32_t dwReadUseROMCheck = 0;
  
  dwCheck =  IDDD_DBA_REG_ROM_Load();
  if(dwCheck) return dwCheck;
  
  dwCheck = IDDD_DBA_REG_ROM_Single_Read(IDDD_DBA_REG_ADDRESS_ROM_START, &dwReadUseROMCheck);
  
  if(dwReadUseROMCheck != ROM_USE_CHECK_VAL)
  {
    dwCheck = IDDD_DBA_ROM_Default_Data_Setting();
    if(dwCheck) return dwCheck;
    
    dwCheck = IDDD_DBA_REG_ROM_Save();
    if(dwCheck) return dwCheck;
  }
  
  return dwCheck;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_DBA_ROM_Default_Data_Setting(void)
{
  BaseType_t xToken;
  int32_t dwCheck = 0;
  
  xToken = xSemaphoreTake(g_tIDDD_DBA_ROM_Drv.xMutex_Access_Key, ROM_ACCESS_TIME_OUT);
  if(xToken == pdFALSE) return OS_ACCES_FAIL;
  
  g_dwIDDD_DBA_ROM_MEM_Block[0] = ROM_USE_CHECK_VAL;

  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_LED1 - IDDD_DBA_REG_ADDRESS_ROM_START] = 300;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_LED2 - IDDD_DBA_REG_ADDRESS_ROM_START] = 300;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_LED3 - IDDD_DBA_REG_ADDRESS_ROM_START] = 300;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_LED4 - IDDD_DBA_REG_ADDRESS_ROM_START] = 300;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_LED5 - IDDD_DBA_REG_ADDRESS_ROM_START] = 300;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_LED6 - IDDD_DBA_REG_ADDRESS_ROM_START] = 300;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_LED7 - IDDD_DBA_REG_ADDRESS_ROM_START] = 300;


  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_PD1 - IDDD_DBA_REG_ADDRESS_ROM_START] = 0;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_PD2 - IDDD_DBA_REG_ADDRESS_ROM_START] = 0;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_PD3 - IDDD_DBA_REG_ADDRESS_ROM_START] = 0;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_PD4 - IDDD_DBA_REG_ADDRESS_ROM_START] = 0;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_PD5 - IDDD_DBA_REG_ADDRESS_ROM_START] = 0;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_PD6 - IDDD_DBA_REG_ADDRESS_ROM_START] = 0;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_ROM_PD7 - IDDD_DBA_REG_ADDRESS_ROM_START] = 0;
  
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_LED_ON_TIME - IDDD_DBA_REG_ADDRESS_ROM_START] = 1000;
  g_dwIDDD_DBA_ROM_MEM_Block[IDDD_DBA_REG_PD_SCAN_TIME - IDDD_DBA_REG_ADDRESS_ROM_START] = 1200;

  
   xSemaphoreGive(g_tIDDD_DBA_ROM_Drv.xMutex_Access_Key);
   
  return dwCheck;  
}


//-----------------------------------------------------------------------------
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_DBA_REG_ROM_Single_Read(uint32_t dwDBA_REG_Address, uint32_t *p_dwReadMem)
{
  int32_t dwCheck = 0;
  
  dwCheck = hsDBA_Read( &g_tIDDD_DBA_ROM_Drv, (uint16_t)dwDBA_REG_Address, p_dwReadMem);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_DBA_REG_ROM_Single_Write(uint32_t dwDBA_REG_Address, uint32_t dwWriteData)
{
  int32_t dwCheck = 0;
  
  dwCheck = hsDBA_Write( &g_tIDDD_DBA_ROM_Drv, (uint16_t)dwDBA_REG_Address, dwWriteData);
  
  return dwCheck;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_DBA_REG_ROM_Block_Read(uint32_t dwDBA_REG_StartAddress, uint32_t dwReadSize, uint32_t *p_dwReadMem)
{
  int32_t dwCheck = 0;
  
  dwCheck = hsDBA_Block_Handling( &g_tIDDD_DBA_ROM_Drv, HS_DBA_ACCESS_ATT_RD, dwDBA_REG_StartAddress, dwReadSize, p_dwReadMem);
  
  return dwCheck;
  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_DBA_REG_ROM_Block_Write(uint32_t dwDBA_REG_StartAddress, uint32_t dwWriteSize, uint32_t *p_dwWriteMem)
{
  int32_t dwCheck = 0;
  
  dwCheck = hsDBA_Block_Handling( &g_tIDDD_DBA_ROM_Drv, HS_DBA_ACCESS_ATT_WR, dwDBA_REG_StartAddress, dwWriteSize, p_dwWriteMem);
  
  return dwCheck;
  
}



//----------------------------------------------------------------------------
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_DBA_REG_ROM_Single_Load(uint32_t dwDBA_REG_Address, uint32_t *p_dwLoadDatMem)
{
  int32_t dwCheck = 0;
  uint32_t dwIndex, dwROM_Address, dwLoadMem;
  
  if( (dwDBA_REG_Address < IDDD_DBA_REG_ADDRESS_ROM_START) ||
      (dwDBA_REG_Address >= IDDD_DBA_REG_ADDRESS_ROM_MAX) )
  {
    return DAT_ERR_PARAM_DATA;
  }
  
  dwIndex = dwDBA_REG_Address - IDDD_DBA_REG_ADDRESS_ROM_START;
  dwROM_Address = g_dwIDDD_DBA_ROM_ADD_Arry[IDDD_DBA_ROM_ADD_START] + (dwIndex * 4);
  
  dwCheck = hsEEPROM_Read(&g_tIDDD_EEPROM_Drv, dwROM_Address, (uint8_t *)&dwLoadMem, sizeof(uint32_t));
  if(dwCheck) return dwCheck;
  
  dwCheck = hsDBA_Write(&g_tIDDD_DBA_ROM_Drv, (uint16_t)dwDBA_REG_Address, dwLoadMem);
  if(dwCheck) return dwCheck;
  
  *p_dwLoadDatMem = dwLoadMem;
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_DBA_REG_ROM_Single_Save(uint32_t dwDBA_REG_Address)
{
  int32_t dwCheck = 0;
  uint32_t dwIndex, dwROM_Address, dwSavedMem;
  
  if( (dwDBA_REG_Address < IDDD_DBA_REG_ADDRESS_ROM_START) ||
      (dwDBA_REG_Address >= IDDD_DBA_REG_ADDRESS_ROM_MAX) )
  {
    return DAT_ERR_PARAM_DATA;
  }
  
  dwIndex = dwDBA_REG_Address - IDDD_DBA_REG_ADDRESS_ROM_START;
  dwROM_Address = g_dwIDDD_DBA_ROM_ADD_Arry[IDDD_DBA_ROM_ADD_START] + (dwIndex * 4);
  
  dwCheck = hsDBA_Read(&g_tIDDD_DBA_ROM_Drv, (uint16_t)dwDBA_REG_Address, &dwSavedMem);
  if(dwCheck) return dwCheck;
  
  dwCheck = hsEEROM_Write(&g_tIDDD_EEPROM_Drv, (uint16_t)dwROM_Address, (uint8_t *)&dwSavedMem, sizeof(uint32_t));
  if(dwCheck) return dwCheck;
    
  return dwCheck;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None ROM_ACCESS_TIME_OUT
  */   
int32_t IDDD_DBA_REG_ROM_Load(void)
{
  BaseType_t xToken;
  int32_t dwCheck = 0;
  
  xToken = xSemaphoreTake(g_tIDDD_DBA_ROM_Drv.xMutex_Access_Key, ROM_ACCESS_TIME_OUT);
  if(xToken == pdFALSE) return OS_ACCES_FAIL;
  
  dwCheck = hsEEPROM_Read(&g_tIDDD_EEPROM_Drv, g_dwIDDD_DBA_ROM_ADD_Arry[IDDD_DBA_ROM_ADD_START], 
                          (uint8_t *)g_tIDDD_DBA_ROM_Drv.p_DataBlockMem, (uint16_t)g_tIDDD_DBA_ROM_Drv.dwDataMemSizeToByte);
  if(dwCheck) goto IDDD_DBA_REG_ROM_LD_EXIT;
  
  
IDDD_DBA_REG_ROM_LD_EXIT:
  xSemaphoreGive(g_tIDDD_DBA_ROM_Drv.xMutex_Access_Key);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */   
int32_t IDDD_DBA_REG_ROM_Save(void)
{
  BaseType_t xToken;
  int32_t dwCheck = 0;
  
  xToken = xSemaphoreTake(g_tIDDD_DBA_ROM_Drv.xMutex_Access_Key, ROM_ACCESS_TIME_OUT);
  if(xToken == pdFALSE) return OS_ACCES_FAIL;
  
  dwCheck = hsEEROM_Write(&g_tIDDD_EEPROM_Drv, g_dwIDDD_DBA_ROM_ADD_Arry[IDDD_DBA_ROM_ADD_START], 
                          (uint8_t *)g_tIDDD_DBA_ROM_Drv.p_DataBlockMem, (uint16_t)g_tIDDD_DBA_ROM_Drv.dwDataMemSizeToByte);
  if(dwCheck) goto IDDD_DBA_REG_ROM_LD_EXIT;
  
  
IDDD_DBA_REG_ROM_LD_EXIT:
  xSemaphoreGive(g_tIDDD_DBA_ROM_Drv.xMutex_Access_Key);
  
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


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */   

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
