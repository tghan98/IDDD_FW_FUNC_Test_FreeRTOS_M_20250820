# 2026-07-09_TRF_Timing_Fix_Stage5_2cycle_감쇠꼬리_재연성_측정결과

## 1. 작업 목적

Stage5 계획서(2-cycle 재연성 검증)에 따라, 카세트 두께별(1.0/0.8/0.6/0.4/0.2mm) 2-cycle 실측 파형을 확보하고 다음 두 가지를 확인한다.

- 두께 변화에 따라 tail 세기/감쇠시간이 어떻게 변하는지
- 1-cycle tail과 2-cycle tail의 차이가 물리적 재연성 문제인지, 잔류 tail 중첩 효과인지

## 2. 수행내용

### 2.1 공통 조건

- 측정 명령: `trfrun`
- 측정 함수: `IDDD_RunCmd_TRF_Real2Cycle()`
- 로그 헤더: `TRF REAL 2-CYCLE (TIM-CNT poll)`
- 채널/전류: `CH4`, `cur=500`
- 샘플링: `50us`, `200 samples` (`0~9950us`)
- 타이밍:
  - ON1=200us, OFF1=1200us
  - ON2=2000us, OFF2=3000us
- 실측 전환시각: `act(us) on1:200 off1:1200 on2:2000 off2:3000` (모든 측정에서 일치)

### 2.2 그래프 저장 경로

- `C:\Users\hansu\Desktop\INCLIX F-1\Measurement\2cycle 재연성`
- 사용 파일:
  - `A기기_CH4_2000us_1_0mm.png`
  - `A기기_CH4_2000us_0_8mm.png`
  - `A기기_CH4_2000us_0_6mm.png`
  - `A기기_CH4_2000us_0_4mm.png`
  - `A기기_CH4_2000us_0_2mm.png`

### 2.3 코드 작업 반영 사항

Stage5 2-cycle 실측을 위해 기존 단발 경로를 보존한 상태에서 신규 2-cycle 경로를 추가/연결하였다.

- `IDDD_RunCmd_TRF_Real2Cycle()` 신규 추가 (`IDDD_RunCmd_TRF_Interface.c`)
  - ON1/OFF1/ON2/OFF2 시퀀스 구현
  - TIM3 CNT 실측 전환시각(`act(us)`) 로그 출력
  - `tim_us < TRF2_LED_ON1_US` 조건으로 dark 평균 산출
- 2-cycle 타이밍 define 추가 (`IDDD_RunCmd_TRF_Interface.c`)
  - `TRF2_LED_ON1_US=200`, `TRF2_LED_OFF1_US=1200`
  - `TRF2_LED_ON2_US=2000`, `TRF2_LED_OFF2_US=3000`
  - 샘플링은 1차 구현 기준으로 `50us`, `200 samples` 유지
- `trfrun` 매핑 변경 (`IDDD_RunCommand_App.c`)
  - `cmdIDDD_TRF_RUN` 호출 대상을 `IDDD_RunCmd_TRF_Real2Cycle()`로 변경
  - 기존 `IDDD_RunCmd_TRF_Real()` 호출은 원복 가능하도록 주석 보존
- 버퍼 정책
  - 신규 버퍼 추가 없이 기존 `g_TRF_Samples` 재사용
  - `IDDD_RunCmd_TRF_Real()` 단발 함수는 삭제하지 않고 보존

### 2.4 두께별 요약 표

아래 값은 그래프 주석/로그 기준의 대표 지점값이다.

| 두께 | dark | 1250us (1cycle tail 시작) | 1950us (ON2 직전 1cycle 잔류) | 3050us (2cycle tail 시작) | 2-1 시작 차이 | plateau 도달시각 | plateau |
|---|---:|---:|---:|---:|---:|---:|---:|
| 1.0mm | 177 | 548 | 321 | 666 | +118 | 8800us | 180 |
| 0.8mm | 178 | 488 | 272 | 570 | +82 | 7800us | 180 |
| 0.6mm | 178 | 429 | 218 | 487 | +58 | 7150us | 180 |
| 0.4mm | 178 | 382 | 189 | 419 | +37 | 6050us | 180 |
| 0.2mm | 177 | 321 | 183 | 335 | +14 | 4100us | 180 |

### 2.5 관찰 사실

1) 두께가 작아질수록 tail 세기가 단조 감소한다.

- 1250us 값이 `548 -> 488 -> 429 -> 382 -> 321`로 감소한다.
- Stage5 계획서에서 수정한 방향(좁아질수록 신호 약화)이 실측으로 확인됐다.

2) 두께가 작아질수록 감쇠 종료가 빨라진다.

- plateau 도달시각이 `8800us -> 4100us`로 단축된다.

3) 2cycle 시작값이 1cycle 시작값보다 높은 이유는, 1cycle 꼬리가 조금 남은 상태에서 2cycle을 시작하기 때문이다.

- 쉽게 말해, 2cycle 값에는 "2cycle 신호 + 1cycle에서 남은 신호"가 함께 들어간다.
- 이 겹침 효과는 두께가 작아질수록 줄어든다.
- 실제로 `2-1 시작 차이`가 `+118 -> +14`로 작아졌다.

4) baseline 특성

- LED를 켜기 전 dark는 약 177~178이다.
- 충분히 시간이 지난 뒤 값은 약 180으로 모인다.
- 즉 끝값이 시작값보다 약 +2~+3 높게 남는 공통 경향이 있다.

5) 포화 상태

- LED ON plateau는 약 3810~3830 수준으로 12bit 최대값 4095 미만이다.
- 현 조건(cur=500)에서는 클리핑이 없으나 마진은 크지 않다.

### 2.6 해석

현재 raw 파형에서 2cycle tail이 1cycle tail보다 높게 보인다고 해서, 바로 "재연성이 나쁘다"고 결론 내리면 안 된다.

2cycle 값에는 아래 두 가지가 같이 섞여 있기 때문이다.

- 2cycle 시작 시점까지 남아 있던 1cycle 꼬리 신호
- 시작 dark와 끝 plateau 사이의 작은 baseline 차이(약 +2~+3)

따라서 공정한 비교를 하려면, 남아 있는 1cycle 꼬리 성분을 계산해서 빼 준 뒤 2cycle을 비교해야 한다.

## 3. 검증 결과

- [x] Stage5 2-cycle 함수 동작은 명령/타이밍/로그 기준으로 정상이다.
- [x] 두께가 작아질수록 signal amplitude와 tail 지속시간이 함께 감소한다.
- [x] 1cycle/2cycle 차이는 두께 감소에 따라 빠르게 축소되며, 0.2mm에서는 거의 근접한다.
- [x] 현재 데이터는 "두께가 작아져서 재연성이 깨졌다"가 아니라, "신호가 약해지고 겹침이 줄어든 결과"로 해석하는 것이 타당하다.

## 4. 잔여 이슈/TODO

- 로그 자동 추출 스크립트에서 `Total values extracted: 0`가 반복 출력되므로 파서 정규식 점검이 필요하다.
- 본 문서 수치는 그래프 주석/수동 확인 기반이며, 후처리 스크립트 보정값과 소수점 수준 차이가 발생할 수 있다.
- 1cycle 잔류 꼬리 보정(계산 후 차감)을 하기 전에는 raw 값만으로 재연성 최종판정을 내리지 않는다.

## 5. 다음 단계

### 5.1 우선 수행 (권장)

- 각 두께에서 1cycle 꼬리(1200~2000us)를 먼저 모델링한다.
- 그 모델로 3000us 이후에도 남아 있을 1cycle 잔류분을 계산한다.
- 계산된 잔류분을 2cycle raw에서 빼서, 순수 2cycle 신호를 만든다.
- 그다음 같은 시간축(OFF 후 0~800us)에서 `2cycle/1cycle 비율`, `기울기`, `면적`을 다시 계산한다.
- 마지막으로 Stage5 계획서 8절 기준(`ratio 0.9~1.1`, `slope ±10%`)으로 최종 판정한다.

### 5.2 후속 실험 (선택)

- 초기 tail 구간 정밀화를 위한 20us 샘플링 재측정(후속 정밀 비교 단계)
- empty 조건 2-cycle 추가 측정으로 baseline drift/offset 범위 재확인
