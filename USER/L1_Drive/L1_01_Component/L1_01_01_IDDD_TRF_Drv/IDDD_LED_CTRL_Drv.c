
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
#include "IDDD_LED_CTRL_Drv.h"

//HW
//#include "PP_BD_v2_0_HW.h"

//Define 
#define VREF_mV         3300 //3.3V 기준, mV
#define IDDD_DAC_RES    4095 //12bit DAC 최대 값

//0.80586mV Ref 10 ohm 0.806/10 = 0.08



/* Extern --------------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

//OS Var


//Normal Var
uint32_t g_dwDAC_Convert_Count;



/* Private function prototypes -----------------------------------------------*/



//Macro Function



/*Function -------------------------------------------------------------------*/

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
void IDDD_LED_Channel_Select(uint32_t dwCH)
{
  if(dwCH >= OPT_LED_CH_OFF) dwCH = OPT_LED_CH_OFF;
  
  switch(dwCH)
  {
  case OPT_LED_CH1:
    
    OPT_LED_SEL1_GPIO_Port->BRR = (uint32_t)OPT_LED_SEL1_Pin;
    OPT_LED_SEL2_GPIO_Port->BRR = (uint32_t)OPT_LED_SEL2_Pin;
    OPT_LED_SEL3_GPIO_Port->BRR = (uint32_t)OPT_LED_SEL3_Pin;
    
    break;
  case OPT_LED_CH2:

    OPT_LED_SEL1_GPIO_Port->BSRR = (uint32_t)OPT_LED_SEL1_Pin;
    OPT_LED_SEL2_GPIO_Port->BRR = (uint32_t)OPT_LED_SEL2_Pin;
    OPT_LED_SEL3_GPIO_Port->BRR = (uint32_t)OPT_LED_SEL3_Pin;

    break;
  case OPT_LED_CH3:

    OPT_LED_SEL1_GPIO_Port->BRR = (uint32_t)OPT_LED_SEL1_Pin;
    OPT_LED_SEL2_GPIO_Port->BSRR = (uint32_t)OPT_LED_SEL2_Pin;
    OPT_LED_SEL3_GPIO_Port->BRR = (uint32_t)OPT_LED_SEL3_Pin;

    break;
  case OPT_LED_CH4:

    OPT_LED_SEL1_GPIO_Port->BSRR = (uint32_t)OPT_LED_SEL1_Pin;
    OPT_LED_SEL2_GPIO_Port->BSRR = (uint32_t)OPT_LED_SEL2_Pin;
    OPT_LED_SEL3_GPIO_Port->BRR = (uint32_t)OPT_LED_SEL3_Pin;

    break;
  case OPT_LED_CH5:

    OPT_LED_SEL1_GPIO_Port->BRR = (uint32_t)OPT_LED_SEL1_Pin;
    OPT_LED_SEL2_GPIO_Port->BRR = (uint32_t)OPT_LED_SEL2_Pin;
    OPT_LED_SEL3_GPIO_Port->BSRR = (uint32_t)OPT_LED_SEL3_Pin;

    break;
  case OPT_LED_CH6:

    OPT_LED_SEL1_GPIO_Port->BSRR = (uint32_t)OPT_LED_SEL1_Pin;
    OPT_LED_SEL2_GPIO_Port->BRR = (uint32_t)OPT_LED_SEL2_Pin;
    OPT_LED_SEL3_GPIO_Port->BSRR = (uint32_t)OPT_LED_SEL3_Pin;

    break;
  case OPT_LED_CH7:

    OPT_LED_SEL1_GPIO_Port->BRR = (uint32_t)OPT_LED_SEL1_Pin;
    OPT_LED_SEL2_GPIO_Port->BSRR = (uint32_t)OPT_LED_SEL2_Pin;
    OPT_LED_SEL3_GPIO_Port->BSRR = (uint32_t)OPT_LED_SEL3_Pin;
    
    break;
  default:

    OPT_LED_SEL1_GPIO_Port->BSRR = (uint32_t)OPT_LED_SEL1_Pin;
    OPT_LED_SEL2_GPIO_Port->BSRR = (uint32_t)OPT_LED_SEL2_Pin;
    OPT_LED_SEL3_GPIO_Port->BSRR = (uint32_t)OPT_LED_SEL3_Pin;

    break;
  }
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_LED_Current_Set(uint32_t dwCurrentVal_x100uA)
{
  int32_t dwCheck = 0;

  if(dwCurrentVal_x100uA > VREF_mV) return DAT_ERR_PARAM_DATA; 
  
  
  g_dwDAC_Convert_Count = (dwCurrentVal_x100uA * IDDD_DAC_RES)/VREF_mV;
  
  
  dwCheck = HW_DAC_CTRL(g_dwDAC_Convert_Count);
  
  return dwCheck;
}

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
uint32_t Read_IDDD_LED_DAC_CountResult(void)
{
  return g_dwDAC_Convert_Count;
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
