
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
#include "BoTnLE525BLE_Port.h"

//HW

//Interface & Handle

//-----------------------------------------------------------------------------
//define 

//OS Config

//Other
#define hsBOTNLE525BLE_UART_RX_DMA_USE  0
#define hsBOTNLE525BLE_UART_TX_DMA_USE  1


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
int32_t port_hsBoTnL525BLE_UART_Config(void *p_PP_DRV, uint32_t dwBuadRate, uint32_t dwEN_nDIS)
{
  UART_HandleTypeDef *p_HAL_USART;
  int32_t dwCheck = 0;
  
  p_HAL_USART = (UART_HandleTypeDef *)p_PP_DRV;

  p_HAL_USART->Instance = USART1;
  p_HAL_USART->Init.BaudRate = dwBuadRate;
  p_HAL_USART->Init.WordLength = UART_WORDLENGTH_8B;
  p_HAL_USART->Init.StopBits = UART_STOPBITS_1;
  p_HAL_USART->Init.Parity = UART_PARITY_NONE;
  p_HAL_USART->Init.Mode = UART_MODE_TX_RX;
  p_HAL_USART->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  p_HAL_USART->Init.OverSampling = UART_OVERSAMPLING_16;
  p_HAL_USART->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  p_HAL_USART->Init.ClockPrescaler = UART_PRESCALER_DIV1;
  p_HAL_USART->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  
  if(dwEN_nDIS) //Enable 
  {
    dwCheck = HAL_UART_Init(p_HAL_USART);
  }
  else         //Disable
  {
    dwCheck = HAL_UART_DeInit(p_HAL_USART);
  } 
  return dwCheck;  
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
void port_hsBoTnL525BLE_UART_IRQ_Linker(uint32_t dwPortNum, uint32_t dwFP_Rx, uint32_t dwFP_Tx)
{
  HW_USART_RX_Linker(dwPortNum, dwFP_Rx);
  HW_USART_TX_Linker(dwPortNum, dwFP_Tx);  
}


/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t port_hsBoTnL525BLE_UART_RX_Run(void *p_PP_DRV, uint8_t *p_uGetDataMem, uint32_t dwGetDataSize)
{
  int32_t dwCheck = 0;
  
#if  hsBOTNLE525BLE_UART_RX_DMA_USE
  dwCheck = HAL_UART_Receive_DMA(p_PP_DRV, p_uGetDataMem, dwGetDataSize);
#else
  dwCheck = HAL_UART_Receive_IT(p_PP_DRV, p_uGetDataMem, dwGetDataSize);
#endif  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t port_hsBoTnL525BLE_UART_RX_Stop(void *p_PP_DRV)
{
  int32_t dwCheck = 0;
  
#if  hsBOTNLE525BLE_UART_RX_DMA_USE
  dwCheck = HAL_UART_DMAStop(p_PP_DRV);
#else
  dwCheck = HAL_UART_AbortReceive_IT(p_PP_DRV);
#endif  
  return dwCheck;
}



/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t port_hsBoTnL525BLE_UART_TX_Run(void *p_PP_DRV, uint8_t *p_uPutDataMem, uint32_t dwPutDataSize)
{
  int32_t dwCheck = 0;
  
#if hsBOTNLE525BLE_UART_TX_DMA_USE
  dwCheck = HAL_UART_Transmit_DMA(p_PP_DRV, p_uPutDataMem, dwPutDataSize);
#else
  dwCheck = HAL_UART_Transmit_IT(p_PP_DRV, p_uPutDataMem, dwPutDataSize);
#endif
  
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
