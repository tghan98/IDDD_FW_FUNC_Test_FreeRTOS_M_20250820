# BLE BYPASS 모드 +ERROR 문제 해결 기록

> **✅ 문제 해결 완료 (2026-05-22)**
> 최종 원인: BoT-nLE523 USB 동글 GPI 헤더핀 납땜 불량 → GPI 레벨 불확정/HIGH → BYPASS 진입 불가.
> 정상 동글 교체 후 모든 CLI 명령(help, info, ledpw 등) 정상 동작 확인.

---

## 1. 시스템 구성

| 항목 | 내용 |
|------|------|
| MCU | STM32U031xx + FreeRTOS |
| BLE SERVER | BoT-nLE521 (MCU 보드 탑재, USART1, 19200bps) |
| BLE CLIENT | BoT-nLE523 (USB 동글, COM3, 115200bps, FTDI 기반) |
| GUI | Qt6 PC 프로그램, COM3 통해 동글과 통신 |
| 역할 | GUI → 동글(CLIENT) → BLE → MCU(SERVER) → 센서 측정 |

### MCU 보드 BLE 관련 핀

| 핀 | 설명 |
|----|------|
| BLE_C_CK | GPIOF pin0, pull-down, SERVER 연결 상태 읽기용 |
| BLE_AT_CMD | PCB 하드와이어 LOW (pull-down 저항), MCU GPIO 제어 불가 |

### 동글 GPI 핀 (핵심)

- 동글 GPI(P14/#24 핀): **HIGH = AT Command Mode**, **LOW + Falling Edge = BYPASS Mode**
- 반드시 HIGH→LOW Falling Edge가 있어야 BYPASS 진입 (Level LOW 유지만으로는 부족)
- 동글 PCB 스위치 현재 위치: ●(하단) = GPI LOW 고정
- `AT+INTPULLDOWN=ON` 확인됨 (GPI 내부 풀다운 활성화)
- **DTR 신호 → 동글 GPI 핀에 직결**: DTR HIGH = GPI HIGH = AT 모드 강제

---

## 2. 원인 확정 (2026-05-22 Python 검증 완료)

### 1차 원인: Qt QSerialPort DTR 자동 HIGH 어서션

```
GUI Open 버튼
  → QSerialPort::open() 호출
  → Windows FTDI 드라이버가 DTR 신호를 자동으로 HIGH
  → 동글 GPI HIGH
  → AT Command Mode 강제 유지
  → 모든 명령에 +ERROR
```

**Python 검증**: `DTR=False`로 COM3를 열고 BLE 연결 시퀀스 실행 → `info` 명령 정상 동작 확인.
검증 스크립트: `C:\Users\hansu\Desktop\ble_dtr_test.py`

### 2차 원인: BYPASS 비활성 타임아웃 (현재 미해결)

1차 원인(DTR) 해결 후 추가 발견:
- `help`, `info` 등 2~3개 명령은 정상 동작
- 명령 간 수초 간격이 생기면 +ERROR 재발
- BLE 채널로 데이터 흐름이 없으면 BoT-nLE523 동글이 내부 타임아웃으로 BYPASS를 종료하고 AT 모드로 복귀하는 것으로 추정

---

## 3. 완료된 작업

### 3-1. GUI 소스코드 위치

```
C:\Users\hansu\Desktop\INCLIX F-1 V2\sw\F1_gui\@Optic_test_소스코드\@Optic_test\
백업: @Optic_test_소스코드_backup_20260522 (원본 보존됨)
```

### 3-2. `Common\Network\CSerialNetwork.cpp` — 수정 완료

**변경 1: open() 3개 오버로드에 DTR=false 추가**

```cpp
// 기존
return d->mSerialPort.open(this->getOpenMode(openMode));

// 수정 후 (3개 오버로드 모두 동일 패턴)
bool opened = d->mSerialPort.open(this->getOpenMode(openMode));
d->mSerialPort.setDataTerminalReady(false);
return opened;
```

**변경 2: Qt6 호환 — errorOccurred 시그널명 수정**

```cpp
// 기존 (Qt5)
connect(&d->mSerialPort, QOverload<QSerialPort::SerialPortError>::of(&QSerialPort::error), ...)

// 수정 후 (Qt6)
connect(&d->mSerialPort, &QSerialPort::errorOccurred, this, [this]
        (QSerialPort::SerialPortError serialPortError)
        {
            this->close();
        });
```

~~**변경 3: DTR 유지 타이머 (500ms마다 DTR=false 재설정)**~~ → **제거됨 (2026-05-22)**

BYPASS 문제 해결에 기여하지 못했고 GPI edge/level 상태 분석을 흐리는 변수로 판단. 제거.

### 3-3. `Common\CUtil.cpp` — Qt6 호환 수정 완료

```cpp
// QString::sprintf 제거됨 → asprintf 로 전면 교체 (6곳)
// Before: retValue.sprintf("%02x", target);
return QString::asprintf("%02x", target);
return QString::asprintf("%02x%02x", targetBuf[0], targetBuf[1]);
return QString::asprintf("%02x%02x%02x%02x", targetBuf[0], targetBuf[1], targetBuf[2], targetBuf[3]);

// uchar push_back 암묵적 변환 제거됨 → 명시적 캐스트
// Before: retValue.push_back(target);  // uchar
retValue.push_back((char)target);
```

### 3-4. `Widgets\MainWidgets\MainWidget.cpp` — 변경 없음 (원본 상태)

keep-alive `\r` 타이머 접근법을 시도했으나 MCU CLI 노이즈 문제로 완전 원복됨.
현재 파일은 원본과 동일하며, 추가된 코드 없음.

---

## 4. 현재 남은 문제: BYPASS 비활성 타임아웃

### 증상

```
[연결 성공 후]
HS> help          ← 성공
HS> info          ← 성공 (전체 출력 수신됨)
[수 초 대기]
+ERROR            ← 사용자 클릭 1 (BYPASS 이미 종료된 상태)
+ERROR            ← 사용자 클릭 2
+ERROR            ← 사용자 클릭 3
```

### 원인 추정

BoT-nLE523 BYPASS 모드에 내부 비활성 타임아웃이 있어, BLE 채널로 데이터 흐름이 없으면 자동으로 AT 모드로 복귀함.
타임아웃 길이: 1.5초는 OK (Python 테스트 성공), 5초는 실패 → 약 2~4초로 추정.

### 제안된 해결 방법 (미구현, Codex가 검토·구현 필요)

#### 방안 A: GUI에서 BLE 채널 keep-alive (MCU 펌웨어 수정 병행)

1. **GUI**: `QTimer`로 2초마다 `\r` 전송 (BLE 채널을 활성 상태로 유지)
2. **MCU 펌웨어**: 빈 엔터 입력 시 `HS> ` 프롬프트 재출력 생략

   수정 위치: `USER/L3_Application/L3_00_hsDEBUG_UART_CLI_App/hsDEBUG_UART_CLI_App.c`

   FreeRTOS CLI에서 빈 입력(길이 0) 처리 시 프롬프트 출력 조건부 억제.

   GUI 수정 위치: `Widgets\MainWidgets\MainWidget.cpp` — `serialOpen()` 성공 및 `+CONNECTED` 수신 후 keep-alive 타이머 시작, `serialClose()` 시 정지.

#### 방안 B: GUI에서 BLE 채널 keep-alive (MCU 수정 없이 필터링)

1. **GUI**: 2초마다 `\r` 전송
2. **GUI**: 수신 데이터에서 `\nHS> ` 단독 줄(keep-alive 응답)은 로그 위젯에 표시하지 않음

#### 방안 C: BoT-nLE523 BYPASS 타임아웃 AT 명령으로 비활성화

BoT-nLE523 매뉴얼 확인 필요: `AT+BYPASSTIMEOUT` 또는 유사 명령으로 타임아웃을 0(무제한)으로 설정 가능한지 조사.
가능하다면 GUI 연결 시퀀스(`onClickedOpen` → AT 명령 전송 단계)에 추가.

---

## 5. MCU 펌웨어 현재 상태

파일: `USER/L3_Application/L3_00_hsDEBUG_UART_CLI_App/hsDEBUG_UART_CLI_App.c`

```c
HW_BLE_Power_ONnOFF(ON);
vTaskDelay(1000);
BoTnLE_AT_Command_Send(&g_tBLE_Drv, "AT+SECLV=4\r"); // 부팅 시 보안 레벨 복원
vTaskDelay(1500);

vRegisterCLICommands();
hsDebug_MSG("*** Micro Optics Test Command Line Interface ***\n");
cliWrite(cli_prompt);
```

- `AT+SECLV=4`: SERVER 보안 레벨 복원 목적으로 유지 중
- `AT+DELBOND`: 효과 없어 제거됨
- CLI 프롬프트: `"\nHS> "`
- CLI는 수신된 모든 문자를 echo하며, `+CONNECTED[MAC]` 이벤트가 USART1로 들어오면 CLI가 이를 명령으로 처리 → "Command not recognised" 출력됨 (정상 범위 내 동작, 기능상 무해)

---

## 6. 시도했으나 실패·원복된 방법

| 방법 | 결과 | 원복 여부 |
|------|------|----------|
| MCU→GUI keep-alive `\r` 전송 (MCU RX 태스크에서 BLE 연결 상태에서만) | 실패 - CLIENT 타임아웃은 단방향 | 원복 |
| GUI→MCU keep-alive `\r` 2초마다 + bypassActive 플래그 | MCU 노이즈 + DTR 문제 중첩으로 불안정 | 원복 |
| AT+SECLV=1 | 부분 개선 후 불안정 | 원복 (AT+SECLV=4로) |
| AT+DELBOND (서버 부팅 시) | 효과 없음 | 제거됨 |
| 신호 강도 개선 | 무관 | - |

---

## 7. 빌드 환경

| 항목 | 내용 |
|------|------|
| Qt 버전 | Qt 6.11.1 (MinGW 13.1.0 64-bit) |
| Qt Creator | 설치됨, 정상 빌드 확인 |
| 프로젝트 파일 | `@Optic_test\Optics test.pro` |
| 필수 모듈 | `QT += core gui serialport charts` |
| 빌드 방법 | Qt Creator → Build → Rebuild All (변경 후 반드시 Rebuild, qmake 재실행 필요) |

---

*최종 업데이트: 2026-05-22*
