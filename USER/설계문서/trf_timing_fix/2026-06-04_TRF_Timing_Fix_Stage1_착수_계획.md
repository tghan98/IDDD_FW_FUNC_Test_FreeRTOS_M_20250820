# 2026-06-04_TRF_Timing_Fix_Stage1_착수_계획

## 1. 목적
Stage1은 Stage0에서 확보한 Timer 카운팅 + 소프트웨어 GPIO 기반 5구간 시퀀스를 회귀 없이 유지한 상태에서, ADC가 실제로 변환에 소요하는 시간을 계측하고, 그 결과를 기반으로 adc_window 구간 내 수용 가능한 샘플 수를 보수적으로 확정하는 단계다. 본 단계의 측정은 외부 트리거 핀 경로(PA7 -> 회로 -> PB11 EXTI)를 사용하지 않고, ADC를 SW Start + DMA 버스트(100샘플) 방식으로 한 번에 수집하며, 시간 계측은 TIM3 카운터(CNT)의 시작/완료 시점 차이로 산출한다. GUI 입력 경로(전송 문자열 trfrun)는 변경하지 않으며, Stage1 검증 기간에는 펌웨어 내부 디스패치에서 측정 루틴을 임시 매핑해 검증한 뒤, 확정된 결과를 trfrun의 adc_window 정책으로 통합한다.

## 2. Stage0 선행조건 및 회귀 보호
Stage1은 다음 Stage0 완료 조건이 충족된 상태에서 착수한다.
- PWM 출력 의존 제거 완료
- Timer 카운팅 + 소프트웨어 GPIO 기반 5구간 시퀀스 재현 완료
- 오실로스코프 기준 PA6, PA7, PB11 구간 순서 확인 완료
- trfrun 10회 연속 실행 시 timeout/error 없음 확인 완료
- trftim, trfrun 기존 명령 회귀 없음 확인 완료

Stage1 구현 중에도 Stage0 안정 경로는 직접 수정하지 않으며, 측정 경로와 분리해 관리한다.

## 3. 측정 방식 결정사항(핵심 변경)
### 3.1 트리거 정책
Stage1 측정은 외부 트리거 핀(PA7) 및 EXTI(PB11) 경로를 사용하지 않는다. 외부 트리거 체인은 Stage0 시퀀스 검증 용도로만 유지하며, Stage1 측정 루틴에서는 일절 사용하지 않는다.

### 3.2 ADC 동작 모드
ADC는 외부 트리거 모드를 사용하지 않고, 소프트웨어 시작(SW Start) + DMA 모드로 동작시킨다. DMA는 100샘플을 한 번에 수집하도록 길이를 100으로 설정한다. 즉, 100회를 개별 트리거로 반복하는 것이 아니라, 한 번의 시작으로 ADC가 연속 변환을 수행하면서 DMA가 100개를 자동으로 채운 뒤 TC 인터럽트를 발생시키는 구조다.

### 3.3 시간 측정 방식(TIM CNT 타임스탬프)
ADC 1회 변환 시간 및 100샘플 총 수집 시간은 TIM3 카운터(CNT)의 차이값으로 산출한다. 측정 시작 직전과 DMA 완료(TC) 시점의 CNT를 각각 읽어, 오버플로우 안전한 시간차 계산 헬퍼로 us 단위 차이를 구한다. 1샘플당 평균 시간은 (100샘플 총 시간 / 100)으로 환산한다. 본 단계의 avg/min/max는 100샘플 총시간 기준으로 집계한다.

### 3.4 GPIO 마커 정책
일반 GPIO는 트리거가 아니라 측정 구간 마커(디버그 마커)로만 사용한다. 측정 시작 직전에 마커 핀을 High(또는 Low)로 토글하고, DMA 완료 후 플래그를 폴링으로 인식한 직후 반대 레벨로 토글해 오실로스코프에서 측정 구간 폭을 직접 관찰한다. 마커 핀은 기존 외부 트리거용 알트펑션/EXTI 설정과 충돌하지 않도록 일반 출력 모드(Push-Pull)로 재설정한다.

### 3.5 DMA 완료 동기화(전역 플래그)
ADC SW Start + DMA(100) 호출 후 메인 측정 루틴은 DMA 완료를 기다려야 한다. DMA TC 콜백(인터럽트)과 메인 루틴 사이의 동기화는 전역 플래그로 처리한다. 플래그는 반드시 volatile로 선언해 컴파일러 최적화로 인한 무한 대기 위험을 제거한다.

기본 동작 시퀀스는 다음과 같다.
1. 측정 시작 직전에 플래그를 RESET 한다.
2. 시작 CNT를 기록한다.
3. 마커 GPIO를 시작 레벨로 토글한다.
4. ADC SW Start + DMA(length=100)를 호출한다.
5. DMA TC 콜백에서 종료 CNT를 기록하고 플래그를 SET 한다.
6. 메인 루틴은 플래그가 SET 될 때까지 대기한다.
7. 종료 CNT - 시작 CNT 차이를 계산해 측정값에 누적한다.
8. 마커 GPIO를 종료 레벨로 토글한다.

## 4. 오실로스코프 관찰 기준
Stage1 측정에서는 마커 GPIO를 관찰한다. 마커는 100회 개별 트리거 펄스 열이 아니라, 100샘플 수집 구간 전체를 덮는 폭이 큰 단일 펄스 1개로 보인다. 즉, “100번 모양”은 보이지 않으며, 그 단일 펄스의 폭이 100샘플 수집 총 시간의 증거가 된다. 외부 트리거 핀 파형은 Stage1 측정 중에는 동작하지 않으므로 관찰 대상이 아니다.

## 5. Stage1 범위
### 5.1 포함 범위
- 외부 트리거 사용을 배제한 ADC SW Start + DMA(100) 측정 루틴 구현
- TIM3 CNT 기반 시작/완료 타임스탬프 측정 및 시간차 산출
- 마커용 일반 GPIO 모드 재설정 및 토글 정책 구현
- DMA TC 동기화를 위한 volatile 플래그 정의 및 사용
- 100회 반복 측정 기반 avg/min/max 산출
- max 기준 pass/fail 판정
- 확정된 결과로 trfrun adc_window 수용 샘플 수 정책 통합
- 5개 시간 파라미터 런타임 입력 경로 설계
- 제약식 검증 로직 확장
- ROM 저장 확장 검토

### 5.2 제외 범위
- Stage0 고정 시퀀스 경로 재설계
- 외부 트리거 경로(PA7 -> PB11 EXTI) 사용
- GUI 화면/입력 UI 구현
- GUI TRF Run 전송 문자열 변경(trfrun -> trfmeas)
- BLE/App 연동 확장
- Stage0 검증 산출물 재작성

## 6. 파라미터 및 판정 정책
### 6.1 시간 파라미터
- led_on_us
- residual_wait_us
- adc_window_us
- stabilize_us
- cycle_us

### 6.2 기본 운용 기준 및 제약식
Stage0 기본값은 초기 기준값으로 유지하고, Stage1에서는 위 5개 값을 런타임 입력 가능 대상으로 확장한다. 제약식은 다음과 같이 유지한다.
- led_on_us + residual_wait_us + adc_window_us + stabilize_us <= cycle_us
- 각 파라미터는 0 이상이어야 한다.
- adc_window_us는 최소 1샘플 주기 이상이어야 한다.

### 6.3 측정 판정 기준
- 측정 횟수: 100회 반복(각 반복은 100샘플 버스트 1회 수행)
- 측정 기준 시점: DMA 완료(DMA TC)
- 출력 항목: avg, min, max, count, pass (avg/min/max 단위: 100샘플 총시간)
- pass/fail 판정은 max 기준으로 한다.
- 1차 목적은 adc_window_us 내 수용 가능한 샘플 수를 보수적으로 산정하는 것이다.

## 7. 구현 핵심
### 7.1 GPIO 마커 재설정
측정용 마커 핀은 외부 트리거/알트펑션/EXTI 설정과 분리해 일반 출력 모드(Push-Pull)로 재설정한다. 동일 핀이 기존에 외부 경로용으로 잡혀 있던 경우, Stage1 측정 진입 시점에 모드를 명시적으로 재구성해 충돌을 방지한다.

### 7.2 전역 플래그(volatile)
DMA TC와 메인 루틴 간 동기화를 위해 전역 플래그를 volatile로 선언한다. 플래그는 측정 시작 직전에 RESET, DMA TC 콜백에서 SET 한다. 폴링 대기 루프는 타임아웃을 포함해 무한 대기를 방지한다.

### 7.3 측정 루틴 구조
한 번의 측정은 다음 순서로 수행한다.
1. 플래그 RESET
2. 시작 CNT 기록
3. 마커 GPIO 시작 토글
4. ADC SW Start + DMA(100) 호출
5. 플래그 SET 대기(타임아웃 포함)
6. 종료 CNT 기록(TC 콜백에서 캡처)
7. 마커 GPIO 종료 토글
8. (종료 CNT - 시작 CNT) 차이 계산 후 결과 누적

이 절차를 100회 반복해 avg/min/max를 산출한다.

### 7.4 공용 시간차 계산 헬퍼
TIM3 CNT 기준 시간차 계산은 오버플로우 안전한 공용 헬퍼로 일원화한다. ARR/PSC 변경 시에도 환산식이 흔들리지 않도록 헬퍼 내부에서 us 환산을 책임진다. 측정 루틴, trfrun 통합 경로, 진단 명령이 동일한 헬퍼를 사용하도록 한다.

### 7.5 샘플 수 결정 방식
adc_window_us 내 수용 가능한 샘플 수는 평균값이 아니라 max(최악값) 기준으로 산정한다. 즉, 1샘플 환산 max 기준 변환시간으로 adc_window_us를 나누어 보수적으로 결정한다.

### 7.6 Stage1 임시 매핑 원칙
GUI TRF Run 전송 문자열(trfrun)은 변경하지 않는다. Stage1 검증 기간에 한해 펌웨어 내부 디스패치에서 cmdIDDD_TRF_RUN이 측정 루틴을 타도록 임시 매핑한다. Stage1 확정 후에는 디스패치를 정상 trfrun 경로로 원복하고, trfrun 내부 adc_window 정책을 확정된 수용 샘플 수 기준으로 통합한다.

## 8. 작업 단계
1. Stage0 안정 경로를 기준선으로 고정하고 회귀 위험 구간을 식별한다.
2. 공용 시간차 계산 헬퍼를 정의하고 TIM3 CNT 기준 환산 방식을 고정한다.
3. 마커용 GPIO를 일반 출력 모드로 재설정하는 초기화 경로를 추가한다.
4. DMA TC 동기화용 volatile 전역 플래그를 정의하고 콜백/대기 루프를 구현한다.
5. ADC SW Start + DMA(100) 호출 경로 및 측정 1회 루틴을 구현한다.
6. 100회 반복 측정 및 avg/min/max 산출 로직을 구현한다.
7. cmdIDDD_TRF_RUN 디스패치를 Stage1 기간 동안 측정 루틴으로 임시 매핑해 GUI TRF Run 경로에서 검증한다.
8. 확정된 max 기준 결과로 trfrun adc_window 수용 샘플 수 정책을 설계해 통합한다.
9. 임시 매핑을 해제하고 trfrun 정상 디스패치로 원복한 뒤 재검증한다.
10. 5개 시간 파라미터의 런타임 입력 정책과 제약식 검증 경로를 설계한다.
11. ROM 저장 확장 필요 시 주소/기본값/로드 경로를 별도로 정리한다.
12. Stage0 회귀 여부를 재확인한다.

## 9. 파일별 역할(책임 경계)
- USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_TRF_Timer_CTRL.c
- USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_TRF_Timer_CTRL.h
- USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_ADC_SCAN_Drv.c
- USER/L2_Interface/L2_01_IDDD_RunCMD_Interface/IDDD_RunCmd_TRF_Interface.c
- USER/L3_Application/L3_00_hsDEBUG_UART_CLI_App/hsCLI_Command.c
- USER/L3_Application/L3_01_IDDD_RunCommand_app/IDDD_RunCommand_App.c

파일별 책임은 다음과 같이 해석한다.
- IDDD_TRF_Timer_CTRL.c/.h: TIM3 CNT 기준 타임스탬프 캡처 보조, 오버플로우 안전 시간차 계산 헬퍼 제공
- IDDD_ADC_SCAN_Drv.c: ADC SW Start + DMA(100) 시작/완료 처리, DMA TC 콜백에서 종료 CNT 캡처 및 volatile 플래그 SET
- IDDD_RunCmd_TRF_Interface.c: Stage0 안정 경로 유지, Stage1 측정 루틴 구현, trfrun adc_window 수용 샘플 수 정책 통합
- hsCLI_Command.c: 필요 시 진단용 명령(trfmeas 등) 유지 및 출력 포맷 연결
- IDDD_RunCommand_App.c: Stage1 임시 매핑 적용/해제 및 명령 디스패치 원복 관리

## 10. 검증 항목 및 산출물
### 10.1 검증 항목
1. GUI TRF Run 입력 경로(전송 문자열 trfrun) 변경 없이 Stage1 측정 검증이 가능해야 한다.
2. 외부 트리거 핀(PA7) 및 EXTI(PB11) 경로가 측정 중 비활성 상태여야 한다.
3. 출력 포맷에 avg/min/max/count/pass가 모두 포함되어야 한다.
4. max 기준 판정 결과가 측정값과 일치해야 한다.
5. adc_window_us 내 허용 샘플 수 계산이 재현 가능해야 한다.
6. 제약식 위반 입력 시 즉시 오류 반환이 되어야 한다.
7. 임시 매핑 해제 후 trfrun 경로가 확정된 수용 샘플 수 정책으로 정상 동작해야 한다.
8. Stage1 적용 후에도 trftim 및 Stage0 기본 경로 동작에 회귀가 없어야 한다.
9. 오실로스코프 기준 마커 GPIO에 100샘플 수집 구간을 덮는 단일 펄스가 관찰되어야 한다.

### 10.2 검증 산출물
- Stage1 측정 결과 로그(avg/min/max/count/pass)
- 마커 GPIO 단일 구간 펄스 오실로스코프 스크린샷
- avg/min/max 측정표
- adc_window_us 대비 허용 샘플 수 계산 근거

## 11. 위험요소 및 완화 전략
### 11.1 Stage0 회귀 위험
Stage1 구현 과정에서 Stage0 고정 시퀀스 경로가 직접 수정되면 회귀가 발생할 수 있다. Stage0 경로와 Stage1 측정 경로는 분리해 관리한다.

### 11.2 외부 트리거 잔류 설정 위험
기존 외부 트리거 모드/EXTI/알트펑션 설정이 잔류한 상태에서 마커 GPIO를 토글하면 의도하지 않은 트리거가 발생할 수 있다. Stage1 측정 진입 시 핀 모드를 일반 출력으로 명시 재설정한다.

### 11.3 DMA 동기화 위험
플래그를 volatile로 선언하지 않으면 컴파일러 최적화로 인해 무한 대기에 빠질 수 있다. 또한 대기 루프에 타임아웃이 없으면 비정상 상황에서 시스템이 멈출 수 있다. volatile 선언과 타임아웃을 강제한다.

### 11.4 ARR 가변 적용 위험
cycle_us 기반 ARR 가변 적용은 Stage0 고정 2000us 기준을 깨뜨릴 수 있다. 가변 ARR 적용은 측정 또는 확장 경로에서만 사용하도록 제한한다.

## 12. 완료 기준
다음 조건을 모두 만족하면 Stage1 완료로 판정한다.
- 외부 트리거 미사용 ADC SW Start + DMA(100) 측정 루틴 동작 확인
- TIM3 CNT 기반 시간차 산출 동작 확인
- 마커 GPIO 단일 구간 펄스 오실로스코프 확인
- DMA TC 동기화 플래그(volatile) 정상 동작 확인
- 100회 반복 측정 기반 avg/min/max 산출 완료
- max 기준 pass/fail 판정 완료
- adc_window_us 내 허용 샘플 수 확정
- trfrun adc_window 수용 샘플 수 정책 통합 완료
- 임시 매핑 해제 및 trfrun 정상 디스패치 원복 완료
- 제약식 위반 입력 오류 처리 완료
- Stage0 경로 회귀 없음

## 13. 다음 단계
Stage1 완료 후 Stage2로 이관한다. Stage2에서는 런타임 파라미터 확장과 GUI/입력 연동 범위, 그리고 외부 트리거 경로의 향후 운용 정책을 별도 단계로 다룬다.
