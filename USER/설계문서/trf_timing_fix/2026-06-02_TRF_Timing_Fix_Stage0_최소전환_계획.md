# 2026-06-02_TRF_Timing_Fix_Stage0_최소전환_계획

## 1. 목적
Stage0는 측정 통계나 GUI 확장 없이, 기존 trfrun 경로만으로 PWM 출력 의존성을 제거하고 소프트웨어 GPIO 기반 시퀀스를 재현하는 최소 구현 단계다.

## 2. 문서 사용 가이드(타 AI 에이전트 공용)
이 문서는 단독으로 읽어도 작업 배경, 변경 범위, 실패 이력, 검증 기준을 이해할 수 있도록 작성한다.

핵심 의도는 다음과 같다.
- 기존 PWM(AF 출력) 기반 동작을 제거하고 GPIO 토글 기반 시퀀스로 전환한다.
- 기존 명령 체계(trfrun, trftim)는 유지한다.
- Stage0는 최소 구현과 회귀 방지가 목적이며, 통계/GUI/ROM 확장은 의도적으로 제외한다.

이 문서에서 사용하는 용어는 다음과 같이 고정한다.
- LED ON: 발광 구간
- residual wait: 잔광 대기 구간
- ADC window: ADC 샘플링 유효 구간
- stabilize: 안정화 구간
- cycle: 1회 시퀀스 전체 주기

## 3. 시스템/하드웨어 컨텍스트
- MCU: STM32U031
- FW 구조: HAL + FreeRTOS + IAR(EWARM)
- 기존 방식: TIM3 PWM CH1/CH2를 PA6/PA7 AF로 직접 출력
- 전환 방식: TIM3는 시간 기준(counter only), PA6/PA7은 GPIO 소프트웨어 제어
- 관측 포인트: PA6, PA7, PB11(EXTI11)
- 관련 경로: PA7 -> R37/R39/C5 -> PB11

## 4. Stage0 범위
### 4.1 포함 범위
- PWM 출력 의존 제거
- TIM3 카운팅 전용(시간 기준) 유지
- 소프트웨어 GPIO 기반 5구간 시퀀스 재현
- 기존 trfrun, trftim 호출 경로 호환 유지
- 고정값 파라미터 기반 운용

### 4.2 제외 범위
- trfmeas 명령 추가
- avg/min/max 통계 기능
- CLI/GUI 5파라미터 가변 입력
- ROM 레지스터 확장
- GUI 화면/입력 확장

## 5. 고정값 운용 파라미터
- led_on_us = 1000
- residual_wait_us = 200
- adc_window_us = 700
- stabilize_us = 100
- cycle_us = 2000

제약식:
- led_on_us + residual_wait_us + adc_window_us + stabilize_us <= cycle_us

파생값:
- idle_tail_us = cycle_us - (led_on_us + residual_wait_us + adc_window_us + stabilize_us)
- 현재 고정값 기준 idle_tail_us = 0

타이머 가정:
- TIM3 tick = 1us
- Stage0에서 타이밍 기준은 microsecond 단위 정수 지연으로 해석한다.

## 6. 동작 시퀀스 정의(정규 기준)
trfrun 1회 실행 시 구간 순서는 아래와 같아야 한다.
1. LED ON: PA6/PA7 출력 제어 시작
2. residual wait: 잔광 대기
3. ADC window: ADC 유효 샘플링 구간
4. stabilize: 안정화 대기
5. cycle 종료: 다음 사이클 진입 또는 함수 종료

구현상 내부 상태명은 자유이나, 위 5단계 의미와 순서는 반드시 보존한다.

## 7. 작업 단계
1. PA6, PA7 핀 모드를 AF에서 GPIO 출력으로 전환하고 부팅 시 Low를 강제한다.
2. TIM3 초기화에서 PWM 채널 설정 경로를 제거하고 1us tick 기준의 카운터 전용으로 단순화한다.
3. trfrun 실행 경로에서 고정값 5구간(LED ON -> residual wait -> ADC window -> stabilize -> cycle 종료)을 소프트웨어 GPIO로 재현한다.
4. 내부 상수 검증(제약식)을 1회 수행해 잘못된 상수 편집 시 즉시 오류를 반환한다.
5. trfrun, trftim 회귀 동작을 확인한다.

## 8. 파일별 역할(책임 경계)
- Core/Src/main.c
- Core/Src/stm32u0xx_hal_msp.c
- USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_TRF_Timer_CTRL.c
- USER/L2_Interface/L2_01_IDDD_RunCMD_Interface/IDDD_RunCmd_TRF_Interface.c
- USER/L3_Application/L3_01_IDDD_RunCommand_app/IDDD_RunCommand_App.c

파일별 책임은 다음과 같이 해석한다.
- Core/Src/main.c: GPIO 초기 상태(boot low), TIM3 기본 설정
- Core/Src/stm32u0xx_hal_msp.c: TIM3 MSP/PostInit 경로와 핀 AF 연결 여부
- IDDD_TRF_Timer_CTRL.c: TRF 타이머 제어 API와 TIM3 의존 처리
- IDDD_RunCmd_TRF_Interface.c: trfrun 시퀀스 본체(5구간 정책)
- IDDD_RunCommand_App.c: 명령 디스패치 진입/호출 경로 유지

## 9. 검증 항목
1. trfrun 10회 연속 실행 시 타임아웃/에러 없이 완료되어야 한다.
2. 오실로스코프에서 PA6, PA7, PB11의 구간 순서가 설계 순서와 일치해야 한다.
3. 각 구간 목표값 대비 허용 오차는 +/-2us 이내여야 한다.
4. trftim, trfrun 기존 명령 동작에 회귀가 없어야 한다.
5. Stage0 제외 범위 항목이 실제 변경/검증에 포함되지 않아야 한다.

검증 산출물은 최소 아래를 남긴다.
- trfrun 10회 수행 로그(성공/실패)
- 오실로스코프 스크린샷(채널 라벨 포함)
- 기준값 대비 오차표(us)

## 10. 에러 발생 배경/원인/조치 이력(Stage0)
### 10.1 사례 A: FreeRTOS 핸들러 중복 정의
- 발생 배경: CubeMX 코드 생성 이후 인터럽트 핸들러 블록 상태가 변경됨
- 증상: PendSV_Handler, SVC_Handler, SysTick_Handler duplicate definitions 링크 에러
- 원인: FreeRTOS 포트에서 제공되는 핸들러와 stm32u0xx_it.c 구현이 동시 활성화됨
- 조치: stm32u0xx_it.c의 SVC/PendSV/SysTick 핸들러를 비활성화 처리
- 결과: 중복 정의 링크 에러 해소

### 10.2 사례 B: HAL_TIM_MspPostInit 미정의
- 발생 배경: PWM 제거 과정에서 MSP PostInit 경로 정리
- 증상: no definition for HAL_TIM_MspPostInit 링크 에러
- 원인: stm32u0xx_hal_msp.c에서 PostInit 구현이 제거되었으나, 사용자 드라이버에서 호출이 잔존함
- 조치: IDDD_TRF_Timer_CTRL.c 내 HAL_TIM_MspPostInit 호출 제거(또는 주석)
- 결과: 미정의 심볼 링크 에러 해소

### 10.3 재발 방지 규칙
- CubeMX 생성 직후 Stage0 대상 파일 외 변경 여부를 우선 점검한다.
- 링크 에러 발생 시 "정의 제거"와 "호출 잔존" 쌍을 함께 확인한다.
- FreeRTOS 핸들러는 단일 소스(포트 또는 사용자 파일)에서만 활성화한다.

## 11. 완료 기준
아래 조건을 모두 만족하면 Stage0 완료로 판정한다.
- PWM 출력 의존 제거 완료
- 소프트웨어 GPIO 시퀀스 재현 확인
- 오실로스코프 검증 통과
- 기존 명령 회귀 없음

## 12. 다음 단계
Stage0 완료 후 Stage1로 이관한다.
Stage1에서는 trfmeas 및 변환시간 통계(avg/min/max) 기능을 별도 범위로 다룬다.
