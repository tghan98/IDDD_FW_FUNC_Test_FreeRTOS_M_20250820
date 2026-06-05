# 2026-06-04_TRF_Timing_Fix_Stage0_최소전환_결과

## 1. 목적
Stage0 최소전환 작업에서 PWM 출력 의존성을 제거하고, trfrun 경로를 소프트웨어 GPIO 시퀀스로 전환한 구현 결과를 정리했다. 본 문서는 실제 수행한 변경과 검증 상태를 기록하고, Stage1 이관 전에 남은 확인 항목을 명확히 남기는 것을 목적으로 작성했다.

## 2. 수행내용
### 2.1 상수 이름부터 정리했다
[USER/L2_Interface/L2_01_IDDD_RunCMD_Interface/IDDD_RunCmd_TRF_Interface.c](USER/L2_Interface/L2_01_IDDD_RunCMD_Interface/IDDD_RunCmd_TRF_Interface.c) 상단 define 구역에 5개 시간 상수를 의미형 이름으로 정리해 반영했다. LED ON, residual wait, ADC window, stabilize, cycle 시간을 각각 분리해 가독성과 유지보수성을 높였다.

### 2.2 실행 전 안전검사 함수 1개를 추가했다
같은 파일에 제약식 검사 함수를 추가해 실행 전에 합산 시간 조건을 확인하도록 구성했다. LED ON + residual wait + ADC window + stabilize <= cycle 조건을 만족하지 않으면 즉시 오류를 반환하도록 하여 상수 편집 실수를 조기에 차단했다.

### 2.3 타이머 대기 헬퍼 3개를 추가했다
TIM3 CNT 기반으로 타이머 시작, 절대시각 대기, 타이머 정지 헬퍼를 분리해 반영했다. 상대 지연이 아니라 절대시각 기준 대기 방식으로 구성해 구간 오차 누적을 줄였다.

### 2.4 5구간 1사이클 함수 1개를 추가했다
LED ON 시작, LED ON 종료, residual wait, ADC window 시작과 종료, stabilize와 cycle 종료까지의 순서를 1사이클 함수로 구성했다. PA6과 PA7 제어 타이밍을 함수 내부에서 순차적으로 처리하도록 구현해 구간 의미가 코드에서 직접 보이도록 정리했다.

### 2.5 기존 TRF Run 본문을 교체했다
기존 PWM Start + 100ms polling 루프 구조를 제거하고, 프로파일 검사 -> DMA 시작 -> ADC complete flag가 SET인 동안 1사이클 반복 -> timeout 검사 -> 종료 시 PA6/PA7 Low 정리 순서로 본문을 교체했다. 기존 골격 중 ADC 설정, LED 전류 설정, 채널 선택, DMA start/stop, 결과 출력 경로는 유지했다.

## 3. 다음 단계
- 잔여 검증 3항목을 완료한 뒤 Stage0 완료 판정을 확정한다.
- Stage0 완료 판정 후 Stage1(trfmeas 및 통계 기능) 범위를 별도 문서로 이관한다.
