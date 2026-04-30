#include "freertos_hooks.h"
#include "main.h"

/**
 * @brief Malloc 실패 시 호출되는 훅 함수
 */
void vApplicationMallocFailedHook(void)
{
    // 예: 빨간 LED 점멸
  __disable_irq();
  
  HW_Error_Malloc_Fail();
}

/**
 * @brief 스택 오버플로우 발생 시 호출되는 훅 함수
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
  (void)xTask;
  (void)pcTaskName;

    // 예: 스택 오버플로우 감지 → 무한 루프
    __disable_irq();
    
    HW_Error_StackOverflow();
}

/**
 * @brief Idle 태스크에서 주기적으로 호출됨
 */
void vApplicationIdleHook(void)
{
    // 전력 절감용 처리 등
    //__WFI(); // Wait For Interrupt (저전력 대기)
}

/**
 * @brief 매 시스템 Tick 마다 호출됨
vApplicationGetIdleTaskMemory
 */
void vApplicationTickHook(void)
{
    // 필요 시 구현, 보통 비워둠
}
