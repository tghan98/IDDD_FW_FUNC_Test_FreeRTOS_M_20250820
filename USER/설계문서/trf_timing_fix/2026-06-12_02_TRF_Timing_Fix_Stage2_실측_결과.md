# 2026-06-12_02_TRF_Timing_Fix_Stage2_실측_결과

## 1. 작업 목적

2026-06-12_01_TRF_Timing_Fix_Stage2_실측_계획에 따른 TRF 형광감쇠곡선 TIM CNT 폴링 측정 구현 및 실측 검증 결과를 정리한다.

## 2. 수행내용

### 2.1 IDDD_PD_ADC_Config_Stage1Poll() 신규 추가

`IDDD_ADC_SCAN_Drv.c`에 폴링 측정 전용 ADC 설정 함수 `IDDD_PD_ADC_Config_Stage1Poll()`을 신규 추가하였다. Single Conversion, SW Start, DMA 비활성, 샘플타임 79.5cycle로 구성하며, 구현 패턴은 동일 파일 내 `IDDD_BAT_ADC_ReadLowFunc`가 사용하는 `HAL_ADC_Start → PollForConversion → GetValue → Stop` 흐름을 따른다. TRF_Real()에서 직접 ADC 핸들을 얻을 수 있도록 `IDDD_PD_ADC_GetHandle()` 접근자도 함께 추가하였다. 헤더 `IDDD_ADC_SCAN_Drv.h`에 두 함수 모두 선언하였다. 기존 `IDDD_PD_ADC_Config_Stage1Meas()`는 한 줄도 변경하지 않고 보존하였다.

### 2.2 IDDD_RunCmd_TRF_Real() 폴링 방식 재작성

`IDDD_RunCmd_TRF_Real()`을 DMA 연속측정에서 TIM3 CNT 폴링 방식으로 전면 재작성하였다. 기존 `g_wTRF_MeasBuf[1100]`를 폐기하고 `TRF_Sample_t g_TRF_Samples[100]` (400 bytes)로 교체하였다. `g_wTRF_MeasBuf`는 `IDDD_RunCmd_TRF_Meas()` 전용으로 `TRF_MEAS_SAMPLE_COUNT(100)` 크기로 분리 재선언하였다.

함수 흐름은 ADC Lock → Config_Stage1Poll → TIM3 free counter 시작 → 100회 폴링 루프로 구성된다. 루프 내에서 매회 `i * 20µs` 목표 시각까지 TIM3 CNT를 폴링 대기한 후 실제 CNT를 타임스탬프로 기록하고, `HAL_ADC_Start → PollForConversion → GetValue → Stop` 패턴으로 ADC 단발 변환을 수행한다.

LED 제어는 루프 내 TIM3 CNT 조건 분기로 구현하였다. CNT ≥ 200µs에서 `IDDD_LED_Current_Set(500)` → `IDDD_LED_Channel_Select(OPT_LED_CH1)` → `PA6 HIGH` 순으로 ON, CNT ≥ 1200µs에서 `TRF_SequenceSignal_SetLow()` → `IDDD_LED_Channel_Select(OPT_LED_CH_OFF)` 순으로 OFF 한다. `bLedOn` 플래그로 중복 동작을 방지한다.

측정 완료 후 첫 10샘플로 dark 평균을 계산하고, 100개 샘플을 `[t=XXXus]YYYY` 형식으로 UART 일괄 출력한다.

변경된 define은 다음과 같다:
- `TRF_REAL_SAMPLE_COUNT 100U`: 총 샘플 수
- `TRF_REAL_SAMPLE_INTERVAL_US 20U`: 균일 간격
- `TRF_REAL_LED_ON_US 200U`: LED ON 임계
- `TRF_REAL_LED_OFF_US 1200U`: LED OFF 임계
- `TRF_REAL_DARK_COUNT 10U`: dark 평균용 선두 샘플 수
- `TRF_REAL_LED_CURRENT 500U`: LED 구동 전류

### 2.3 빌드 결과

IAR EWARM 9.0 빌드: errors=0, warnings=0. 바이너리 `IDDD_FW.out` 및 `IDDD_FW.bin` 정상 생성. Stage1Meas 경로(`IDDD_RunCmd_TRF_Meas`, `Config_Stage1Meas`) 및 Stage0 경로(`CmdRun_TRF_Run`, `IDDD_RunCmd_SyncTRF_Run`)는 변경 없음.

## 3. 검증 결과

- [x] 빌드: IAR errors=0, warnings=0
- [ ] `trfrun` 실행 — 100개 샘플 UART 출력 정상
- [ ] 타임스탬프 검증: 0, 20, 40, ..., 1980µs
- [ ] dark 평균값
- [ ] LED ON 구간 동작 확인
- [ ] 감쇠구간 진입 확인
- [ ] Stage0 회귀: 기존 명령 정상

## 4. 잔여 이슈/TODO

- 실물 타깃에 플래시 writing 후 `trfrun`으로 UART 출력 확보 필요
- ADC 단발 변환(HAL_ADC_Start/Poll/Stop)의 실제 소요시간이 20µs 이내인지 실측 확인
- 폴링 루프 내 while(CNT < target) 스핀이 타임스탬프 정밀도에 미치는 영향 검증

## 5. 다음 단계

- 실물 STM32U031에 펌웨어 writing 후 `trfrun` 실행
- UART 로그에서 100개 샘플의 `[t=XXXus]YYYY` 출력 확인
- dark 평균값(약 137), LED ON 구간 상승, 감쇠구간 하강 패턴 확인
- 필요시 샘플 간격/수 조정
