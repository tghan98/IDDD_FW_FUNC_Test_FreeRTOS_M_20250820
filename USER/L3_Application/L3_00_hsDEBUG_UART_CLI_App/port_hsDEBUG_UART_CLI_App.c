/**
  ******************************************************************************
  * @file     
  * @author  
  * @version   
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "port_hsDEBUG_UART_CLI_App.h"

//HW Config

//Public 
//Extern variable

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define hsDEBUG_USART_RX_DMA_USE     0
#define hsDEBUG_USART_TX_DMA_USE     1

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t port_hsDEBUG_UART_CLI_Config(void *p_PP_DRV, uint32_t dwEN_nDIS)
{
  UART_HandleTypeDef *p_HAL_USART;
  int32_t dwCheck = 0;
  
  p_HAL_USART = (UART_HandleTypeDef *)p_PP_DRV;

  p_HAL_USART->Instance = USART1;
  p_HAL_USART->Init.BaudRate = 19200;
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
  * @brief  
  * @param  None
  * @retval None
  */
void port_hsDEBUS_UART_CLI_IRQ_Linker(uint32_t dwFP_Rx, uint32_t dwFP_Tx)
{
  HW_USART_RX_Linker(PORT_NUM_USART1, dwFP_Rx);
  HW_USART_TX_Linker(PORT_NUM_USART1, dwFP_Tx);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t port_hsDEBUG_UART_RX_Run(void *p_PP_DRV, uint8_t *p_uGetDataMem, uint32_t dwGetDataSize)
{
  int32_t dwCheck = 0;
  
#if  hsDEBUG_USART_RX_DMA_USE
  dwCheck = HAL_UART_Receive_DMA(p_PP_DRV, p_uGetDataMem, dwGetDataSize);
#else
  dwCheck = HAL_UART_Receive_IT(p_PP_DRV, p_uGetDataMem, dwGetDataSize);
#endif  
  return dwCheck;
}
/**
  * @brief  
  * @param  None
  * @retval None
  */
int32_t port_hsDEBUS_UART_TX_Run(void *p_PP_DRV, uint8_t *p_uPutDataMem, uint32_t dwPutDataSize)
{
  int32_t dwCheck = 0;
  
#if hsDEBUG_USART_TX_DMA_USE
  dwCheck = HAL_UART_Transmit_DMA(p_PP_DRV, p_uPutDataMem, dwPutDataSize);
#else
  dwCheck = HAL_UART_Transmit_IT(p_PP_DRV, p_uPutDataMem, dwPutDataSize);
#endif
  
  return dwCheck;
}

/**
  * @brief  
  * @param  None
  * @retval None
  */


/**
  * @brief  
  * @param  None
  * @retval None
  */

