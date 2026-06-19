# 2026-06-16_TRF_Timing_Fix_Stage3_LED점등버그수정_실측

## 1. 작업 목적

Stage2 `IDDD_RunCmd_TRF_Real()`에서 발견된 LED 채널 ON/OFF 무한 토글 버그를 수정한다. 버그로 인해 1200µs 이후 OFF가 실행된 시점부터 LED 채널이 CH1↔CH_OFF 간 20µs 주기로 토글되어, LED가 안정적으로 점등되지 않았으며 이로 인해 1.0mm 샘플 카세트에서 형광감쇠 신호가 전혀 검출되지 않았다.

## 2. 수행내용

### 2.1 버그 원인 분석

`TRF_Real()`의 LED ON/OFF 분기에서 `bLedOn` 하나의 플래그로 양쪽을 통제한 것이 근본 원인이다. CNT가 1200µs에 도달하면 OFF 분기가 실행되면서 `bLedOn = 0`으로 리셋되는데, 이후 모든 루프에서 `dwTim >= 200`도 참이므로 ON 분기가 재실행된다. ON 실행 직후 `bLedOn = 1`이 되고, 같은 루프에서 `dwTim >= 1200`도 참이므로 OFF도 다시 실행되는 구조로, 1200~1980µs 전 구간에서 ON→OFF→ON→OFF가 무한 반복되었다.

타임라인으로 정리하면 다음과 같다.

0~190µs: 암구간. ON/OFF 조건 모두 거짓. LED 채널 OFF 상태 유지. 정상.

200~1180µs: ON 조건만 참. `IDDD_LED_Current_Set` → `IDDD_LED_Channel_Select(CH1)` → PA6 HIGH 순으로 LED ON 실행. `bLedOn=1`로 설정되어 중복 방지. 정상.

1200µs: OFF 조건이 최초로 참이 되면서 `IDDD_LED_Channel_Select(CH_OFF)` 실행. `bLedOn=0`으로 리셋.

1220µs: ON 조건 참(bLedOn=0) → ON 실행 → bLedOn=1. 직후 OFF 조건도 참(bLedOn=1) → OFF 실행 → bLedOn=0.

1240~1980µs: 1220µs와 동일한 ON→OFF 토글이 20µs마다 반복. LED 채널이 안정 시간 없이 CH1↔CH_OFF 전환되어 LED 전류가 정상 흐르지 않음.

### 2.2 수정 방안

ON 전용 플래그와 OFF 전용 플래그를 분리하여, OFF 실행이 ON 플래그에 영향을 주지 않도록 한다.

변경 전 코드 구조는 다음과 같다.

uint8_t bLedOn = 0;

if((dwTim >= TRF_REAL_LED_ON_US) && (bLedOn == 0))
{
    IDDD_LED_Current_Set(dwLedCurr);
    IDDD_LED_Channel_Select(dwOptCh);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    bLedOn = 1;
}

if((dwTim >= TRF_REAL_LED_OFF_US) && (bLedOn == 1))
{
    TRF_SequenceSignal_SetLow();
    IDDD_LED_Channel_Select(OPT_LED_CH_OFF);
    bLedOn = 0;
}

변경 후 코드는 ON/OFF에 각각 독립 플래그를 할당한다.

uint8_t bLedOnDone  = 0;
uint8_t bLedOffDone = 0;

if((dwTim >= TRF_REAL_LED_ON_US) && (bLedOnDone == 0))
{
    IDDD_LED_Current_Set(dwLedCurr);
    IDDD_LED_Channel_Select(dwOptCh);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    bLedOnDone = 1;
}

if((dwTim >= TRF_REAL_LED_OFF_US) && (bLedOffDone == 0))
{
    TRF_SequenceSignal_SetLow();
    IDDD_LED_Channel_Select(OPT_LED_CH_OFF);
    bLedOffDone = 1;
}

ON 플래그는 ON 실행 시에만 1로 설정되고 이후 변경되지 않으며, OFF 플래그는 OFF 실행 시에만 1로 설정되고 이후 변경되지 않는다. 양쪽이 완전히 독립적이므로 더 이상 토글이 발생하지 않는다.

수정 대상 파일은 `IDDD_RunCmd_TRF_Interface.c`이며, 변경 범위는 `bLedOn` 변수 선언부 1곳과 ON/OFF 조건부 각 1곳이다. define이나 ADC 설정, 시퀀스 타이밍은 전혀 변경하지 않는다.

### 2.3 OPT_LED_nONOFF 극성 사전 확인

PA6 HIGH가 LED ON인지 OFF인지는 코드에서 단정하지 않고, 회로도와 실측으로 확정한다. 회로도상 `OPT_LED_nONOFF`는 `n` prefix 신호명으로 보아 active LOW일 가능성이 있으며, OPT_LED_VCC가 4.49V로 이미 ON 상태인 점을 감안하면 PA6 HIGH가 LED OFF에 해당할 수 있다. 수정 전에 PA6를 LOW/HIGH로 각각 설정했을 때 OPT_LED_VCC 및 LED 전류 변화를 실측하여 극성을 확정한 후, 극성에 맞춰 `HAL_GPIO_WritePin`의 SET/RESET 인자를 결정한다.

### 2.4 측정 검증 항목 (하드웨어 실측 기반)

수정 후 아래 하드웨어 신호를 실측하여 버그 해소를 확인한다.

OPT_LED_SEL1 전압은 trfrun 중 200~1200µs 동안 LOW(0V), 0~200µs 및 1200µs 이후는 HIGH(3.3V)로 고정되어야 한다. 수정 전에는 1200µs 이후 LOW↔HIGH 토글이 관찰되었으므로, 1200µs 이후 HIGH로 고정되면 수정 성공이다.

SR1 sense 전압은 LED 전류 흐름의 주 판단 기준이며, 200~1200µs 동안 0V 초과(전류 흐름), 그 외 구간은 0V로 관찰되어야 한다.

OPT_LED_nONOFF(PA6)는 실측으로 확정된 극성에 따라 LED ON 구간(200~1200µs)에서 LED가 켜지는 레벨, 그 외 구간에서는 꺼지는 레벨로 고정되어야 한다.

OPT_LED_VCC는 trfrun 전체 구간에서 4.49V로 유지되어야 한다.

### 2.5 PA6 nONOFF 극성 반전 테스트 (신규)

다른 정상 동작 기기에서 동일 LED가 육안으로 빛이 방출되는 것이 확인되었다. 현재 기기에서 LED 빛이 전혀 보이지 않으므로, PA6 `GPIO_PIN_SET`(HIGH)가 실제로는 LED OFF에 해당할 가능성이 높다. 회로도상 `OPT_LED_nONOFF`의 `n` prefix도 active LOW임을 시사한다.

PA6 극성을 반전하여 ON/OFF를 교차 검증한다. LED ON에서 `GPIO_PIN_RESET`(LOW), LED OFF에서 `GPIO_PIN_SET`(HIGH)로 변경한다. 변경 후 LED가 육안으로 확인되고 빈카세트에서 TIA 반응 크기가 변화하면 극성 문제가 확정된다.

코드 변경:

// 7c) LED ON: SET → RESET
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);

// 7d) LED OFF: TRF_SequenceSignal_SetLow() → PA6만 SET
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
IDDD_LED_Channel_Select(OPT_LED_CH_OFF);

## 3. 검증 결과

### 3.1 코드 수정 및 빌드

`bLedOn` 플래그를 `bLedOnDone`/`bLedOffDone` 독립 플래그로 분리하고, PA6 극성을 `GPIO_PIN_SET`(HIGH)→`GPIO_PIN_RESET`(LOW)로 반전하여 `IDDD_RunCmd_TRF_Interface.c`에 적용하였다. IAR EWARM 9.0 빌드 결과 errors=0, warnings=0, 바이너리 정상 생성. Stage0 경로 및 Stage1Meas 경로 회귀 없음.

### 3.2 하드웨어 신호 실측

OPT_LED_SEL1(R42) 전압을 trfrun 중 측정한 결과, 200~1200µs 구간에서 LOW(0V)로 고정되고 1200µs 이후 HIGH(3.3V)로 고정되어 토글이 완전히 해소되었음을 확인하였다. SR1 sense 전압은 200~1200µs 구간에서 0V 초과로 전류 흐름이 확인되었으며, 그 외 구간에서는 0V였다. OPT_LED_nONOFF(PA6)는 LOW(0V)일 때 LED가 점등되고 HIGH(3.3V)일 때 소등되는 active LOW 극성으로 확정되었다. OPT_LED_VCC는 전 구간에서 4.49V로 안정적이었다.

### 3.3 빈카세트 측정 (ADC 로그)

LED ON 구간(200~1200µs)에서 TIA가 1522~2363 범위로 강하게 반응하였다. DC 결합 TIA 특유의 링잉 진동이 관찰되었으며, 1200µs LED OFF와 동시에 ADC 값이 124로 수직 하강하여 암전류(123~138)로 즉시 복귀하였다. LED 점등과 PD 감지가 모두 정상 동작함을 확인하였다.

### 3.4 1.0mm 카세트 측정 (ADC 로그)

LED ON 구간(200~1200µs)에서 ADC 값이 1111~1798로 빈카세트 대비 약 500 count 낮게 나타나 샘플이 여기광을 흡수하고 있음을 확인하였다. 1200µs LED OFF 후에는 123~138 범위에서 빈카세트와 완전히 동일한 패턴으로 평탄하게 유지되어, 형광감쇠 신호는 검출되지 않았다.

## 4. 잔여 이슈/TODO

- 1.0mm 샘플에서 형광감쇠 미검출. LED ON 구간에서 광 흡수(1111~1798)는 확인되었으나 OFF 후 신호 없음. 광학 필터 파장 불일치, 형광 수율 부족, 또는 형광 수명이 20µs보다 짧아 샘플링에서 놓치는 등 HW 측면 추적 필요

## 5. 다음 단계

- 광학 필터/샘플 농도/형광 수율 등 HW 원인 검토
- 필요시 샘플링 간격 축소(1~2µs)로 OFF 직후 고속 측정 검토
