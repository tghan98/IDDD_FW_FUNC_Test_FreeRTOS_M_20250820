# Todo List — TRF Timing Fix 통합 정리

## 1. 현재 상태

| 경로 | ADC 트리거 | 용도 |
|---|---|---|
| **Stage0 원본** | EXTI11 외부 트리거 (PA7→PB11 루프백) | 보존만 됨, 호출 안 함 |
| **Stage1 Meas** | SOFTWARE_START + 연속변환 + DMA | DMA 버스트 시간 측정 (`trfmeas`) |
| **Stage2 Real** | SOFTWARE_START + 단발변환 + 폴링 | TIM3 CNT 폴링 실측 (**실사용 중**) |

Stage0은 코드만 남아 있고, `trfrun`은 Stage2 Real로 연결되어 있다.

## 2. 할 일

1. **ADC 설정 SOFTWARE_START로 통일**
   - `IDDD_PD_ADC_Config()`의 트리거를 EXT_IT11 → SOFTWARE_START 로 변경하고, Stage1에서 확정된 79.5cycle 샘플링타임 적용
   - `Config_Stage1Poll()`은 Config와 동일해지므로 정리
   - `Config_Stage1Meas()`는 DMA 통계용으로 그대로 유지

2. **5구간 시퀀스를 Start/Poll/Stop 방식으로 전환**
   - `TRF_RunOneSequenceCycle()`에서 PA7 토글(외부 트리거) → Completion Flag 폴링 → DMA 읽기 대신, `HAL_ADC_Start/PollForConversion/GetValue/Stop` 직접 호출
   - 이에 따라 Completion Flag(`g_dwIDDD_PD_ADC_CMP_Flag`) 및 `Read_IDDD_PD_ADC_Complete_Flag()` 의존 제거

3. **Stage0 경로 정리**
   - `CmdRun_TRF_Run` / `IDDD_RunCmd_SyncTRF_Run` 폐기 (또는 `#if 0` 보존)
   - 호출자가 사라지는 PWM 함수들(`TImer_Config`, `PWM_Start/Stop/Channel_Enanle`) 정리
   - `g_wADC_Buf[15]` 등 Stage0 전용 전역 변수 제거

4. **CubeMX 동기화**
   - `.ioc` 업데이트 후 `MX_ADC1_Init` 재생성 (SOFTWARE_START + 79.5cycle)
   - PB11 GPIO 모드 검토 (EXTI11 → INPUT/ANALOG)

5. **Stage2 Real과 통합 시퀀스 병존 방안 결정**
   - Real은 200샘플 장시간 측정(Stage3 확장), 통합 시퀀스는 단발 — 용도가 다르므로 병존 또는 통합 판단

## 3. 주의

- 회로 변경 불필요 (PA7→PB11 루프백 무시하면 됨)
- `Config_Stage1Meas`는 절대 삭제 금지 (DMA 통계 측정 경로)
- `IDDD_PD_ADC_GetHandle()`은 통합 후에도 계속 필요
