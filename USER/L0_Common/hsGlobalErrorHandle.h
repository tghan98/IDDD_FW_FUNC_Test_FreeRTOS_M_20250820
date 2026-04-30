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
#ifndef __HS_GBL_ERR_HNDL_H__
#define __HS_GBL_ERR_HNDL_H__

/* Includes ------------------------------------------------------------------*/

    
//Enum
enum GBL_ERR_RETURN_CODE
{
  RETURN_OK = 0,
  HAL_ERR_RESERVE_MAX = 4,
  
  OS_SYS_ERR = 10, 
  OS_ACCES_FAIL,
  OS_SIG_TIMOUT,
  OS_QUE_PUT_FAIL,
  OS_QUE_GET_FAIL,
  OS_ERR_RESERVE_MAX = 19,
  
  DEV_CMPLT_TIMOUT = 20,        //Device Complete Timeout
  DEV_SP_ERR,                   //Device Special Error
  DEV_RC_TIMOUT,                //Devie Receive Timeout
  DEV_TC_TIMOUT,                //Device Transmit Time oit
  DEV_HNDL_FAIL,
  DEV_ERR_RESERVE_MAX = 29,
  
  DAT_ERR_PARAM_DATA = 30,
  DAT_ERR_PARAM_CMD,
  DAT_ERR_MAX = 39,
  
  RETURN_ERR_COD_MAX
};

enum GBL_ERR_LAYER_CODE
{
  GE_LAYER_CODE_0 = 0,
  GE_LAYER_CODE_1,
  GE_LAYER_CODE_2,
  GE_LAYER_CODE_3,
  
  GE_LAYER_CODE_MAX
};


//Type

//PUBLIC
void hsGlobalErrorUseConfig(uint32_t dwFp_LEDHandle);

int32_t hsGlobalErrorResetHandle(void);
int32_t hsGlobalErrorWriteAllHandle(uint32_t dwErrorStatus);
int32_t hsGlobalErrorWriteHandle(uint8_t uLayerCode, uint8_t uUtilCode1, uint8_t uUtilCode2, uint8_t uErrorCode);

int32_t hsGlobalErrorReadAllHandle(uint32_t *p_dwReadMem);
int32_t hsGlobalErrorReadHandle(uint8_t *p_uLayerCode, uint8_t *p_uUtilCode1, uint8_t *p_uUtilCode2, uint8_t *p_uErrorCode);

int32_t hsGlobalErrorBlocking_1st(uint8_t uLayerCode, uint8_t uUtilCode1, uint8_t uUtilCode2, uint8_t uErrorCode);
int32_t hsGlobalErrorBlocking_2nd(uint32_t dwErrorStatus);

#endif /* __HS_GBL_ERR_HNDL_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
