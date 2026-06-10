# 2026-06-10_TRF_Timing_Fix_Stage1_샘플링타임_검증_결과

## 1. 목적
Stage1 착수에서 확보한 측정 루틴(IDDD_RunCmd_TRF_Meas)을 응용해, 샘플링타임 5단계 스윕(3.5→12.5→39.5→79.5→160.5cycle)을 수행하고 ADC 출력값의 수렴 여부를 관찰했다. 본 문서는 각 단계별 측정 결과와 최종 확정값, 그리고 도출 근거를 기록한다.

## 2. 수행내용
### 2.1 ADC값 통계 출력을 추가했다
[IDDD_RunCmd_TRF_Interface.c](USER/L2_Interface/L2_01_IDDD_RunCMD_Interface/IDDD_RunCmd_TRF_Interface.c)의 `IDDD_RunCmd_TRF_Meas` 함수에 100샘플 버스트 내 ADC값의 avg/min/max를 계산해 최종 로그에 함께 출력하는 라인을 추가했다. 시간 통계 출력은 그대로 유지하고 adc 라인이 한 줄 추가되는 구조다.

### 2.2 샘플링타임 5단계 스윕을 수행했다
[IDDD_ADC_SCAN_Drv.c](USER/L1_Drive/L1_01_Component/L1_01_01_IDDD_TRF_Drv/IDDD_ADC_SCAN_Drv.c)의 `IDDD_PD_ADC_Config_Stage1Meas` 함수 내 SamplingTimeCommon1을 순차 변경하며 각 단계에서 trfrun 10회 이상 실행해 시간 통계와 ADC값 통계를 동시 기록했다.

### 2.3 측정 결과를 비교하고 수렴 지점을 확정했다
5단계 전체 결과는 다음과 같다.

| 단계 | sampling | time avg | time max | adc avg | adc min | adc max | 변화폭 |
|------|----------|----------|----------|---------|---------|---------|--------|
| 0 | 3.5cycle | 44us | 48us | 191 | 156 | 196 | - |
| 1 | 12.5cycle | 60us | 64us | 166 | 147 | 170 | -25 |
| 2 | 39.5cycle | 108us | 113us | 144 | 133 | 148 | -22 |
| 3 | 79.5cycle | 180us | 184us | 138 | 128 | 142 | -6 |
| 4 | 160.5cycle | 324us | 329us | 137 | 125 | 141 | -1 |

79.5cycle과 160.5cycle 간 adc avg 차이가 1로 수렴했고, 160.5cycle은 HAL에서 지원하는 최대 샘플링타임이다. 247.5cycle 이상은 미지원으로 확인되었다.

### 2.4 최종 샘플링타임을 79.5cycle로 확정했다
160.5cycle과 ADC값이 실질적으로 동일하면서(138 vs 137), 버스트 시간은 180us로 160.5cycle(324us) 대비 절반 수준이다. 700us adc_window 대비 25.7%만 사용하므로 여유도 충분하다. 3.5cycle(191) 대비 ADC값이 53카운트 차이 나므로, 기존 3.5cycle은 샘플링 부족으로 사용 불가 판정이다.

## 3. 다음 단계
- Stage1 통합 단계로 이관: 임시 매핑 해제, Stage0 경로에 79.5cycle 및 확정된 정책값 반영
- CubeMX에 최종 ADC 설정 반영 후 코드 재생성 및 회귀 검증
