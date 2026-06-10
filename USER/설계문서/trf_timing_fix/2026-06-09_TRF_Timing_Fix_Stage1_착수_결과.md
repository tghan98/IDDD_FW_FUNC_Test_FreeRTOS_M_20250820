# 2026-06-09_TRF_Timing_Fix_Stage1_착수_결과

## 1. 목적
Stage1 착수 작업에서 trfrun 명령으로 100샘플 총시간 측정을 수행할 수 있도록 최소 기능을 구현한 결과를 정리했다. 본 문서는 Stage0 안정 경로를 유지한 상태에서 Stage1 측정 경로를 임시 매핑으로 추가한 내용과 빌드 검증 결과를 기록하고, 다음 검증 단계를 명확히 남기는 것을 목적으로 작성했다.

## 2. 수행내용
### 2.1 ADC 드라이버에 Stage1 측정 상태를 추가했다
콜백과 폴링 루프 간 동기화 경계를 명확히 하기 위해 [IDDD_ADC_SCAN_Drv.c](USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_ADC_SCAN_Drv.c)에 측정 모드 플래그, 완료 플래그, 종료 CNT 캡처 값을 분리한 volatile 전역 상태를 추가했다.

### 2.2 ADC 완료 콜백을 Stage1 분기로 확장했다
Stage0 외부 트리거 경로를 유지한 채 Stage1 전용 완료 신호를 분리하기 위해, 같은 파일의 `HAL_ADC_ConvCpltCallback`에 Stage1 측정 모드일 때 TIM3 CNT를 종료 시각으로 캡처하고 완료 플래그를 SET하는 분기를 추가했다.

### 2.3 Stage1 전용 ADC 런타임 설정 함수를 추가했다
기존 `IDDD_PD_ADC_Config`를 수정하지 않고 Stage1 측정 프로파일로 전환할 수 있도록, 같은 파일에 SW Start / ContinuousConvMode ENABLE / DMAContinuousRequests ENABLE을 적용하는 `IDDD_PD_ADC_Config_Stage1Meas` 함수를 추가했다.

### 2.4 TIM3 기반 1us 측정 헬퍼를 추가했다
DMA 버스트 총시간을 마이크로초 단위로 직접 계산하기 위해 [IDDD_TRF_Timer_CTRL.c](USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_TRF_Timer_CTRL.c)에 TIM3를 1MHz tick으로 구성하는 자유실행 카운터 초기화/정지 함수와 overflow-safe 시간차 계산 함수를 추가했다.

### 2.5 Stage1 측정 실행 루틴을 추가했다
trfrun 명령 하나로 반복 측정과 통계를 자동 수집하기 위해 [IDDD_RunCmd_TRF_Interface.c](USER/L2_Interface/L2_01_IDDD_RunCMD_Interface/IDDD_RunCmd_TRF_Interface.c)에 `IDDD_RunCmd_TRF_Meas` 함수를 추가했다. 각 반복은 flag reset → start CNT 기록 → TRF_MEAS_MARK High → DMA 시작 → timeout 포함 폴링 → 플래그 인식 직후 TRF_MEAS_MARK Low → 총시간 계산 순서로 처리하며, 100회 완료 후 avg/min/max/count를 UART로 출력한다.

### 2.6 RunCommand 디스패치를 Stage1으로 임시 매핑했다
Stage1 측정 루틴을 기존 trfrun 명령으로 즉시 실기 검증하기 위해 [IDDD_RunCommand_App.c](USER/L3_Application/L3_01_IDDD_RunCommand_app/IDDD_RunCommand_App.c)의 `cmdIDDD_TRF_RUN` 분기에서 `CmdRun_TRF_Run` 대신 `IDDD_RunCmd_TRF_Meas`를 호출하도록 임시 전환했다. Stage0 함수 본문은 그대로 보존해 검증 완료 후 즉시 복원할 수 있도록 유지했다.

### 2.7 헤더 선언을 동기화하고 빌드를 검증했다
착수 단계 구현이 정상 컴파일됨을 확인하기 위해 [IDDD_ADC_SCAN_Drv.h](USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_ADC_SCAN_Drv.h), [IDDD_TRF_Timer_CTRL.h](USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_TRF_Timer_CTRL.h), [IDDD_RunCmd_TRF.h](USER/L2_Interface/L2_01_IDDD_RunCMD_Interface/IDDD_RunCmd_TRF.h)에 신규 함수 선언을 추가하고 IAR Rebuild 기준 에러 0, 경고 0을 확인했다.

## 3. 다음 단계
- trfrun 실기 검증에서 TRF_MEAS_MARK 단일 펄스와 UART 통계 로그(avg/min/max/count)를 동시 확인한다.
- timeout 발생률, 측정 분산, 반복 안정성을 확인해 Stage1 측정 루틴의 신뢰성을 점검한다.
- Stage1 검증 완료 후 임시 매핑과 런타임 전환 코드를 정리하고, 합의된 최종 ADC 설정을 CubeMX에 반영한다.
