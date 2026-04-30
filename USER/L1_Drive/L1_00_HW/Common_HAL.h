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
#ifndef __HW_H__
#define __HW_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"


//Rework Numbering
enum REWORK_PP_ID
{
  RWRK_USART1_PP = 0,
  RWRK_USART2_PP,
  
  
  RWRK_I2C1_PP,
  RWRK_I2C2_PP,
  
  RWRK_ADC1_PP,
  
  RWRK_PP_MAX
};


enum PORT_NUM_USART
{
  PORT_NUM_USART1 = 0,
  PORT_NUM_USART2,
    
  PORT_NUM_USART_MAX
};

enum PORT_NUM_I2C
{
  PORT_NUM_I2C1 = 0,
  PORT_NUM_I2C2,
  
  PORT_NUM_I2C_MAX
};

//LED
enum BAT_LED_CH
{
  BAT_LED_CH1 = 0,
  BAT_LED_CH2,
  BAT_LED_CH3,
  
  BAT_LED_CH_MAX
};


enum RESULT_LED_CH
{
  RESULT_LED_POS = 0,
  RESULT_LED_NEG,
  
  RESULT_LED_MAX
};

    

//Function Type
int32_t hsHW_Config(void);


uint32_t BSP_hsSysTickTimer(uint32_t *p_TickTimer, uint32_t dwWaitTickTime);

void HW_LowCode_Error(uint32_t dwErrData);
void HW_Error_Handle(uint32_t dwONnOFF);




void HW_BAT_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF);
void HW_BLE_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF);
void HW_Error_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF);

void HW_CC_CK_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF);
void HW_Cassette_CK_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF);
void HW_Sample_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF);
void HW_Incubator_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF);

void HW_PW_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF);

void HW_Flu_A_LED(uint32_t dwResult_LED_Ch, uint32_t dwONnOFF);
void HW_Flu_B_LED(uint32_t dwResult_LED_Ch, uint32_t dwONnOFF);
void HW_COVID_LED(uint32_t dwResult_LED_Ch, uint32_t dwONnOFF);

void HW_All_LED_ONOFF(uint32_t dwONnOFF);



int32_t HW_DAC_CTRL(uint32_t dwDAC_12B_Count);

void *Read_UseADC_HalDrive(void);
void *Read_UseDMA_ADC_HalDrive(void);
void *Read_TRF_Timer_HalDrive(void);

void HW_USART_RX_Linker(uint32_t dwUSART_PORT_NUM, uint32_t dwFp);
void HW_USART_TX_Linker(uint32_t dwUSART_PORT_NUM, uint32_t dwFp);


void HW_BLE_Power_ONnOFF(uint32_t dwONnOFF);

int32_t HW_ReadPin_BLE_ConnectStatus(void);
int32_t HW_ReadPin_PWSW_Status(void);


void HW_Error_Malloc_Fail(void);
void HW_Error_StackOverflow(void);


#endif /* __HW_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
