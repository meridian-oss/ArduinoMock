# ArduinoMock

PlatformIO の `platform = native` で動作させるための Arduino Mock ライブラリです。  
`Arduino.h` をオーバーライドし、[Arduino リファレンス](https://docs.arduino.cc/learn/programming/reference/) に記載された全 API をサポートします。  
ハードウェアにあたる **Serial / SPI / I2C (Wire)** クラスはコンソール出力（stdout）で動作します。

---

## 対応 API 一覧

| カテゴリ | 内容 |
|---|---|
| デジタル I/O | `pinMode`, `digitalWrite`, `digitalRead` |
| アナログ I/O | `analogRead`, `analogWrite`, `analogReference` |
| 高度な I/O | `tone`, `noTone`, `shiftIn`, `shiftOut`, `pulseIn`, `pulseInLong` |
| 時間 | `millis`, `micros`, `delay`, `delayMicroseconds` |
| 数学 | `min`, `max`, `abs`, `constrain`, `map`, `pow`, `sqrt`, `sq`, `round`, `radians`, `degrees` |
| 三角関数 | `sin`, `cos`, `tan` |
| 文字列型 | `String` クラス（全メソッド実装済み） |
| 文字判定 | `isAlpha`, `isDigit`, `isSpace`, `isUpperCase` など |
| 乱数 | `random`, `randomSeed` |
| ビット操作 | `bit`, `bitRead`, `bitSet`, `bitClear`, `bitToggle`, `bitWrite`, `highByte`, `lowByte` |
| 割り込み | `attachInterrupt`, `detachInterrupt`, `interrupts`, `noInterrupts`, `digitalPinToInterrupt` |
| 通信 | `Serial`, `Serial1`, `Serial2` (stdout/stdin), `SPI` (stdout), `Wire`, `Wire1` (stdout) |
| PROGMEM | `PROGMEM`, `F()`, `PSTR()`, `pgm_read_byte/word/dword/float/ptr` (no-ops) |
| データ型 | `boolean`, `byte`, `word` |

---

## ディレクトリ構成

```
ArduinoMock/
├── library.json            # PlatformIO ライブラリ定義
├── platformio.ini          # ビルド / テスト設定 (native)
├── build_test.sh           # g++ で直接テストを実行するスクリプト
├── src/
│   ├── Arduino.h           # メインヘッダ（Arduino.h をオーバーライド）
│   ├── Arduino.cpp
│   ├── Print.h / Print.cpp
│   ├── Stream.h / Stream.cpp
│   ├── WString.h / WString.cpp   # Arduino String クラス
│   ├── HardwareSerial.h / .cpp   # Serial → stdout/stdin
│   ├── SPI.h / SPI.cpp           # SPI → stdout
│   └── Wire.h / Wire.cpp         # Wire (I2C) → stdout
├── tools/unity/            # 最小限の Unity テストフレームワーク (g++ 用)
│   ├── unity.h
│   └── unity.c
└── test/
    └── test_main/
        └── main.cpp        # 22 テストケース
```

---

## 使い方

### PlatformIO プロジェクトへの組み込み

`platformio.ini` に以下を追記してください：

```ini
[env:native]
platform = native
lib_deps =
    https://github.com/meridian-oss/ArduinoMock.git
```

Arduino スケッチのソースから通常通り `#include <Arduino.h>` を使用できます。  
SPI / Wire を使う場合は明示的に include してください：

```cpp
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
```

### テストの実行

**PlatformIO 経由（インターネット接続が必要）：**

```bash
pio test -e native
```

**g++ で直接実行（オフライン可）：**

```bash
chmod +x build_test.sh
./build_test.sh
```

---

## Serial / SPI / Wire の動作

| クラス | 動作 |
|---|---|
| `Serial.print/println/write` | stdout へ出力 |
| `Serial.read/available` | stdin から読み込み |
| `SPI.transfer` | `[SPI] transfer ...` を stdout に出力し `0xFF` を返す |
| `Wire.write` | `[Wire] write ...` を stdout に出力 |
| `Wire.read/available` | 常に 0 を返す（ハードウェア未接続） |

---

## テスト用ヘルパー

テストコードからピン状態を注入できます：

```cpp
#include <Arduino.h>

setDigitalValue(2, HIGH);    // digitalRead(2) が HIGH を返すよう設定
setAnalogValue(0, 512);      // analogRead(0) が 512 を返すよう設定
```

---

## ライセンス

MIT
