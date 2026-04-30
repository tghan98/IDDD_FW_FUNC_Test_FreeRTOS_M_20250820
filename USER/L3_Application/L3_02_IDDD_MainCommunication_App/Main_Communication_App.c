
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
#include "Main_Communication_App.h"

//HW
#include "BoTnLE525BLE_Drv.h"

//Interface & Handle


//-----------------------------------------------------------------------------
//define 

//OS Config
#define MAIN_COM_APP_MEM        128
#define MAIN_COM_APP_PRIO       (tskIDLE_PRIORITY + 1)     // 6 5 4 3 2 |1| 0
//Other

//Typedef
/* Extern --------------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define MAIN_COM_APP_ERR        hsGlobalErrorBlocking_2nd

/* Private variables ---------------------------------------------------------*/

//OS Val
void *gp_vMainCOM_Param;
TaskHandle_t g_xMain_Com_App_TaskHandle;

//Normal val
extern BoTnLE523_Drv_t g_tBLE_Drv;

extern uint8_t g_uBLE_Read_Rx_Buff[BOTNLE523_RX_BUFF_SZ];

/* Private function prototypes -----------------------------------------------*/
void Main_Communication_App(void * p_vParam);



/* Interrupt  ----------------------------------------------------------------*/

/*Function -------------------------------------------------------------------*/

/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None
  */
int32_t Main_Com_App_Config(void)
{
  int32_t dwCheck = 0;
  
  
  
  xTaskCreate(Main_Communication_App, "MAIN_COM", MAIN_COM_APP_MEM, (void *)gp_vMainCOM_Param,
              MAIN_COM_APP_PRIO, &g_xMain_Com_App_TaskHandle);
  
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
//-----------------------------------------------------------------------------
/**
  * @brief  This 
  * @param  file: 
  * @param  line: 
  * @retval None pdTRUE 
  */
void Main_Communication_App(void * p_vParam)
{
  int32_t dwCheck = 0, dwCnt;
  
  vTaskDelay(500);
  
  dwCheck = BotnLE_BLE_Config(&g_tBLE_Drv, RWRK_USART1_PP, PORT_NUM_USART1, 19200);
  if(dwCheck) MAIN_COM_APP_ERR(dwCheck);
  
  HW_BLE_Power_ONnOFF(ON);
  hsDebug_MSG("*** [MAIN_COM] Application Start ***\n");
  
  for(;;)
  {
    //vTaskDelay(500);
    dwCheck = BoTnLE_AT_Command_Response(&g_tBLE_Drv, g_uBLE_Read_Rx_Buff, 500);
    if(dwCheck == 0)
    {
      hsDebug_MSG("%s\n", g_uBLE_Read_Rx_Buff);
      for(dwCnt = 0; dwCnt< BOTNLE523_RX_BUFF_SZ;dwCnt++) g_uBLE_Read_Rx_Buff[dwCnt] = 0;
    }
  }
}

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
