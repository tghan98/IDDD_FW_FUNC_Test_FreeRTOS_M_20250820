# 2026-07-09_TRF_Timing_Fix_Stage5_2cycle_ON2타이밍스윕_계획

## 1. 작업 목적

본 계획의 목적은 Stage5 2-cycle 측정에서 ON2 시점을 가변 스윕하여, 1cycle 시작값과 2cycle 시작값의 차이를 최소화하는 제품 적용 타이밍을 선정하는 것이다.

기존 Stage5 측정에서는 ON2=2000us 고정 조건에서 두께별 경향과 중첩 특성을 확인하였다. 본 문서는 해당 결과를 기반으로, 실제 제품에 적용 가능한 단일 ON2 타이밍(권장값 1개, 백업 1개)을 도출하기 위한 실행 계획을 정리한다.

## 2. 수행내용

### 2.1 시험 설계

시험 설계의 기본 원칙은 ON2 시점만 변화시키고, 그 외 조건은 고정하여 ON2 변화의 영향만 분리 관찰하는 것이다.

고정 조건은 다음과 같다.

- 채널: CH4
- LED current: cur=500
- 샘플링: 50us, 200 samples (0~9950us)
- ON1=200us, OFF1=1200us
- 2차 LED ON 폭: 1000us 고정

가변 조건은 다음과 같다.

- ON2 후보: 2000us, 3000us, 4000us, 5000us
- OFF2 = ON2 + 1000us

측정 대상 두께는 1.0mm, 0.8mm, 0.6mm, 0.4mm, 0.2mm로 고정한다.

### 2.2 측정 매트릭스

측정은 두께 x ON2 매트릭스로 수행한다. 각 ON2 후보에 대해 전 두께 조건을 동일 순서로 측정하고, 동일 로그 포맷으로 저장한다.

그래프/로그 저장 경로는 기존 Stage5와 동일한 운영 경로를 사용한다.

- `C:\Users\hansu\Desktop\INCLIX F-1\Measurement\2cycle 재연성`

### 2.3 평가 지표

본 계획에서는 시작값 차이 최소화를 1차 목표로 둔다.

1순위 지표는 다음과 같다.

- DeltaStart = ADC(2cycle 시작값) - ADC(1cycle 시작값)

2순위 지표는 다음과 같다.

- DeltaNorm = DeltaStart / (ADC(1cycle 시작값) - dark)

DeltaNorm은 두께별 절대 신호 크기 차이를 보정하기 위한 지표로 사용하며, 동률 또는 근접 후보 비교 시 우선 적용한다.

### 2.4 판정 및 선정 규칙

최종 타이밍 선정은 아래 순서로 진행한다.

- 1순위: 1.0mm 조건에서 DeltaStart가 가장 작은 ON2를 우선 후보로 선정
- 2순위: 전체 두께(1.0~0.2mm)에서 DeltaNorm 편차가 가장 작은 ON2를 우선
- 최종 산출: 추천 ON2 1개 + 백업 ON2 1개

선정 결과는 수치표와 함께 근거 문장(왜 해당 ON2를 선택했는지)을 같이 남긴다.

### 2.5 예상 코드 변경사항

ON2 타이밍 스윕 시 코드 변경은 TRF 2-cycle 타이밍 define 구간으로 한정한다.

- 변경 대상 파일: `USER/L2_Interface/L2_01_IDDD_RunCMD_Interface/IDDD_RunCmd_TRF_Interface.c`
- 변경 항목: `TRF2_LED_ON2_US`, `TRF2_LED_OFF2_US`
- 변경 규칙: `TRF2_LED_OFF2_US = TRF2_LED_ON2_US + 1000us` 유지

명령 매핑(`trfrun`)과 함수 선언은 현재 구조를 유지하며 추가 변경하지 않는다. 또한 ON2 조건별 비교 일관성을 위해 로그 포맷(`nom/act us`)은 동일하게 유지한다.

## 3. 검증 계획

- [ ] ON2별(2000/3000/4000/5000) 측정 로그 확보
- [ ] 두께별 DeltaStart 계산 완료
- [ ] 두께별 DeltaNorm 계산 완료
- [ ] 추천 ON2 1개 선정
- [ ] 백업 ON2 1개 선정

## 4. 잔여 이슈/TODO

- 로그 자동 추출 스크립트의 `Total values extracted: 0` 파싱 이슈 우선 수정
- ON2=5000us 조건에서 후반부 샘플 수가 충분한지 확인

## 5. 다음 단계

- ON2 후보 4개에 대해 두께별 실측 수행
- 결과 요약표(두께 x ON2, DeltaStart/DeltaNorm) 작성
- 선정 규칙에 따라 제품 적용 ON2 최종 후보 확정
