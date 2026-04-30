#ifndef FREERTOS_HOOKS_H
#define FREERTOS_HOOKS_H

#include "FreeRTOS.h"
#include "task.h"  // TaskHandle_t 정의 포함

#ifdef __cplusplus
extern "C" {
#endif

#if 0  //task.h에 선언 되어 있어 해당 해더에서 주석 처리 함.

#if configUSE_MALLOC_FAILED_HOOK
void vApplicationMallocFailedHook(void);
#endif  
  
#if configCHECK_FOR_STACK_OVERFLOW
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
#endif

#if configUSE_IDLE_HOOK
void vApplicationIdleHook(void);
#endif

#if configUSE_TICK_HOOK
void vApplicationTickHook(void);
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif // FREERTOS_HOOKS_H
