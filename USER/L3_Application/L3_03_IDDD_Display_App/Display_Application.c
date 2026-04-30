
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
#include "Display_Application.h"

//HW

//Interface & Handle
#include "Display_Interface.h"


//-----------------------------------------------------------------------------
//define 

//OS Config
#define DISP_APP_MEM        128
#define DISP_APP_PRIO       (tskIDLE_PRIORITY + 1)     // 6 5 4 3 2 |1| 0


//Other

//Typedef
/* Extern --------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//OS Val
TaskHandle_t g_xDisp_App_TaskHandle;
void *g_vDISP_Param;

//Other
uint32_t g_dwPreSqDispMode, g_dwCurrentSqDispMode, g_dwBATDispMode, g_dwBLEDispMode;
uint32_t g_dwBlankBlink_OnOff_Cnt, g_dwBlanMode_TimeCnt;

//Mecro 
#define DISP_ERROR                      hsGlobalErrorBlocking_2nd

/* Private function prototypes -----------------------------------------------*/
void IDDD_Display_App(void *p_vParam);


/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t IDDD_Display_App_Config(void)
{
  int32_t dwCheck = 0;
  
  xTaskCreate(IDDD_Display_App, "DISP", DISP_APP_MEM, (void *)g_vDISP_Param,
              DISP_APP_PRIO, &g_xDisp_App_TaskHandle);
  
  return dwCheck;
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
void IDDD_Display_App(void *p_vParam)
{
  int32_t dwCheck = 0;
  
  vTaskDelay(1500);
  
  IDDD_Disp_Status_Write(staDISP_COD_IN);
  IDDD_Disp_BAT_Status_Write(staBAT_3);
  IDDD_Disp_BLE_Status_Write(staBLE_CONEC);
 
  
  for(;;)
  {
    
    HW_All_LED_ONOFF(OFF);
    
    //Sq Mode
    dwCheck = IDDD_Disp_Status_Read(&g_dwCurrentSqDispMode);
    if(dwCheck) DISP_ERROR(dwCheck);
    
    if(g_dwPreSqDispMode != g_dwCurrentSqDispMode)
    {
      g_dwPreSqDispMode = g_dwCurrentSqDispMode;
      
      g_dwBlankBlink_OnOff_Cnt = 0;
      
      g_dwBlanMode_TimeCnt = 0;
    }
    
    switch(g_dwCurrentSqDispMode)
    {
    case staDISP_COD_IN:
      
      HW_PW_LED_ONnOFF(0,ON);
      
      if(g_dwBlankBlink_OnOff_Cnt < 5) HW_CC_CK_LED_ONnOFF(0, OFF);
      else HW_CC_CK_LED_ONnOFF(0, ON);      
      
      break;
    case staDISP_CST_IN:
      
      HW_PW_LED_ONnOFF(0,ON);
      HW_CC_CK_LED_ONnOFF(0, ON);
      
      if(g_dwBlankBlink_OnOff_Cnt < 5) HW_Cassette_CK_LED_ONnOFF(0, OFF);
      else HW_Cassette_CK_LED_ONnOFF(0, ON);
      
      break;
    case staDISP_SMPL_IN:

      HW_PW_LED_ONnOFF(0,ON);
      HW_CC_CK_LED_ONnOFF(0, ON);
      HW_Cassette_CK_LED_ONnOFF(0, ON);
      
      if(g_dwBlankBlink_OnOff_Cnt < 5) HW_Sample_LED_ONnOFF(0, OFF);
      else HW_Sample_LED_ONnOFF(0, ON);

      break;
    case staDISP_INCU_RN:

      HW_PW_LED_ONnOFF(0,ON);
      HW_CC_CK_LED_ONnOFF(0, ON);
      HW_Cassette_CK_LED_ONnOFF(0, ON);
      HW_Sample_LED_ONnOFF(0, ON);
      
      if(g_dwBlankBlink_OnOff_Cnt < 5) HW_Incubator_LED_ONnOFF(0, OFF);
      else HW_Incubator_LED_ONnOFF(0, ON);

      break;
    case staDISP_RES_RN:

      HW_PW_LED_ONnOFF(0,ON);
      HW_CC_CK_LED_ONnOFF(0, ON);
      HW_Cassette_CK_LED_ONnOFF(0, ON);
      HW_Sample_LED_ONnOFF(0, ON);
      HW_Incubator_LED_ONnOFF(0, ON);
      
      HW_Flu_A_LED(RESULT_LED_POS, ON);
      HW_Flu_B_LED(RESULT_LED_NEG, ON);
      HW_COVID_LED(RESULT_LED_POS, ON);

      break;      
    case staDISP_ERROR_RN:
      
      HW_PW_LED_ONnOFF(0,ON);
      HW_CC_CK_LED_ONnOFF(0, ON);
      HW_Cassette_CK_LED_ONnOFF(0, ON);
      HW_Sample_LED_ONnOFF(0, ON);
      HW_Incubator_LED_ONnOFF(0, ON);

      if(g_dwBlankBlink_OnOff_Cnt < 5) HW_Error_LED_ONnOFF(0, OFF);
      else HW_Error_LED_ONnOFF(0, ON);
      
      break;
    default:
      
      HW_PW_LED_ONnOFF(0,ON);
      
      break;
    }
    
    //BAT  
    dwCheck = IDDD_Disp_BAT_Status_Read(&g_dwBATDispMode);
    if(dwCheck) DISP_ERROR(dwCheck);
    
    switch(g_dwBATDispMode)
    {
    case staBAT_BLNK_1:
      
      if(g_dwBlankBlink_OnOff_Cnt < 5) HW_BAT_LED_ONnOFF(BAT_LED_CH1, OFF);
      else HW_BAT_LED_ONnOFF(BAT_LED_CH1, ON);
      
      break;
    case staBAT_1:
      
      HW_BAT_LED_ONnOFF(BAT_LED_CH1, ON);
      
      break;
    case staBAT_2:
      
      HW_BAT_LED_ONnOFF(BAT_LED_CH1, ON);
      HW_BAT_LED_ONnOFF(BAT_LED_CH2, ON);
      
      break;
    default:
      
      HW_BAT_LED_ONnOFF(BAT_LED_CH1, ON);
      HW_BAT_LED_ONnOFF(BAT_LED_CH2, ON);
      HW_BAT_LED_ONnOFF(BAT_LED_CH3, ON);
      
      break;
    }
    
    
    //BLE
    dwCheck = IDDD_Disp_BLE_Status_Read(&g_dwBLEDispMode);
    if(dwCheck) DISP_ERROR(dwCheck);
    
    switch(g_dwBLEDispMode)
    {
    case staBLE_CONEC:
      
      HW_BLE_LED_ONnOFF(0, ON);
      
      break;
    default:
      if(g_dwBlankBlink_OnOff_Cnt < 5) HW_BLE_LED_ONnOFF(0, OFF);
      else HW_BLE_LED_ONnOFF(0, ON);
        
      break;
    }
    
    //Timer
    g_dwBlanMode_TimeCnt++;
    if(g_dwBlanMode_TimeCnt > 9) g_dwBlanMode_TimeCnt = 0;
    
    g_dwBlankBlink_OnOff_Cnt++;
    if(g_dwBlankBlink_OnOff_Cnt > 9) g_dwBlankBlink_OnOff_Cnt = 0;
    
    vTaskDelay(100);
  }
}


//-----------------------------------------------------------------------------
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None pdTRUE 
  */


//-----------------------------------------------------------------------------

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
