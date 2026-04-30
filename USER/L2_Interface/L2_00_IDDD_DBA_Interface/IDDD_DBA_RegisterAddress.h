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
#ifndef __IDDD_DBA_REG_ADDRESS_H__
#define __IDDD_DBA_REG_ADDRESS_H__

/* Includes ------------------------------------------------------------------*/
    
//------------------------------------------    
enum IDDD_DBA_REG_ADDRESS_RAM
{
  IDDD_DBA_REG_ADDRESS_RAM_START = 100,
  
  IDDD_DBA_REG_ADDRESS_RAM_SEL_OPT_CH,
  
  IDDD_DBA_REG_ADDRESS_RAM_SEL_LED_CH,
  IDDD_DBA_REG_ADDRESS_RAM_SEL_PD_CH,
  
  IDDD_DBA_REG_ADDRESS_RAM_BLE_STA,
  IDDD_DBA_REG_ADDRESS_RAM_BAT_STA,
  IDDD_DBA_REG_ADDRESS_RAM_DISP_STA,
  
  IDDD_DBA_REG_ADDRESS_RAM_MAX
};

enum IDDD_DBA_REG_ADDRESS_ROM
{
  IDDD_DBA_REG_ADDRESS_ROM_START = 200,
  IDDD_DBA_REG_ROM_LED1,
  IDDD_DBA_REG_ROM_LED2,
  IDDD_DBA_REG_ROM_LED3,
  IDDD_DBA_REG_ROM_LED4,
  IDDD_DBA_REG_ROM_LED5,
  IDDD_DBA_REG_ROM_LED6,
  IDDD_DBA_REG_ROM_LED7,
  
  IDDD_DBA_REG_ROM_PD1,
  IDDD_DBA_REG_ROM_PD2,
  IDDD_DBA_REG_ROM_PD3,
  IDDD_DBA_REG_ROM_PD4,
  IDDD_DBA_REG_ROM_PD5,
  IDDD_DBA_REG_ROM_PD6,
  IDDD_DBA_REG_ROM_PD7,
  
  IDDD_DBA_REG_LED_ON_TIME,
  IDDD_DBA_REG_PD_SCAN_TIME,
  
  IDDD_DBA_REG_ADDRESS_ROM_MAX
};


//------------------------------------------


//function


#endif /* __IDDD_DBA_REG_ADDRESS_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
