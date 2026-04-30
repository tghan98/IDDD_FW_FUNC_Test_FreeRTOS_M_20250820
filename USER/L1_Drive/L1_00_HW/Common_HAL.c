
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
#include "Common_HAL.h"

//Define ---------------------------------------------------------------------

    
/* Extern --------------------------------------------------------------------*/


extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;



extern DAC_HandleTypeDef hdac1;


extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern TIM_HandleTypeDef htim3;

/* Private variables ---------------------------------------------------------*/
uint32_t g_dwMainTickCnt;
uint32_t g_dwInitError_Pos;

void (*gFp_Uart_Rx_Callback[PORT_NUM_USART_MAX])(uint32_t dwCOM_Port);
void (*gFp_Uart_Tx_Callback[PORT_NUM_USART_MAX])(uint32_t dwCOM_Port);

/* Private function prototypes -----------------------------------------------*/

//Macro Function


/* Interrupt  ----------------------------------------------------------------*/
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  
  switch( (uint32_t)(huart->Instance))
  {
  case (uint32_t)USART1:
    
    if( gFp_Uart_Rx_Callback[PORT_NUM_USART1] != NULL)
    {
      (gFp_Uart_Rx_Callback[PORT_NUM_USART1])(PORT_NUM_USART1);
    }
    
    break;

  case (uint32_t)USART2:
    
    if( gFp_Uart_Rx_Callback[PORT_NUM_USART2] != NULL)
    {
      (gFp_Uart_Rx_Callback[PORT_NUM_USART2])(PORT_NUM_USART2);
    }
    
    break;
    
  default:
    
    HW_LowCode_Error(SET);
    
    break;
  }
  
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  
  switch( (uint32_t)(huart->Instance))
  {
  case (uint32_t)USART1:
    
    if(gFp_Uart_Tx_Callback[PORT_NUM_USART1] != NULL)
    {
      (gFp_Uart_Tx_Callback[PORT_NUM_USART1])(PORT_NUM_USART1);
    }
    
    break;

  case (uint32_t)USART2:
    
    if(gFp_Uart_Tx_Callback[PORT_NUM_USART2] != NULL)
    {
      (gFp_Uart_Tx_Callback[PORT_NUM_USART2])(PORT_NUM_USART2);
    }
    
    break;
    
  default:
    
    HW_LowCode_Error(SET);
    
    break;
  }  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None

  */



/*Function -------------------------------------------------------------------*/


//------------------------------------------------------------------------------
//HAL Delay 
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HAL_IncTick(void)
{
  g_dwMainTickCnt += uwTickFreq;
}

/**
  * @brief  This 
  * @param  file:  0(USART1) ~ 4 (USART5)
  * @param  line: 
  * @retval None
  */
void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY)
  {
    wait += (uint32_t)(uwTickFreq);
  }

  while ((HAL_GetTick() - tickstart) < wait)
  {
  }
  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
uint32_t HAL_GetTick(void)
{
  return g_dwMainTickCnt;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// HW Dependent code **********************************************************
//------------------------------------------------------------------------------

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval    
*/
int32_t hsHW_Config(void)
{
  int32_t dwCheck = 0;
  //Rework Peripheral Config
  
  //USART
  dwCheck = Rework_PP_Drv_Config(&huart1, RWRK_USART1_PP);
  if(dwCheck) return dwCheck;

  dwCheck = Rework_PP_Drv_Config(&huart2, RWRK_USART2_PP);
  if(dwCheck) return dwCheck;
  
  //I2C
  dwCheck = Rework_PP_Drv_Config(&hi2c1, RWRK_I2C1_PP);
  if(dwCheck) return dwCheck;
  
  dwCheck = Rework_PP_Drv_Config(&hi2c2, RWRK_I2C2_PP);
  if(dwCheck) return dwCheck;
  
  
  dwCheck = Rework_PP_Drv_Config(&hadc1, RWRK_ADC1_PP);
  if(dwCheck) return dwCheck;
  
  // Other Device Config
  hsGlobalErrorUseConfig( (uint32_t)HW_Error_Handle);
  HW_All_LED_ONOFF(ON);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_BAT_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF)
{
  if(dwCh >= BAT_LED_CH_MAX) dwCh = BAT_LED_CH3;
  
  switch(dwCh)
  {
  case BAT_LED_CH1:
    
    if(dwONnOFF) BAT_G1_GPIO_Port->BSRR = (uint32_t)BAT_G1_Pin;
    else BAT_G1_GPIO_Port->BRR = (uint32_t)BAT_G1_Pin;
    
    break;
  case BAT_LED_CH2:

    if(dwONnOFF) BAT_G2_GPIO_Port->BSRR = (uint32_t)BAT_G2_Pin;
    else BAT_G2_GPIO_Port->BRR = (uint32_t)BAT_G2_Pin;

    break;
  default:

    if(dwONnOFF) BAT_G3_GPIO_Port->BSRR = (uint32_t)BAT_G3_Pin;
    else BAT_G3_GPIO_Port->BRR = (uint32_t)BAT_G3_Pin;

    break;
    
  }
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_BLE_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF)
{
  dwCh++;
  
  if(dwONnOFF) BLE_G_GPIO_Port->BRR = (uint32_t)BLE_G_Pin;
  else BLE_G_GPIO_Port->BSRR = (uint32_t)BLE_G_Pin;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_Error_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF)
{
  dwCh++;
  
  if(dwONnOFF) Prog_R1_GPIO_Port->BSRR = (uint32_t)Prog_R1_Pin;
  else Prog_R1_GPIO_Port->BRR = (uint32_t)Prog_R1_Pin;
  
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_CC_CK_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF)
{
  dwCh++;
  
  if(dwONnOFF) CC_CK_G_GPIO_Port->BRR = (uint32_t)CC_CK_G_Pin;
  else CC_CK_G_GPIO_Port->BSRR = (uint32_t)CC_CK_G_Pin;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_Cassette_CK_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF)
{
  dwCh++;
  
  if(dwONnOFF) Prog_G1_GPIO_Port->BSRR = (uint32_t)Prog_G1_Pin;
  else Prog_G1_GPIO_Port->BRR = (uint32_t)Prog_G1_Pin;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_Sample_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF)
{
  dwCh++;
  
  if(dwONnOFF) Prog_G2_GPIO_Port->BSRR = (uint32_t)Prog_G2_Pin;
  else Prog_G2_GPIO_Port->BRR = (uint32_t)Prog_G2_Pin;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_Incubator_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF)
{
  dwCh++;
  
  if(dwONnOFF) Prog_G3_GPIO_Port->BSRR = (uint32_t)Prog_G3_Pin;
  else Prog_G3_GPIO_Port->BRR = (uint32_t)Prog_G3_Pin;  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_PW_LED_ONnOFF(uint32_t dwCh, uint32_t dwONnOFF)
{
  dwCh++;
  
  if(dwONnOFF) PW_G_GPIO_Port->BSRR = (uint32_t)PW_G_Pin;
  else PW_G_GPIO_Port->BRR = (uint32_t)PW_G_Pin;  
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_Flu_A_LED(uint32_t dwResult_LED_Ch, uint32_t dwONnOFF)
{
  
  if(dwResult_LED_Ch >= RESULT_LED_MAX) dwResult_LED_Ch = RESULT_LED_NEG;
  
  switch(dwResult_LED_Ch)
  {
  case RESULT_LED_POS:
    if(dwONnOFF) FLU_A_R_GPIO_Port->BSRR = (uint32_t)FLU_A_R_Pin;
    else FLU_A_R_GPIO_Port->BRR = (uint32_t)FLU_A_R_Pin;
    
    break;
  default:
    if(dwONnOFF) FLU_A_G_GPIO_Port->BSRR = (uint32_t)FLU_A_G_Pin;
    else FLU_A_G_GPIO_Port->BRR = (uint32_t)FLU_A_G_Pin;
    
    break;
  }
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_Flu_B_LED(uint32_t dwResult_LED_Ch, uint32_t dwONnOFF)
{
  
  if(dwResult_LED_Ch >= RESULT_LED_MAX) dwResult_LED_Ch = RESULT_LED_NEG;
  
  switch(dwResult_LED_Ch)
  {
  case RESULT_LED_POS:
    if(dwONnOFF) FLU_B_R_GPIO_Port->BSRR = (uint32_t)FLU_B_R_Pin;
    else FLU_B_R_GPIO_Port->BRR = (uint32_t)FLU_B_R_Pin;
    
    break;
  default:
    if(dwONnOFF) FLU_B_G_GPIO_Port->BSRR = (uint32_t)FLU_B_G_Pin;
    else FLU_B_G_GPIO_Port->BRR = (uint32_t)FLU_B_G_Pin;
    
    break;
  }
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_COVID_LED(uint32_t dwResult_LED_Ch, uint32_t dwONnOFF)
{
  
  if(dwResult_LED_Ch >= RESULT_LED_MAX) dwResult_LED_Ch = RESULT_LED_NEG;
  
  switch(dwResult_LED_Ch)
  {
  case RESULT_LED_POS:
    if(dwONnOFF) COVID_R_GPIO_Port->BSRR = (uint32_t)COVID_R_Pin;
    else COVID_R_GPIO_Port->BRR = (uint32_t)COVID_R_Pin;
    
    break;
  default:
    if(dwONnOFF) COVID_G_GPIO_Port->BSRR = (uint32_t)COVID_G_Pin;
    else COVID_G_GPIO_Port->BRR = (uint32_t)COVID_G_Pin;
    
    break;
  }
}

//------------------------------------------------------------------------------
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_All_LED_ONOFF(uint32_t dwONnOFF)
{
  if(dwONnOFF)
  {
    HW_BAT_LED_ONnOFF(BAT_LED_CH1, ON);
    HW_BAT_LED_ONnOFF(BAT_LED_CH2, ON);
    HW_BAT_LED_ONnOFF(BAT_LED_CH3, ON);
    
    
    HW_BLE_LED_ONnOFF(0, ON);
    
    HW_Error_LED_ONnOFF(0, ON);
    
    HW_CC_CK_LED_ONnOFF(0, ON);
    
    HW_Cassette_CK_LED_ONnOFF(0, ON);
    HW_Sample_LED_ONnOFF(0, ON);
    HW_Incubator_LED_ONnOFF(0, ON);
    
    HW_PW_LED_ONnOFF(0, ON);
    
    HW_Flu_A_LED(RESULT_LED_POS, ON);
    HW_Flu_A_LED(RESULT_LED_NEG, ON);
    
    HW_Flu_B_LED(RESULT_LED_POS, ON);
    HW_Flu_B_LED(RESULT_LED_NEG, ON);
    
    HW_COVID_LED(RESULT_LED_POS, ON);
    HW_COVID_LED(RESULT_LED_NEG, ON);
  }
  else
  {
    HW_BAT_LED_ONnOFF(BAT_LED_CH1, OFF);
    HW_BAT_LED_ONnOFF(BAT_LED_CH2, OFF);
    HW_BAT_LED_ONnOFF(BAT_LED_CH3, OFF);
    
    
    HW_BLE_LED_ONnOFF(0, OFF);
    
    HW_Error_LED_ONnOFF(0, OFF);
    
    HW_CC_CK_LED_ONnOFF(0, OFF);
    
    HW_Cassette_CK_LED_ONnOFF(0, OFF);
    HW_Sample_LED_ONnOFF(0, OFF);
    HW_Incubator_LED_ONnOFF(0, OFF);
    
    HW_PW_LED_ONnOFF(0, OFF);
    
    HW_Flu_A_LED(RESULT_LED_POS, OFF);
    HW_Flu_A_LED(RESULT_LED_NEG, OFF);
    
    HW_Flu_B_LED(RESULT_LED_POS, OFF);
    HW_Flu_B_LED(RESULT_LED_NEG, OFF);
    
    HW_COVID_LED(RESULT_LED_POS, OFF);
    HW_COVID_LED(RESULT_LED_NEG, OFF);    
  }
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t HW_DAC_CTRL(uint32_t dwDAC_12B_Count)
{
  int32_t dwCheck = 0;
  
  dwCheck = HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dwDAC_12B_Count);
  
  return dwCheck;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void *Read_UseADC_HalDrive(void)
{
  return (void *)&hadc1;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void *Read_UseDMA_ADC_HalDrive(void)
{
  return (void *)&hdma_adc1;
}



/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void *Read_TRF_Timer_HalDrive(void)
{
  return (void *)&htim3;
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

void HW_USART_RX_Linker(uint32_t dwUSART_PORT_NUM, uint32_t dwFp)
{
  if(dwUSART_PORT_NUM >= PORT_NUM_USART_MAX) dwUSART_PORT_NUM = PORT_NUM_USART2;
  
  gFp_Uart_Rx_Callback[dwUSART_PORT_NUM] = (void(*)(uint32_t))dwFp;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_USART_TX_Linker(uint32_t dwUSART_PORT_NUM, uint32_t dwFp)
{
  if(dwUSART_PORT_NUM >= PORT_NUM_USART_MAX) dwUSART_PORT_NUM = PORT_NUM_USART2;
  
  gFp_Uart_Tx_Callback[dwUSART_PORT_NUM] = (void(*)(uint32_t))dwFp;
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_BLE_Power_ONnOFF(uint32_t dwONnOFF)
{
  if(dwONnOFF)
  {
    BLE_PW_nONOFF_GPIO_Port->BRR = (uint32_t)BLE_PW_nONOFF_Pin;
  }
  else
  {
    BLE_PW_nONOFF_GPIO_Port->BSRR = (uint32_t)BLE_PW_nONOFF_Pin;
  }
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t HW_ReadPin_BLE_ConnectStatus(void)
{
  if(BLE_C_CK_GPIO_Port->IDR & BLE_C_CK_Pin)
  {
    return SET;
  }
  
  return RESET;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t HW_ReadPin_PWSW_Status(void)
{
  if(PC_SW_SIG_GPIO_Port->IDR & PC_SW_SIG_Pin)
  {
    return SET;
  }
  
  return RESET;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */



//==============================================================================
// UTILL

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
uint32_t BSP_hsSysTickTimer(uint32_t *p_TickTimer, uint32_t dwWaitTickTime)
{
  uint32_t dwGetNowTick;
  
  dwGetNowTick = HAL_GetTick();
  
  if(*p_TickTimer == 0)
  {
    *p_TickTimer = dwGetNowTick;
  }
  else
  {
    if( (dwGetNowTick - *p_TickTimer) >= dwWaitTickTime) 
    {
      *p_TickTimer = 0; 
      
      return RESET;
    }
  }
  
  return SET;
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
  * @retval None
  */
void HW_LowCode_Error(uint32_t dwErrData)
{
  __disable_irq();
  
  HW_All_LED_ONOFF(OFF);
  
  HW_Error_LED_ONnOFF(0, ON);
  
  while(dwErrData)
  {
    HW_Flu_A_LED(RESULT_LED_POS, ON);
    HW_Flu_B_LED(RESULT_LED_POS, ON);
    HW_COVID_LED(RESULT_LED_POS, ON);
    HAL_Delay(200);
    
    HW_Flu_A_LED(RESULT_LED_POS, OFF);
    HW_Flu_B_LED(RESULT_LED_POS, OFF);
    HW_COVID_LED(RESULT_LED_POS, OFF);
    HAL_Delay(200);
    
  }
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_Error_Handle(uint32_t dwONnOFF)
{
  HW_All_LED_ONOFF(OFF);
  HW_Error_LED_ONnOFF(0, ON);
  
  if(dwONnOFF)
  {
    HW_Flu_A_LED(RESULT_LED_POS, ON);
    HW_Flu_B_LED(RESULT_LED_POS, ON);
    HW_COVID_LED(RESULT_LED_POS, ON);
    
    HW_Flu_A_LED(RESULT_LED_NEG, ON);
    HW_Flu_B_LED(RESULT_LED_NEG, ON);
    HW_COVID_LED(RESULT_LED_NEG, ON);    

  }
  else
  {
    HW_Flu_A_LED(RESULT_LED_POS, OFF);
    HW_Flu_B_LED(RESULT_LED_POS, OFF);
    HW_COVID_LED(RESULT_LED_POS, OFF);
    
    HW_Flu_A_LED(RESULT_LED_NEG, OFF);
    HW_Flu_B_LED(RESULT_LED_NEG, OFF);
    HW_COVID_LED(RESULT_LED_NEG, OFF);        
  }
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_Error_Malloc_Fail(void)
{
  HW_All_LED_ONOFF(OFF);

  HW_Error_LED_ONnOFF(0, ON);

  while (1)
  {
      //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0); // 예시 GPIO
      //HAL_Delay(200);
    HW_Flu_A_LED(RESULT_LED_POS, ON);
    HAL_Delay(200);
    HW_Flu_A_LED(RESULT_LED_POS, OFF);
    HAL_Delay(200);
  }
  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void HW_Error_StackOverflow(void)
{
  HW_All_LED_ONOFF(OFF);

  HW_Error_LED_ONnOFF(0, ON);

  while (1)
  {
      //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0); // 예시 GPIO
      //HAL_Delay(200);
    HW_Flu_B_LED(RESULT_LED_POS, ON);
    HAL_Delay(200);
    HW_Flu_B_LED(RESULT_LED_POS, OFF);
    HAL_Delay(200);
  }  
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
