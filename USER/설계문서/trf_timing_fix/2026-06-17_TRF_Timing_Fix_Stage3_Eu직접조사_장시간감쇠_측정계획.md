# 2026-06-17_TRF_Stage4_Eu직접조사_장시간감쇠_측정계획

## 1. 목적

유로피움 형광막을 1.0mm 카세트 틈새 경로가 아니라 표면 직접 조사 조건으로 측정했을 때, LED OFF 이후 ADC raw가 2ms 시점까지도 dark 수준으로 복귀하지 않는 현상이 확인됐다.

기존 `TRF REAL Stage2` 측정은 `20us x 100 samples`로 0~1980us까지만 기록하므로, 감쇠가 어디까지 내려가는지 확인하기 전에 측정이 종료된다. 본 계획은 측정 시간을 4~10ms 범위로 확장하여 Eu 직접 조사 조건의 장시간 감쇠 구간을 확인하는 것을 목적으로 한다.

## 2. 배경

### 2.1 기존 1.0mm 카세트 틈새 구조

원래 1.0mm 카세트는 유로피움 형광물질이 도포된 유리막 조각을 1.0mm 틈새에 배치하고, 해당 틈새로 여기광을 조사하며 같은 틈새 경로로 PD/TIA/ADC가 신호를 읽는 구조다.

이 조건에서는 LED ON 구간의 광 응답은 관찰됐지만, LED OFF 이후 장시간 형광감쇠 tail은 dark 수준으로 유지되어 확인되지 않았다.

### 2.2 Eu 형광막 직접 조사 조건

카세트를 분리하여 유로피움 형광막 유리 조각을 꺼낸 뒤, 틈새 경로가 아니라 형광막 표면 자체에 빛을 조사하고 ADC를 읽었을 때 LED OFF 이후 강한 지연 신호가 관찰됐다.

대표적으로 `cur=500 / Eu direct` 그래프에서는 다음 흐름이 보였다.

- dark 평균: 약 135
- LED ON 구간: raw 약 1100~1900 범위
- LED OFF 직후 gate 구간: 일시적으로 dark 근처
- 약 1350us 이후: raw가 다시 크게 상승
- 약 1500us 부근: peak 약 3000
- 1980us 시점: raw 약 1400 수준으로, 아직 dark보다 크게 높음

따라서 현재 2ms 측정 범위는 감쇠 종료를 확인하기에 부족하다.

## 3. 현재 측정 한계

현재 펌웨어의 Stage2 실측 설정은 다음과 같다.

```c
TRF_REAL_SAMPLE_COUNT       = 100U
TRF_REAL_SAMPLE_INTERVAL_US = 20U
TRF_REAL_LED_ON_US          = 200U
TRF_REAL_LED_OFF_US         = 1200U
```

측정 범위는 다음과 같다.

```text
0us ~ 1980us
```

Eu 직접 조사 조건에서는 1980us에서도 감쇠가 끝나지 않았으므로, 현재 로그만으로는 lifetime, 감쇠 종료 시점, plateau 여부를 판단할 수 없다.

## 4. 측정 확장 방안

### 4.1 1차: 4ms 확장 측정

가장 먼저 수행할 기본 확장안이다.

```c
TRF_REAL_SAMPLE_COUNT       = 200U
TRF_REAL_SAMPLE_INTERVAL_US = 20U
```

측정 범위:

```text
0us ~ 3980us
```

장점:

- 기존 20us 해상도를 유지한다.
- 현재 100샘플 구조를 단순 확장하므로 구현 위험이 낮다.
- 구조체 버퍼 200개 기준 약 800 bytes로 부담이 작다.

판정:

- 4ms 안에 raw가 dark + 10~20 count 근처로 내려오는지 확인한다.
- 4ms에서도 raw가 높게 유지되면 2차 측정으로 넘어간다.

### 4.2 2차: 6ms 확장 측정

4ms에서 감쇠 종료가 확인되지 않을 경우 수행한다.

```c
TRF_REAL_SAMPLE_COUNT       = 300U
TRF_REAL_SAMPLE_INTERVAL_US = 20U
```

측정 범위:

```text
0us ~ 5980us
```

장점:

- 20us 해상도를 유지하면서 약 6ms까지 감쇠를 확인할 수 있다.
- 버퍼는 구조체 300개 기준 약 1.2KB 수준이다.

주의:

- UART 출력량이 증가하므로 로그 누락이나 GUI 파서 처리 여부를 확인한다.

### 4.3 3차: 10ms coarse 측정

감쇠 종료 시점만 확인하거나 긴 tail의 전체 추세를 보고 싶을 때 수행한다.

```c
TRF_REAL_SAMPLE_COUNT       = 200U
TRF_REAL_SAMPLE_INTERVAL_US = 50U
```

측정 범위:

```text
0us ~ 9950us
```

장점:

- 200샘플 출력량을 유지하면서 약 10ms까지 볼 수 있다.
- 장시간 tail의 종료 시점 또는 plateau 여부 확인에 유리하다.

단점:

- 20us 대비 시간 해상도가 낮아 LED OFF 직후 세부 파형 분석에는 부적합하다.
- 이 모드는 장시간 감쇠 끝 확인용으로만 사용한다.

## 5. 권장 진행 순서

1. `cur=500 / Eu 직접 조사` 조건에서 `20us x 200 samples`로 4ms 측정
2. 4ms에서도 dark로 복귀하지 않으면 `20us x 300 samples`로 6ms 측정
3. 6ms에서도 감쇠가 충분히 끝나지 않으면 `50us x 200 samples`로 10ms coarse 측정
4. 같은 설정으로 `cur=3000 / Eu 직접 조사` 조건도 측정하여 광량 의존성을 비교
5. 필요 시 기존 1.0mm 틈새 카세트 조건에서도 동일한 장시간 측정을 수행하여 직접 조사 조건과 비교

## 6. 측정 조건

우선순위 조건:

| 조건 | 목적 |
|---|---|
| Eu 형광막 표면 직접 조사 / cur=500 | 현재 감쇠가 잘려 보인 기준 조건 |
| Eu 형광막 표면 직접 조사 / cur=3000 | 안정적인 LED ON 및 강한 tail 비교 조건 |
| 기존 1.0mm 카세트 조립 상태 / cur=500 | 원래 광학 구조와 직접 조사 조건 비교 |
| 비코팅 유리 또는 빈 상태 | 형광이 아닌 반사/외부광/회복 아티팩트 분리 |

광학 조건은 측정 중 고정해야 한다. 보드를 들추거나 카세트/유리막 위치를 바꾸면 외부광 유입 및 반사 조건이 달라져 OFF 이후 비정상 상승이 발생할 수 있다.

## 7. 출력 및 분석 항목

### 7.1 로그 출력

기존 출력 형식을 유지하되, sample count와 interval을 명확히 출력한다.

예:

```text
----- TRF REAL Stage2 (TIM-CNT poll) -----
ch:0 cur:500 dark:135 samples:200 interval:20us
[t=0us]...
```

dark 평균은 고정 샘플 수가 아니라 타임스탬프 기준으로 계산한다.

```text
tim_us < TRF_REAL_LED_ON_US
```

위 조건을 만족하는 샘플만 dark 평균에 포함한다. 이렇게 해야 `20us x 200`, `50us x 200`처럼 sample interval을 바꿔도 LED ON 이후 샘플이 dark 평균에 섞이지 않는다.

### 7.2 분석 항목

- dark 평균
- LED ON 구간 raw min/max/mean
- LED OFF 이후 peak 시점 및 peak raw
- peak 이후 감쇠 추세
- raw가 `dark + 10~20 count` 근처로 내려오는 시간
- 4ms/6ms/10ms 시점의 raw 잔류값
- cur=500과 cur=3000의 감쇠 형태 비교
- Eu 직접 조사와 기존 1.0mm 틈새 구조의 OFF 이후 파형 비교

## 8. 판정 기준

### 8.1 장시간 측정 성공 기준

- 설정한 샘플 수만큼 로그가 정상 출력된다.
- 타임스탬프가 설정 간격대로 증가한다.
- LED ON/OFF 타이밍이 기존과 동일하게 유지된다.
- 감쇠 구간이 2ms 이후까지 연속적으로 확인된다.

### 8.2 물리 현상 판정 기준

Eu 직접 조사 조건에서만 장시간 tail이 나타나고, 비코팅 유리/빈 상태에서는 나타나지 않으면 유로피움 발광 성분 가능성이 높다.

반대로 비코팅 유리나 빈 상태에서도 유사한 지연 상승/감쇠가 나타나면, 형광이 아니라 TIA 회복, 외부광 유입, 반사 조건 변화, ADC/TIA 아티팩트 가능성을 우선 검토한다.

기존 1.0mm 틈새 구조에서는 tail이 계속 보이지 않고 Eu 직접 조사에서만 tail이 보인다면, 펌웨어/ADC보다는 카세트 광학 구조, 코팅면 방향, 여기광 조사 경로, 방출광 수광 경로 문제를 우선 검토한다.

## 9. 구현 메모

단기 검증은 상수 변경으로 충분하다.

1차 변경:

```c
#define TRF_REAL_SAMPLE_COUNT           200U
#define TRF_REAL_SAMPLE_INTERVAL_US      20U
```

2차 변경:

```c
#define TRF_REAL_SAMPLE_COUNT           300U
#define TRF_REAL_SAMPLE_INTERVAL_US      20U
```

3차 변경:

```c
#define TRF_REAL_SAMPLE_COUNT           200U
#define TRF_REAL_SAMPLE_INTERVAL_US      50U
```

단, 반복 실험이 많아질 경우에는 compile-time define 변경 대신 CLI 파라미터 또는 별도 진단 명령으로 sample count/interval을 선택할 수 있게 확장하는 것이 좋다.

## 10. 다음 액션

1. 4ms 확장 측정용으로 sample count 200 적용
2. `cur=500 / Eu 직접 조사` 로그 확보
3. 감쇠가 dark 근처까지 내려오는지 확인
4. 부족하면 6ms 또는 10ms coarse 측정으로 확장
5. 결과 그래프를 `Stage2_Measurement` 폴더에 저장
6. 감쇠 종료 시점 및 조건별 비교 결과를 별도 실측 결과 문서로 정리
