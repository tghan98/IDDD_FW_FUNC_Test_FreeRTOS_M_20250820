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
#ifndef __HS_DEBUG_MSG_H__
#define __HS_DEBUG_MSG_H__

/* Includes ------------------------------------------------------------------*/
    
//------------------------------------------    

//------------------------------------------

#if 0 //------------------------------------------------------------------------
#define hsRS485_COM_SLV_DEBUG_MS        1
#define hsMTN_IF_DEBUG_MSG              1
#define hsCH_DEBUG_MSG                  1
#define hsMAINCOM_DEBUG_MSG             1
#define hsMAINCOM_IF_DEBUG_MSG          1
#define hsADP_IF_DEBUG_MSG              1
#define hsSH_DEBUG_MSG                  1
#define hsMAIN_HNDL_DEBUG_MSG           1   
#define hsREG_IF_DEBUG_MSG              1
#define hsDEV_MNT_DEBUG_MSG             0
#define hsDISPLAY_DEBUG_MSG             1    
#define hsMCP23018_DRV_DEBUG_MSG        1
#define hsZEUS_ADP_DRV_DEBUG_MSG        1
#define hsTEMPER_CTRL_DEBUF_MSG         1
#define hsTEMP_RD_DEBUF_MSG             0
#define hsTCN75A_DEBUG_MSG              1

#endif //-----------------------------------------------------------------------

#define MOPT_CTRL_APP_DEBUG_MS          1


//function
void hsDebug_MSG_Config(void *fpOutStream);
void hsDebug_MSG(char *fmt, ...);

#endif /* __HS_DEBUG_MSG_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
