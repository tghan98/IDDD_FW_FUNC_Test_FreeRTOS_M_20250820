# 2026-06-12_01_TRF_Timing_Fix_Stage2_실측_계획

## 1. 작업 목적

TRF 형광감쇠곡선(0~2000µs) 실측 메커니즘을 Stage2로 이관하여 재정비한다. Stage1에서 DMA 연속측정(79.5cycle SW Start Continuous) 검증은 완료되었으나, 전 구간 raw 캡처 목표가 미달되었고 Stage1 문서가 6개까지 과다 축적되었다. Stage2에서는 Stage1Meas(79.5cycle DMA) 설정을 보존하고, 별도로 TIM3 CNT 폴링 기반의 러프한 단발 ADC 측정 방식으로 0~2000µs 전 구간 raw 값을 확보하는 것이 목표이다. 측정 방식은 연속 DMA가 아닌 정해진 시점마다 ADC 단발 변환을 수행하는 TIM CNT 폴링으로 단순화한다.

## 2. 수행내용

### 2.1 Stage1Meas(79.5cycle DMA) 설정 보존

기존 `IDDD_PD_ADC_Config_Stage1Meas()` 함수는 향후 Stage0 5구간 사이클에 반영될 최종 ADC 프로파일이므로 한 줄도 변경하지 않고 그대로 보존한다. 이 함수는 ADC_SAMPLETIME_79CYCLES_5, SW Start, ContinuousConvMode=ENABLE, DMAContinuousRequests=ENABLE 설정을 유지하며, 현재 `IDDD_RunCmd_TRF_Meas()`에서 사용 중인 경로도 그대로 둔다. Stage2 폴링 측정은 이 설정과 완전히 분리된 별도 경로로 구현한다.

### 2.2 Stage1Poll(단발 폴링) ADC 설정 신규 추가

폴링 측정 전용으로 `IDDD_PD_ADC_Config_Stage1Poll()` 함수를 신규 작성한다. 파일은 `IDDD_ADC_SCAN_Drv.c`에 추가하고 헤더 `IDDD_ADC_SCAN_Drv.h`에 선언한다. 설정은 Single Conversion, SW Start, DMA 비활성, 샘플타임 79.5cycle로 구성한다. 구현 패턴은 동일 코드베이스 내 `IDDD_BAT_ADC_Drv.c`의 `IDDD_BAT_ADC_Config` + `IDDD_BAT_ADC_ReadLowFunc`를 참조한다.

### 2.3 TRF_Real() 함수 전면 개편

`IDDD_RunCmd_TRF_Real()`을 DMA 연속측정 방식에서 TIM3 CNT 폴링 방식으로 재작성한다. 기존 `g_wTRF_MeasBuf[1100]` 전역 버퍼를 폐기하고 `{uint16_t tim; uint16_t val;}` 구조체 100개 배열(`g_TRF_Samples[100]`, 400 bytes)로 교체한다.

함수 흐름은 다음과 같다. ADC Lock 및 `Config_Stage1Poll()` 호출로 단발 측정 모드로 진입한 후, TIM3를 1µs free counter로 시작한다. 100회 루프에서 매회 목표 시각(`i * 20µs`)까지 TIM3 CNT를 폴링 대기한 뒤, 실제 CNT를 타임스탬프로 저장하고 ADC 단발 변환(HAL_ADC_Start → PollForConversion → GetValue → Stop)을 수행해 값을 기록한다.

LED 제어는 루프 내에서 TIM3 CNT 조건 분기로 구현한다. CNT 200µs 도달 시 `IDDD_LED_Current_Set(500)` → `IDDD_LED_Channel_Select(OPT_LED_CH1)` → `PA6 HIGH` 순으로 LED를 켜고, CNT 1200µs 도달 시 `TRF_SequenceSignal_SetLow()` → `IDDD_LED_Channel_Select(OPT_LED_CH_OFF)` 순으로 끈다. LED 제어 순서는 Stage1에서 검증된 Current→Channel→GPIO 규칙을 준수한다.

측정 완료 후 100개 샘플을 `[t=XXXus]YYYY` 형식으로 UART 일괄 출력한다. 첫 10샘플(0~190µs)은 암전류 구간으로 dark 평균 계산에 사용한다.

### 2.4 샘플링 전략

세 구간(암구간 0~200µs, LED ON 200~1200µs, 감쇠구간 1200~2000µs)을 균일 20µs 간격으로 100샘플 측정한다. 배분은 암구간 10샘플(인덱스 0~9), LED ON 50샘플(인덱스 10~59), 감쇠구간 40샘플(인덱스 60~99)이다. ADC 클럭은 SYSCLK 56MHz(RCC_ADCCLKSOURCE_SYSCLK), 1회 변환 시간은 약 1.64µs로 샘플 간격 20µs 내에 충분히 완료된다.

## 3. 검증 결과

- [ ] 빌드: IAR errors=0, warnings=0
- [ ] `trfrun` 실행 시 100개 샘플 UART 출력 정상
- [ ] 타임스탬프가 0, 20, 40, ..., 1980µs ± 수µs 범위 내
- [ ] dark 평균값이 이전 검증치(~137)와 유사 (LED OFF 상태)
- [ ] LED ON 구간(200~1200µs) 값 상승 확인
- [ ] 감쇠구간(1200~2000µs) 값 하강 추세 확인
- [ ] Stage0 회귀: trfrun/trftim/ledpw/optsel 기존 명령 정상 동작

## 4. 잔여 이슈/TODO

- ADC 단발 변환(HAL_ADC_Start/Poll/Stop) 한 사이클의 실제 소요시간이 20µs 샘플 간격 내에 수용되는지 실측 확인 필요
- 폴링 루프 내 while(CNT < target) 스핀이 타임스탬프 정밀도에 미치는 영향 검증

## 5. 다음 단계

- `IDDD_ADC_SCAN_Drv.c`에 `IDDD_PD_ADC_Config_Stage1Poll()` 추가 및 헤더 선언
- `IDDD_RunCmd_TRF_Real()` 재작성 (폴링 루프 + LED 제어 + 출력)
- IAR 빌드 및 Stage0 회귀 확인
- `trfrun` 실행으로 100샘플 데이터 확보
- 감쇠곡선 데이터 검토 후 필요시 간격/샘플 수 조정
