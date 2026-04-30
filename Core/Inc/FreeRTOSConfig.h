#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#include <stdint.h>
extern uint32_t SystemCoreClock;
#endif

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *----------------------------------------------------------*/

//Kernel 
#define configUSE_PREEMPTION                    1                       //선점형 스케줄링 사용
#define configUSE_TIME_SLICING                  1                       //동일 우선순위 태스크 간 시간 분할
#define configUSE_IDLE_HOOK                     0                       //Idle Hook 함수 사용 여부
#define configUSE_TICK_HOOK                     0                       //Tick Hook 함수 사용 여부
#define configCPU_CLOCK_HZ                      ( SystemCoreClock )     //CPU 클럭
#define configTICK_RATE_HZ                      ( 1000 )                //Tick 주기 (1초에 몇 번 tick?)
#define configMAX_PRIORITIES                    ( 7 )                   //태스크 우선순위 단계 수
#define configMINIMAL_STACK_SIZE                ( 64 )                 //최소 스택 크기 (word 단위)
#define configTOTAL_HEAP_SIZE                   ( 6 * 1024 )            /*동적 할당 heap 크기 예: 6KB heap */

//Sync & memory 
#define configUSE_MUTEXES                       1                       //뮤텍스 사용 여부
#define configUSE_RECURSIVE_MUTEXES             0                       //재귀적 뮤텍스 사용 여부

#define configUSE_COUNTING_SEMAPHORES           0                       //카운팅 세마포어 사용 여부

#define configQUEUE_REGISTRY_SIZE               8                       //큐, 세마포어, 뮤텍스 등의 커널 오브젝트에 이름 개수 ex)vQueueAddToRegistry(xMyQueue, "SensorQueue");

#define configUSE_MALLOC_FAILED_HOOK            1                       // malloc 실패 시 훅 함수 사용, 따라서 개발 및 초기 제품 안정성 확보 시에는 반드시 1로 사용

#define configCHECK_FOR_STACK_OVERFLOW          2                       //스택 오버플로우 감지 방식 2

#define configUSE_APPLICATION_TASK_TAG          0                       //태스크 태그 기능 사용

#define configSUPPORT_STATIC_ALLOCATION         1                       // 정적 할당 지원 여부
#define configSUPPORT_DYNAMIC_ALLOCATION        1                       //동적 할당 지원 여부

#define configAPPLICATION_ALLOCATED_HEAP        0                       //정적 heap 사용 시 0, User Heap 설정 시 1


//Other
#define configMAX_TASK_NAME_LEN                 ( 16 )  //태스크 이름의 최대 길이(문자 수), 15자의 이름 + 1개의 null-terminator (\0)로 구성
#define configUSE_16_BIT_TICKS                  0       //FreeRTOS의 tick counter를 16비트로 할지 32비트로 할지 결정, 0 → 32비트 사용 (기본 설정)
#define configIDLE_SHOULD_YIELD                 1       //Idle 태스크가 실행 중일 때, 같은 우선순위의 준비된 태스크가 있으면 CPU를 양보할지 여부

#define configGENERATE_RUN_TIME_STATS           0       //설명: 런타임에서 태스크별 CPU 사용 시간 통계를 수집할 수 있는 기능


/* Software timer definitions. */
#define configUSE_TIMERS                        0
#define configTIMER_TASK_PRIORITY               ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            ( configMINIMAL_STACK_SIZE * 2 )

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         ( 1 )

/* Set the following definitions to 1 to include the API function, or zero
   to exclude the API function. */
#define INCLUDE_vTaskPrioritySet                1                       //동적으로 우선순위 설정
#define INCLUDE_uxTaskPriorityGet               1                       //동적으로 우선순위 읽기
#define INCLUDE_vTaskDelete                     1                       //태스크 삭제 기능은 일반적으로 필요
#define INCLUDE_vTaskSuspend                    1                       //태스크 일시 중지 기능
#define INCLUDE_xResumeFromISR                  1                       //ISR에서 태스크 복귀에 필요
#define INCLUDE_vTaskDelayUntil                 1                       //주기 실행 필요 시만 사용
#define INCLUDE_vTaskDelay                      1                       //기본 딜레이 (ms 단위)
#define INCLUDE_xTaskGetSchedulerState          1                       //스케줄러가 현재 실행 중인지, 중단됐는지 확인하는 함수 사용
#define INCLUDE_xTaskGetCurrentTaskHandle       1                       //현재 실행 중인 태스크의 핸들을 가져오는 함수 사용 디버깅 
#define INCLUDE_xTaskGetIdleTaskHandle          0                       //Idle 태스크의 핸들을 얻는 함수 디버깅, 필요 없음
#define INCLUDE_eTaskGetState                   1                       //태스크 상태(enum: Running, Ready 등)를 조회하는 함수 사용
#define INCLUDE_xEventGroupSetBitFromISR        1                       //ISR에서 Event Group 비트를 설정하는 함수 사용
#define INCLUDE_xTimerPendFunctionCall          0                       //ISR에서 콜백을 예약하는 xTimerPendFunctionCall() 함수 사용

   
//기타
#define INCLUDE_pcTaskGetName                   0                       //태스크 이름 문자열 반환
#define INCLUDE_xTaskGetHandle                  0                       //태스크 이름을 기반으로 핸들을 가져오기
#define INCLUDE_vTaskList                       0                       //태스크 상태 문자열 출력 함수 vTaskList() 사용 가능
#define INCLUDE_vTaskGetInfo                    0                       //태스크의 상세 정보 구조체 반환
#define INCLUDE_xTaskAbortDelay                 1                       //vTaskDelay()를 강제 중단하는 API (xTaskAbortDelay())   

//#defein INCLUDE_vTaskCleanUpResources           0                       //기본값은 0이며 대부분의 임베디드 시스템에서는 사용하지 않음

   
/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
 #define configPRIO_BITS       __NVIC_PRIO_BITS
#else
 #define configPRIO_BITS       2        /* STM32U031 등 Cortex-M0+: 2비트 (0~3 우선순위) */
#endif

/* Interrupt priorities */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         3
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    2

#define configKERNEL_INTERRUPT_PRIORITY         ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Optional: enable trace facility */
#define configUSE_TRACE_FACILITY                0
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* Assertion for debugging */
#define configASSERT(x) if ((x) == 0) { taskDISABLE_INTERRUPTS(); for(;;); }
//#define configASSERT(x)    do { if ((x) == 0) { __disable_irq(); while(1); } } while(0)

//
#define configCOMMAND_INT_MAX_OUTPUT_SIZE       128


//
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
