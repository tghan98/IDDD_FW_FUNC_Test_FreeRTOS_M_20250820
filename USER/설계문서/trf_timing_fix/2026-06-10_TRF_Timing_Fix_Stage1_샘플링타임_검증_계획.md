# 2026-06-10_TRF_Timing_Fix_Stage1_샘플링타임_검증_계획

## 1. 목적
Stage1 착수에서 확보한 측정 루틴(IDDD_RunCmd_TRF_Meas)을 그대로 응용해, 현재 샘플링타임(ADC_SAMPLETIME_3CYCLES_5)이 ADC값 정확도에 충분한지 검증한다. 샘플링타임을 단계적으로 올리며 ADC 출력값과 버스트 시간의 변화를 관찰하고, 변화가 없거나 수렴하면 현재 설정을 최종 확정한다.

## 2. 문서 사용 가이드(타 AI 에이전트 공용)
이 문서는 Stage1 착수 결과(2026-06-09_TRF_Timing_Fix_Stage1_착수_결과)에서 이어지는 검증 단계로, 통합 전 최종 확인을 목적으로 한다.

핵심 의도는 다음과 같다.
- Stage1 측정 코드를 수정 없이 최대한 재활용한다.
- ADC값 통계 출력만 최소 추가하고, 나머지 구조는 그대로 둔다.
- Stage0 운용 경로는 일절 건드리지 않는다.

## 3. 현재 기준선
2026-06-09 착수 결과 기준 확정 상태는 다음과 같다.
- ADC 설정: SW Start, ContinuousConvMode ENABLE, DMAContinuousRequests ENABLE
- 샘플링타임: ADC_SAMPLETIME_3CYCLES_5 (약 3.5 ADC clock cycle)
- 100샘플 버스트 시간: avg 44us, min 43us, max 48us (100회 반복 기준)
- timeout/error: 0
- 현재 출력: 시간 통계만 (avg/min/max/count)

## 4. 검증 범위
### 4.1 포함 범위
- ADC값 통계(avg/min/max) 출력 1줄 추가
- 샘플링타임 단계 변경: 3.5cycle → 한두 단계 상향
- 각 단계에서 시간 통계와 ADC값 통계를 동시 비교
- 판정: ADC값 변화가 ±노이즈 수준이면 현재 설정 유지, 유의미한 변화가 있으면 상향 검토

### 4.2 제외 범위
- Stage0 경로 수정
- DMA 길이/반복 횟수 변경
- ADC 클럭 분주비 변경
- trfrun 문자열 변경
- CubeMX 재생성

## 5. 파라미터 및 판정 정책
### 5.1 변경 대상 파라미터
- IDDD_PD_ADC_Config_Stage1Meas 함수 내 SamplingTimeCommon1

### 5.2 스윕 단계 (ST RPN 권고값 기준)
- 단계 0 (현재): ADC_SAMPLETIME_3CYCLES_5
- 단계 1: ADC_SAMPLETIME_12CYCLES_5
- 단계 2: ADC_SAMPLETIME_39CYCLES_5

### 5.3 판정 기준
- ADC값 avg/min/max가 단계 간 ±5 이내 변화면 "현재 설정 충분" 판정
- max 버스트 시간이 700us(adc_window) 대비 20% 이내면 무조건 통과
- timeout 0 유지

## 6. 작업 단계
1. IDDD_RunCmd_TRF_Meas 최종 출력부에 ADC값 avg/min/max 라인 1줄 추가
2. 빌드 및 trfrun 1회 실행으로 출력 포맷 확인
3. IDDD_PD_ADC_Config_Stage1Meas에서 SamplingTimeCommon1을 단계 1로 변경
4. 빌드 및 측정 → 시간 + ADC값 기록
5. 단계 2로 변경 후 동일 측정
6. 3단계 결과 비교 → 최종 샘플링타임 확정
7. 확정값으로 코드 원복 (또는 그대로 유지)

## 7. 파일별 역할(책임 경계)
- IDDD_RunCmd_TRF_Interface.c: ADC값 통계 계산 및 출력 추가 (최소 수정)
- IDDD_ADC_SCAN_Drv.c: 샘플링타임 상수 변경 (IDDD_PD_ADC_Config_Stage1Meas)
- 그 외 파일: 수정 없음

## 8. 검증 항목
1. 각 단계에서 timeout 0, count 100이어야 한다.
2. ADC값 avg/min/max가 단계 간 기록되어 비교 가능해야 한다.
3. max 버스트 시간이 adc_window(700us) 이내여야 한다.
4. Stage0 경로 회귀가 없어야 한다.

## 9. 완료 기준
- 샘플링타임 3단계 측정 완료 및 ADC값 추이 기록
- "현재 설정 유지" 또는 "N단계로 상향" 결정
- 결정값으로 코드 확정

## 10. 다음 단계
샘플링타임 확정 후 통합 단계로 이관한다.
통합 단계: 임시 매핑 해제, Stage0 경로에 정책값 반영, CubeMX 최종 설정.
