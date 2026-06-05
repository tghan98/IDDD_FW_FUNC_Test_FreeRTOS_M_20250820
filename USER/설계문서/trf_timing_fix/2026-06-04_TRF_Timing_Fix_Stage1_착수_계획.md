# 2026-06-04_TRF_Timing_Fix_Stage1_착수_계획

## 1. 목적
Stage1은 Stage0에서 확보한 Timer 카운팅 + 소프트웨어 GPIO 기반 5구간 시퀀스를 유지한 상태에서, adc_window 구간 내 ADC 변환 가능 횟수를 실측하고 통계(avg/min/max) 기반으로 확정하는 기능 확장 단계다. 본 단계의 목표는 GUI 입력 경로는 유지한 채(기존 TRF Run -> trfrun), Stage1 기간 동안 펌웨어 내부 디스패치에서 측정 루틴을 임시 매핑해 변환시간을 확정하고, 확정 후 trfrun의 adc_window를 반복트리거 방식으로 통합하는 것이다.

## 2. Stage0 선행조건
Stage1은 아래 Stage0 완료 조건이 충족되었다는 전제에서 착수한다.
- PWM 출력 의존 제거 완료
- Timer 카운팅 + 소프트웨어 GPIO 5구간 시퀀스 재현 완료
- 오실로스코프에서 PA6, PA7, PB11 구간 순서 확인 완료
- trfrun 10회 연속 실행 시 timeout, error 없음 확인 완료
- trftim, trfrun 기존 명령 회귀 없음 확인 완료

Stage1 작업 중에도 위 경로는 회귀 없이 유지되어야 한다.

## 3. 문서 사용 가이드(타 AI 에이전트 공용)
이 문서는 단독으로 읽어도 Stage1의 작업 목적, Stage0와의 경계, 구현 범위, 검증 기준을 이해할 수 있도록 작성한다.

핵심 의도는 다음과 같다.
- Stage0의 고정값 시퀀스 경로는 안정 경로로 유지한다.
- Stage1은 adc_window 내 ADC 측정 성능을 수치로 확정하는 단계다.
- 측정 결과는 avg/min/max와 pass/fail 기준으로 정리한다.
- Stage1 구현 중 Stage0 경로 재설계는 포함하지 않는다.

## 4. Stage1 범위
### 4.1 포함 범위
- 펌웨어 내부 임시 매핑을 통한 측정 검증 경로 구성(입력은 기존 trfrun 유지)
- ADC 1회 변환시간 실측
- 100회 반복 측정 기반 avg/min/max 산출
- max 기준 pass/fail 판정
- 확정된 max 기준으로 trfrun adc_window 반복트리거 정책 통합
- 5개 시간 파라미터 런타임 입력 경로 설계
- 제약식 검증 로직 확장
- ROM 저장 확장 검토

### 4.2 제외 범위
- Stage0 고정 시퀀스 경로 재설계
- GUI 화면/입력 UI 구현
- GUI TRF Run 전송 문자열 변경(trfrun -> trfmeas)
- BLE/App 연동 확장
- Stage0 검증 산출물 재작성

## 5. 파라미터 및 판정 정책
### 5.1 시간 파라미터
- led_on_us
- residual_wait_us
- adc_window_us
- stabilize_us
- cycle_us

### 5.2 기본 운용 기준
- Stage0 기본값은 초기 기준값으로 유지한다.
- Stage1에서는 위 5개 값을 런타임 입력 가능 대상으로 확장한다.
- 제약식은 아래와 같이 유지한다.
  - led_on_us + residual_wait_us + adc_window_us + stabilize_us <= cycle_us
- 각 파라미터는 0 이상이어야 한다.
- adc_window_us는 최소 1샘플 주기 이상이어야 한다.

### 5.3 측정 판정 기준
- 측정 횟수: 100회
- 측정 기준: DMA 완료(DMA TC)
- 출력 항목: avg, min, max, count, pass
- pass/fail 기준은 max 기준으로 판정한다.
- Stage1의 1차 목적은 adc_window_us 안에 ADC를 몇 회 넣을 수 있는지 보수적으로 산정하는 것이다.

## 6. 구현 핵심
### 6.1 측정 기준 시점 확정
ADC 변환 완료 기준은 DMA TC 시점으로 고정한다. 측정값은 TIM3 카운터를 사용해 us 단위로 환산한다.

### 6.2 공용 시간차 계산 적용
오버플로우 안전한 시간차 계산 헬퍼를 공용화해 측정 시작 시점과 DMA 완료 시점의 차이를 동일 방식으로 계산한다.

### 6.3 측정 루틴 구조
PA7 트리거 펄스를 정해진 주기로 100회 발생시키고, 각 측정에서 DMA 완료 시점의 변환시간을 수집한다. 수집한 결과는 avg/min/max로 산출한다.

### 6.4 샘플 수 결정 방식
adc_window_us 안에 수용 가능한 샘플 수는 max 기준 변환시간으로 계산한다. 즉, 평균값이 아니라 최악값을 기준으로 보수적으로 결정한다.

### 6.5 Stage1 임시 매핑 원칙
Stage1 검증 기간에는 GUI 입력 경로를 변경하지 않고, 펌웨어 내부에서 cmdIDDD_TRF_RUN 디스패치가 측정 루틴을 타도록 임시 매핑한다. Stage1 확정 후에는 디스패치를 정상 trfrun 경로로 원복하고, trfrun 내부 adc_window를 확정된 반복트리거 방식으로 통합한다.

## 7. 작업 단계
1. Stage0 안정 경로를 기준선으로 고정하고, 측정 루틴 추가 전 회귀 위험 구간을 식별한다.
2. 공용 시간차 계산 헬퍼를 정의하고 TIM3 카운터 기준 환산 방식을 고정한다.
3. ADC 1회 변환시간 측정 루틴을 구현한다.
4. 100회 반복 측정 및 avg/min/max 산출 로직을 구현한다.
5. cmdIDDD_TRF_RUN 디스패치를 Stage1 기간 동안 측정 루틴으로 임시 매핑해 GUI TRF Run 경로에서 검증한다.
6. 확정된 max 기준 변환시간으로 trfrun adc_window 반복트리거 정책을 설계하고 통합한다.
7. 임시 매핑을 해제하고 trfrun 정상 디스패치로 원복한 뒤, 통합된 trfrun 경로를 재검증한다.
8. 5개 시간 파라미터의 런타임 입력 정책과 제약식 검증 경로를 설계한다.
9. ROM 저장 확장 필요 시 주소/기본값/로드 경로를 별도로 정리한다.
10. Stage0 회귀 여부를 다시 확인한다.

## 8. 파일별 역할(책임 경계)
- USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_TRF_Timer_CTRL.c
- USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_TRF_Timer_CTRL.h
- USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_ADC_SCAN_Drv.c
- USER/L2_Interface/L2_01_IDDD_RunCMD_Interface/IDDD_RunCmd_TRF_Interface.c
- USER/L3_Application/L3_00_hsDEBUG_UART_CLI_App/hsCLI_Command.c
- USER/L3_Application/L3_01_IDDD_RunCommand_app/IDDD_RunCommand_App.c

파일별 책임은 다음과 같이 해석한다.
- IDDD_TRF_Timer_CTRL.c/.h: TIM3 카운터 기준 측정 보조, 시간차 계산, 측정용 타이머 정책
- IDDD_ADC_SCAN_Drv.c: ADC 시작/완료, DMA TC 시점, 완료 플래그 처리
- IDDD_RunCmd_TRF_Interface.c: Stage0 안정 경로 유지, 측정 루틴 구현, trfrun adc_window 반복트리거 통합
- hsCLI_Command.c: 필요 시 진단용 trfmeas 명령 유지 및 출력 포맷 연결
- IDDD_RunCommand_App.c: Stage1 임시 매핑 적용/해제 및 명령 디스패치 원복 관리

## 9. 검증 항목
1. GUI TRF Run 입력 경로(전송 문자열 trfrun) 변경 없이 Stage1 측정 검증이 가능해야 한다.
2. 출력 포맷에 avg/min/max/count/pass가 모두 포함되어야 한다.
3. max 기준 판정 결과가 측정값과 일치해야 한다.
4. adc_window_us 안에서 허용 가능한 샘플 수 계산이 재현 가능해야 한다.
5. 제약식 위반 입력 시 즉시 오류 반환이 되어야 한다.
6. 임시 매핑 해제 후 trfrun 경로가 확정된 반복트리거 정책으로 정상 동작해야 한다.
7. Stage1 적용 후에도 trftim 및 Stage0 기본 경로 동작에 회귀가 없어야 한다.
8. 오실로스코프에서 PA7, PB11 기준 측정 타이밍이 설계와 일치해야 한다.

검증 산출물은 최소 아래를 남긴다.
- Stage1 측정 결과 로그(avg/min/max/count/pass)
- 오실로스코프 스크린샷
- avg/min/max 측정표
- adc_window_us 대비 허용 샘플 수 계산 근거

## 10. 위험요소 및 완화 전략
### 10.1 Stage0 회귀 위험
Stage1 구현 과정에서 Stage0 고정 시퀀스 경로가 직접 수정되면 회귀가 발생할 수 있다. 따라서 Stage0 경로와 Stage1 측정 경로는 분리해 관리한다.

### 10.2 DMA 완료 기준 변경 위험
DMA 완료 처리 시점을 수정하면 기존 ADC 완료 플래그 동작이 달라질 수 있다. 측정 전용 경로와 기존 경로를 분리해 영향 범위를 줄인다.

### 10.3 ARR 가변 적용 위험
cycle_us 기반 ARR 가변 적용은 Stage0 고정 2000us 기준을 깨뜨릴 수 있다. 가변 ARR 적용은 측정 또는 확장 경로에서만 사용하도록 제한한다.

## 11. 완료 기준
아래 조건을 모두 만족하면 Stage1 완료로 판정한다.
- GUI TRF Run 입력 경로 유지 상태에서 측정 루틴 검증 완료
- avg/min/max 결과 산출 완료
- max 기준 pass/fail 판정 완료
- adc_window_us 내 허용 샘플 수 확정
- trfrun adc_window 반복트리거 통합 완료
- 임시 매핑 해제 및 trfrun 정상 디스패치 원복 완료
- 제약식 위반 입력 오류 처리 완료
- Stage0 경로 회귀 없음

## 12. 다음 단계
Stage1 완료 후 Stage2로 이관한다.
Stage2에서는 런타임 파라미터 확장과 GUI/입력 연동 범위를 별도 단계로 다룬다.
