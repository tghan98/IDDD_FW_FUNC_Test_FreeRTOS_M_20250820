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
#ifndef __IDDD_RUNCMD_IF_H__
#define __IDDD_RUNCMD_IF_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
    
//------------------------------------------    

#define IDDD_RUN_CMD_DAT_SZ     4

#define IDDD_RUN_CMD_QUE_SZ     3
    
enum IDDD_RUN_CMD_CTRL
{
  ctrlIDDD_RUN_CMD_ATT = 0,
  ctrlIDDD_RUN_CMD_CMD,
  
  ctrlIDDD_RUN_CMD_MAX
};

enum IDDD_RUN_CMD_ATT
{
  attIDDD_RUN_CMD_WR = 1,
  attIDDD_RUN_CMD_RD,
  attIDDD_RUN_CMD_RUN,
  
  attIDDD_RUN_CMD_MAX
};


enum IDDD_RUN_CMD
{
  cmdIDDD_LED_PW = 10,
  cmdIDDD_OPT_SET,
  cmdIDDD_TRF_TIM,
  cmdIDDD_TRF_RUN = 20,
  cmdIDDD_INFO,
  cmdIDDD_SAVE,
  
  cmdIDDD_CMD_MAX
};


//-----------------------------------------
typedef struct
{
  uint16_t wCTRL[ctrlIDDD_RUN_CMD_MAX]; //0 ATT, 2 CMD
  uint32_t dwData[IDDD_RUN_CMD_DAT_SZ];
}IDDD_Run_Cmd_COM_t;

//------------------------------------------

//function

int32_t IDDD_RunCmd_Interface_Config(void);

int32_t comIDDD_RunCmd_Send(IDDD_Run_Cmd_COM_t *p_tSendDataMem, uint32_t dwWaitTime);
int32_t comIDDD_RunCmd_Get(IDDD_Run_Cmd_COM_t *p_tGetDataMem, uint32_t dwWaitTime);

#endif /* __IDDD_RUNCMD_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
