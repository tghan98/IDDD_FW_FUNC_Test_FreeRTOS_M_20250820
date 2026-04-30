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
#ifndef __REWORK_PP_DRV_H__
#define __REWORK_PP_DRV_H__

/* Includes ------------------------------------------------------------------*/

#define RWRK_PP_SYS_ERR                 255

//Type    
typedef struct Rework_PP_Drv_Type_
{
  void *p_PP_Drv;
  SemaphoreHandle_t xMutex;
}Rework_PP_Drv_t;

//PUBLIC
int32_t Rework_PP_Drv_Config(void *p_PP_Drv, uint32_t dwPP_Drv_ID);

Rework_PP_Drv_t *Read_Rework_PP_Drv(uint32_t dwRework_PP_ID);

int32_t Lock_Rework_PP_Drv(Rework_PP_Drv_t *p_tDrv, uint32_t xWaitTime);
void Unlock_Rework_PP_Drv(Rework_PP_Drv_t *p_tDrv);

#endif /* __REWORK_PP_DRV_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
