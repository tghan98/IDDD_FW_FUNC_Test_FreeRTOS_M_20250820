# 2026-05-29_TRF_Timing_Fix_계획

## 1. 설계방향
1순위 목표는 TIM3 기반 PWM Output 방식을 Timer 카운팅 + 소프트웨어 GPIO 방식으로 전환하는 것이다.
타이머는 시간 기준만 제공하고, LED/ADC 제어는 펌웨어 상태 제어로 수행한다.
2순위 목표는 ADC 1회 변환(샘플 취득) 소요시간을 us 단위로 실측하여 ADC 유효 구간 내 샘플링 가능 횟수를 확정하는 것이다.

## 2. 구현 핵심

### 2.1 확정 파라미터
| 항목 | 값 |
|---|---|
| 측정 명령어 | `trfmeas` |
| 변환 완료 기준 | DMA 완료(DMA TC) |
| 측정 횟수 | 100회 |
| 트리거 펄스 High | 1us |
| 트리거 펄스 Low | 6us |
| 1샘플 주기 | 7us(High+Low) |
| 시간 측정 소스 | TIM3->CNT (1tick=1us) |
| 시간차 계산 | 오버플로우 안전 계산(공용 inline 헬퍼) |
| 판정 기준 | max 기준 |
| 결과 보고 | avg/min/max 동시 제시 |
| 출력 포맷 | `trfmeas: avg=%luus min=%luus max=%luus count=%lu pass=%s` |
| LED ON 시간 led_on_us | 가변 파라미터(기본값 1000us) |
| 잔광대기 시간 residual_wait_us | 가변 파라미터(기본값 200us, 범위 200~400us 권장) |
| ADC 샘플링 구간 adc_window_us | 가변 파라미터(기본값 700us) |
| 안정화 구간 stabilize_us | 가변 파라미터(기본값 100us) |
| 전체 주기 cycle_us | 가변 파라미터(1us 단위 임의 입력, 기본값 2000us, 권장 범위 500~5000us) |
| ADC 트리거 경로 | PA7 출력 → R37/R39/C5 → PB11(EXTI11) (회로 변경 없음) |
| 호환 유지 | 기존 명령 `trftim`, `trfrun` 동작 유지 |

운용 시점 파라미터는 모두 가변으로 설정 가능하되, 다음 제약을 만족해야 한다.
- led_on_us + residual_wait_us + adc_window_us + stabilize_us <= cycle_us
- 각 파라미터는 0 이상이어야 한다.
- adc_window_us는 1샘플 주기(7us, High 1us + Low 6us) 이상이어야 한다.
- 제약식 위반 입력 시 명령은 즉시 오류를 반환한다.

### 2.2 핀 모드 변경
| 핀 | 기존 | 변경 후 |
|---|---|---|
| PA6 | TIM3_CH1 (AF) | GPIO_Output (LED 제어, 펌웨어 토글) |
| PA7 | TIM3_CH2 (AF) | GPIO_Output (ADC 트리거 펄스, 펌웨어 토글) |
| PB11 | EXTI11 입력 | 변경 없음 |
| PC0 | ADC1_IN0 | 변경 없음 |

### 2.3 작업 순서 (Phase)
1. Phase 1 — 핀 모드 전환  
   파일: `Core/Src/main.c`, `Core/Src/stm32u0xx_hal_msp.c`  
   작업: PA6/PA7 AF 설정 제거, GPIO_Output PP 초기화. 기동 시 Low 유지.
2. Phase 2 — 공용 시간차 헬퍼 추가  
   파일: `USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_TRF_Timer_CTRL.h`  
   작업: `static inline uint32_t TRF_DeltaTick(uint32_t start, uint32_t end, uint32_t arr)` 정의. 오버플로우 안전 계산 적용.
3. Phase 3 — TIM3 카운팅 전용 초기화  
   파일: `USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_TRF_Timer_CTRL.c`  
   작업: PWM 채널 설정 제거, Prescaler=55(1us tick) 유지, ARR은 cycle_us-1로 가변 적용. Start/Stop 함수는 카운터 제어만 수행.
4. Phase 4 — 측정 루틴 구현  
   파일: `USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_ADC_SCAN_Drv.c`, `IDDD_TRF_Timer_CTRL.c`  
   작업: PA7 트리거 펄스(High 1us / Low 6us) 100회 발생 + DMA TC 시점 CNT 캡처 + avg/min/max 산출.
5. Phase 5 — CLI 명령 `trfmeas` 추가  
   파일: `USER/L3_Application/L3_00_hsDEBUG_UART_CLI_App/hsCLI_Command.c`  
   작업: 측정 루틴 호출 후 결과를 출력 포맷에 맞춰 출력.
6. Phase 6 — 1순위 시퀀스 통합  
   파일: `USER/L2_Interface/L2_01_IDDD_RunCMD_Interface/IDDD_RunCmd_TRF_Interface.c`  
   작업: 기존 `trfrun` 경로에서 LED ON, 잔광대기, ADC 샘플링, 안정화, 주기 종료까지를 소프트웨어 GPIO + 타이머 카운팅 기반 상태 시퀀스로 재구성한다. 다섯 개 시간 파라미터(led_on_us, residual_wait_us, adc_window_us, stabilize_us, cycle_us)를 런타임 입력값으로 적용하며, 제약식 위반 시 즉시 오류를 반환한다.

## 3. 검증 핵심
1. 측정 절차: `trfmeas` 실행 → 100회 PA7 트리거 → DMA TC 기준 변환시간 캡처 → avg/min/max 출력.
2. 출력 포맷: `trfmeas: avg=%luus min=%luus max=%luus count=%lu pass=%s` 형식만 사용.
3. 합격 판정: max 기준으로 자동 판정. `pass=OK` 또는 `pass=NG` 출력.
4. 오실로스코프 교차 검증: PA7(트리거 출력), PB11(EXTI 입력), 필요 시 PA6(LED). 펄스 폭/지연/총 길이 비교.
5. 호환 유지 검증: 기존 `trftim`, `trfrun` 명령이 변경 후에도 정상 동작해야 한다.
6. 오류 입력 검증: 범위 초과 입력 시 즉시 오류 반환되어야 한다.
7. 가변성 검증: cycle_us를 임의값(예: 1000us, 1217us, 2000us 등 1us 단위)으로 입력하고 led_on_us, residual_wait_us, adc_window_us, stabilize_us 조합을 변경해도 시퀀스 순서와 타이밍 제약이 유지되어야 한다. 제약식 위반 입력 시 명령은 즉시 NG를 반환해야 한다.

## 4. 완료 기준
1. 1순위 합격: PWM 자동 출력 없이 동일 기능 재현. LED ON/OFF·ADC 트리거 시점이 설정값 대비 ±2us 이내.
2. 2순위 합격: 100회 측정에서 max ≤ 7us. count=100, pass=OK.
3. 교차 검증 합격: 오실로스코프 실측 펄스폭/주기와 `trfmeas` 결과가 ±2us 이내로 일치.
4. 호환 합격: 기존 명령 동작 회귀 없음.
5. 위 4개 항목이 모두 만족되면 본 변경을 완료로 판정한다.
