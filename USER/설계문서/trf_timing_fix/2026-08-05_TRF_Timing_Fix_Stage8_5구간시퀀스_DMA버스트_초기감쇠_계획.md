# 2026-08-05_TRF_Timing_Fix_Stage8_5구간시퀀스_DMA버스트_초기감쇠_계획

## 1. 작업 목적

본 계획의 목적은 Stage0에서 합의한 5구간 시퀀스 골격에, Stage1에서 확정한 측정 자산(SW Start + DMA 100샘플 버스트, 79.5cycle 샘플링타임)을 얹어 최종 측정 형태를 구성하는 것이다.

측정 대상은 LED OFF 시점 기준 +150us부터 시작되는 초기 형광 감쇠 구간이며, 100샘플 DMA 버스트로 약 180us 구간을 고해상도(약 1.8us/샘플)로 촘촘하게 확보하는 것을 목표로 한다.

이번 단계는 긴 tail(약 150ms까지 이어지는 후반 감쇠)을 관찰 범위에 포함하지 않는다. 관찰 범위는 OFF+150us부터 OFF+330us까지의 초기 감쇠 구간으로 한정하며, 이는 DMA 연속 버스트가 ADC 최대속도로 한 번에 채집되는 특성에 근거한 의도적 범위 설정이다.

암흑 차분은 함수 내부에서 수행하지 않는다. dark 카세트를 꽂고 `trfrun`으로 본 시퀀스를 1회 실행하고, 이어서 기준 카세트를 새로 꽂아 `trfrun`으로 본 시퀀스를 다시 1회 실행한 뒤, 두 실행 결과를 오프라인에서 차분하는 방식으로 dark 기준을 확보한다.

## 2. 수행내용

구현 순서는 다음과 같다. 1) 타이밍 프로파일 상수 갱신 및 검증식 정합 → 2) 신규 버스트 측정 함수 추가 → 3) 버스트 시각 복원 및 출력 포맷 구성 → 4) `trfrun` 매핑 교체 및 원복 주석/로그 태그 → 5) 헤더 프로토타입 추가.

### 2.1 타이밍 프로파일 상수 갱신

기존 5구간 프로파일 상수를 이번 실험 조건으로 갱신한다. LED ON은 1000us에서 4000us로, Residual wait는 200us에서 150us로, ADC window는 700us에서 약 200us(100샘플 버스트 실측 약 180us에 여유 포함)로, 전체 Cycle은 2000us에서 5000us로 조정한다. Stabilize는 100us를 유지한다.

이와 함께 `TRF_SequenceTimingProfile_Validate()`의 판정 기준을 갱신된 프로파일과 정합시킨다. 활성 구간 합(4000+150+200+100 = 4450us)이 Cycle(5000us)을 넘지 않는지 확인하고, 여유 시간 550us가 확보되는지 확인한다. 또한 전체 Cycle이 측정 타이머 한 바퀴(1us tick 기준 0xFFFF, 즉 65535us) 안에 들어오는지 확인한다. 기존 검증식의 9999us 상한 표기는 구형 TRF3 프로파일 기준이므로 측정 타이머(MeasTimer) 기준과 혼동되지 않도록 정리한다.

### 2.2 신규 버스트 측정 함수 추가

신규 함수 `IDDD_RunCmd_TRF_Real_Burst`(가칭)를 추가한다. 기존 단발 폴링 함수 `IDDD_RunCmd_TRF_Real()`은 삭제하지 않고 보존하여 이상 발생 시 원복과 비교가 가능하도록 한다.

ADC 설정은 단발 폴링용 `Config_Stage1Poll`이 아니라, SW Start + 연속 변환 + DMA 구조인 `Config_Stage1Meas`를 사용한다. 버스트 버퍼는 기존 `g_wTRF_MeasBuf[100]`을 재사용한다.

LED 제어는 Stage2 버그수정에서 확정된 active LOW 극성(PA6 RESET = 점등, PA6 SET = 소등)을 따른다. 전류와 채널은 기존 `Real()` 계열과 동일하게 optsel 채널 및 ROM 전류를 읽어(실패 시 상수 fallback) `IDDD_LED_Current_Set` 이후 `IDDD_LED_Channel_Select` 순서로 설정한다.

구간 전이는 구형 `TRF_SequenceWaitUntilUs`의 ADC 완료플래그 결합 방식을 사용하지 않고, `Real()`과 동일한 `while(TIM3->CNT < target)` 단순 대기 방식으로 처리한다. LED ON은 t=0, LED OFF는 t=4000us, 버스트 시작은 t=4150us(OFF+150us) 시점에 배치한다.

### 2.3 버스트 시각 복원 및 출력 포맷

DMA 연속 버스트는 샘플별 개별 타임스탬프가 없으므로, `IDDD_RunCmd_TRF_Meas()`에서 사용하는 방식과 동일하게 버스트 시작 CNT와 종료 CNT를 캡처한다. 실측 버스트 총시간을 99로 나눠 per-sample 간격을 산출하고, `버스트 시작 시각 + 샘플 index × per-sample 간격`으로 각 샘플의 시간축을 복원한다.

버스트 완료 검출과 종료 CNT 캡처는 신규 인터럽트를 추가하지 않고, 기존 Meas 모드의 DMA 전송완료(TC) 경로인 `HAL_ADC_ConvCpltCallback`을 그대로 재사용한다. `Config_Stage1Meas`로 100샘플 버스트를 시작하면 버퍼가 다 채워지는 순간 DMA TC 인터럽트가 이 콜백을 호출하며, 콜백은 ISR 안에서 `TIM3->CNT`를 종료 CNT(`g_dwTRF_Meas_End_Cnt`)로 래치하고 완료 플래그(`g_dwTRF_Meas_Done_Flag`)를 세운다. 폴링이나 고정시간 대기 방식은 완료 순간과 검출 사이의 소프트웨어 지연으로 종료 CNT 정밀도가 떨어지므로 채택하지 않는다. 따라서 신규 함수는 `IDDD_TRF_Meas_Mode_CTRL(SET)` → 완료 플래그 RESET → DMA start → 완료 플래그 폴링 → `Read_IDDD_TRF_Meas_End_Cnt()` 순서로 종료 시각을 확보한다.

출력은 헤더 한 줄(채널, 설정 전류, 버스트 시작 시각, 실측 버스트 총시간, per-sample 간격)에 이어 원시 100샘플 값을 연속 출력하는 구조로 구성한다. 함수 내부에서 dark 평균이나 차분은 계산하지 않는다.

GUI 파서는 수정할 수 없으므로, TRF burst 원시값 출력은 일반 `hsDebug_MSG()` 경로와 분리한다. 기존 debug 출력 경로는 문자 단위 TX 큐와 CLI TX task의 DMA 배출 구조를 사용하므로, 출력량이 많을 때 tail 전송이 늘어져 GUI 파서의 프레임/침묵 타임아웃이 먼저 발동할 수 있다. 따라서 Stage8 burst 로그 구간만 `hsDEBUG_UART_TX_BlockingStart()`로 기존 TX queue empty 및 UART READY 상태를 확인한 뒤, 128바이트 라인 버퍼에 `snprintf()`로 한 줄씩 생성하고 `hsDEBUG_UART_TX_BlockingString()`으로 동기 전송한다. 시작 마커, `Num[0]`~`Num[99]`, 종료 마커까지 출력한 뒤 `hsDEBUG_UART_TX_BlockingEnd()`로 UART 독점 구간을 해제한다.

### 2.4 trfrun 매핑 교체

`IDDD_RunCommand_App.c`의 `cmdIDDD_TRF_RUN` 분기에서 호출 대상을 신규 버스트 함수로 교체한다. 기존 `IDDD_RunCmd_TRF_Real()` 호출은 삭제하지 않고 원복용 주석으로 보존한다. 로그 헤더에는 기존 로그와 혼동을 방지하기 위해 식별 태그(예: `BURST_OFF150`)를 넣는다.

### 2.5 헤더 프로토타입 추가

신규 함수 프로토타입을 `IDDD_RunCmd_TRF.h`에 추가한다. 기존 함수 선언은 그대로 유지한다.

## 3. 검증 계획

- [ ] 타이밍 프로파일 상수 갱신 및 `Validate()` 정합 확인
- [ ] 신규 버스트 함수 구현 및 빌드 성공
- [ ] 기존 Meas 모드 `HAL_ADC_ConvCpltCallback`(DMA TC) 경로 재사용으로 종료 CNT 캡처 확인
- [ ] `trfrun` 매핑 교체 및 원복 주석 보존 확인
- [ ] 로그 헤더 `BURST_OFF150` 식별 태그 출력 확인
- [ ] 1회 실행 시 원시 100샘플 UART 출력 정상
- [x] TRF burst 출력부를 `hsDebug_MSG()` 큐 경로에서 분리
- [x] UART blocking 전송 전 기존 TX queue empty 및 UART READY 대기 적용
- [x] 시작 마커부터 `Num[0]`~`Num[99]`, 종료 마커까지 GUI 파서 100샘플 인식 확인
- [ ] 버스트 시작 시각이 OFF+150us 부근으로 기록되는지 확인
- [ ] 버스트 총시간이 약 180us(79.5cycle 기준) 수준으로 기록되는지 확인
- [ ] dark 카세트 실행과 기준 카세트 실행 로그가 각각 정상 확보되는지 확인
- [ ] 기존 명령(`Real()` 원복 경로, `trftim` 등) 회귀 없음 확인

## 4. 검증 결과

2026-08-06 현재 `trfrun` 실행에서 `BURST_OFF150` 로그가 정상 출력되었고, ADC 원시값은 `Num[0]`부터 `Num[99]`까지 모두 출력되었다. GUI 파서 결과는 `Total values extracted: 100`, `Values per cycle: 100`으로 확인되었다.

기존에 관찰되던 90~93개 조기 종료 현상은 TRF burst 로그를 동기 전송 경로로 분리한 뒤 재현되지 않았다. 다만 GUI가 `Original string`과 분석 결과를 같은 콘솔에 다시 출력하므로 종료 마커가 표시상 한 번 더 보일 수 있다. 이 표시 아티팩트는 펌웨어 원문 프레임 완결성과 GUI 파싱 결과에는 영향을 주지 않는다.

