# Todo List — TRF Timing Fix 통합 정리

> 현재 매핑: `trfrun` → `IDDD_RunCmd_TRF_Real2Cycle_Gap100ms` (Stage6) · 다음 전환: Stage5 ON2 스윕 함수

## 0. trfrun 매핑 상태

| 실험 모드 | 매핑 함수 | 로그 태그 | 원복 주석 |
|---|---|---|---|
| Stage5 연속 | `IDDD_RunCmd_TRF_Real2Cycle` | `2-CYCLE` | 유지 |
| Stage6 분리(현재) | `IDDD_RunCmd_TRF_Real2Cycle_Gap100ms` | `GAP100MS` | 유지 |
| Stage5 ON2 스윕(예정) | (스윕 함수) | `SWEEP` | 예정 |

## 1. 현재 상태 (Stage 진행 요약)

| Stage | 핵심 작업 | 결론/확정값 | 상태 |
|---|---|---|---|
| Stage0 | PWM 의존 제거, SW GPIO 5구간 시퀀스 전환 | `TRF_RunOneSequenceCycle` 골격 확보(이후 폴링 방식으로 대체) | 보존(미사용) |
| Stage1 착수 | `trfrun`으로 100샘플 버스트 시간측정 루틴 추가 | `IDDD_RunCmd_TRF_Meas` + TIM3 1us 카운터 확보 | 완료 |
| Stage1 샘플링 | 샘플링타임 5단계 스윕(3.5~160.5cycle) | 79.5cycle 확정(160.5와 ADC 1차이, 시간 절반) | 완료 |
| Stage2 실측 | 폴링 단발 실측(`Real`)로 전면 재작성 | `Config_Stage1Poll`, `g_TRF_Samples`, ON200/OFF1200 확립 | 완료(실사용 기반) |
| Stage3 | 측정창 10ms 확장, 3-way(빈/1.0mm/direct) 비교 | 50us×200 확장, dark 기준 재정의(≈135) | 완료 |
| Stage4 | 기기별 튜닝 측정 | - | 보류 |
| Stage5 | 2-cycle 감쇠꼬리 재연성 + ON2 타이밍 스윕 | 두께별 tail 경향 확보, ON2 스윕 마무리 예정 | 진행 중 |
| Stage6 | 120ms gap 분리측정 재연성 검증 | PH1/PH2 지표 거의 동일, 잔류중첩 단독원인 가설 약화 | 완료 |

현재 `trfrun`은 Stage6 분리측정 함수에 매핑되어 있고, 다음 작업은 Stage5 ON2 스윕으로 매핑 전환이다.

## 2. 할 일

1. **Stage5 ON2 스윕 재개**
   - `cmdIDDD_TRF_RUN` 매핑을 Stage6 함수 → ON2 스윕 함수로 교체
   - Stage6 호출은 원복용 주석으로 보존, 로그 헤더에 `SWEEP` 태그 추가
2. **스윕 실행**
   - 1차 성긴 스윕: ON2 = 2000/3000/4000/5000/6000us
   - 2차 정밀 스윕: 최적 후보 ±500us
   - 조건 고정(전류, 샘플링, ON/OFF 폭, gap)
3. **결과 정리**
   - 지표: ON 평균, OFF+50us, OFF+150us, Dark, Run간 표준편차
   - 최적 1안 + 백업 2안 선정, 미선정 사유 명시
4. **후속 정리 (낮은 우선순위)**
   - Stage0 잔여 코드(`CmdRun_TRF_Run`, `IDDD_RunCmd_SyncTRF_Run`, PWM 함수, `g_wADC_Buf`) 정리
   - CubeMX 최종 반영(`.ioc` → `MX_ADC1_Init` 재생성, PB11 모드 검토) 및 회귀 검증
   - 실험 종료 후 `trfrun` 매핑 원복

## 3. 주의

- 회로 변경 불필요 (PA7→PB11 루프백 무시하면 됨)
- `Config_Stage1Meas`는 절대 삭제 금지 (DMA 통계 측정 경로)
- `IDDD_PD_ADC_GetHandle()`은 통합 후에도 계속 필요
- 실험 전환 시 반드시 매핑표/로그 태그 갱신 후 빌드

## 4. 과거 이력 (Stage0~2 상세)

| 경로 | ADC 트리거 | 용도 |
|---|---|---|
| **Stage0 원본** | EXTI11 외부 트리거 (PA7→PB11 루프백) | 보존만 됨, 호출 안 함 |
| **Stage1 Meas** | SOFTWARE_START + 연속변환 + DMA | DMA 버스트 시간 측정 (`trfmeas`) |
| **Stage2 Real** | SOFTWARE_START + 단발변환 + 폴링 | TIM3 CNT 폴링 실측 (**실사용 중**) |

기존 정리 할 일(현재 대부분 완료/보류로 재판정):
1. ADC SOFTWARE_START 통일 — 부분완료·보류 (실측 경로는 이미 `Config_Stage1Poll` 사용, 기본 트리거 정리만 잔여)
2. 5구간 시퀀스 Start/Poll/Stop 전환 — 사실상 완료 (`Real`/`Real2Cycle` 계열 적용, 구형 `TRF_RunOneSequenceCycle`만 잔존)
3. Stage0 경로 정리 — 유지(보류)
4. CubeMX 동기화 — 유지(보류)
5. Real/통합 시퀀스 병존 결정 — 완료 (`Real` 계열 실사용 확정)
