[![GitHub license](https://img.shields.io/github/license/smdn/MHZ19X-ArduinoFramework)](https://github.com/smdn/MHZ19X-ArduinoFramework/blob/main/LICENSE.txt)
[![tests/main](https://img.shields.io/github/actions/workflow/status/smdn/MHZ19X-ArduinoFramework/test-build.yml?branch=main&label=tests%2Fmain)](https://github.com/smdn/MHZ19X-ArduinoFramework/actions/workflows/test-build.yml)

# MH-Z19 CO2 sensor library for Arduino Framework
Winsen Sensors製 NDIR方式 CO2センサーモジュール、**MH-Z19シリーズ**用のライブラリです。

Arduino Frameworkが利用可能な各種ボード/MCUでMH-Z19シリーズを動作させることができます。

# Usage / 使用例
[examples](./examples/)ディレクトリから、各ボード/MCU向けのサンプルコードを閲覧できます。

The sample codes for various boards/MCUs can be found in the [examples](./examples/) directory.

# Features / 特徴
Arduino Framework用のライブラリです。　[PlatformIO](https://platformio.org/)での動作を確認していますが、Arduino IDEでも動作が期待できます。

This library has been tested with [PlatformIO](https://platformio.org/), is also expected to work with the Arduino IDE.

`MH-Z19`はPWM信号/シリアル通信の二種類で計測値を取得できますが、現時点では本ライブラリは**シリアル通信**のみをサポートしています。

This library only supports retrieving measurement values by the serial communication.　Retrieving measurement values from the PWM signals is not supported.

## APIとサポートする機能

### 測定値の取得
以下のようなAPIでセンサモジュールの操作を行うことができます。

現時点では、MH-Z19シリーズがサポートするCO2濃度の読み取り(コマンド`0x86`)および、セルフキャリブレーションの有効化/無効化の設定(コマンド`0x79`)のみをサポートしています。

```cpp
#include <Arduino.h>
#include <MHZ19X.h>

// MH-Z19Cセンサモジュールを操作するインスタンスを作成
auto co2sensor = MHZ19C(); // Arduinoおよび同互換ボードの場合
// auto co2sensor = MHZ19C_UART1(); // ESP32の場合

void setup()
{
  co2sensor.begin();

  // セルフキャリブレーションをオンにする
  co2sensor.switchSelfCalibration(true);
}

void loop()
{
  uint16_t co2ppm;
  MHZ19X_error_t result;

  // CO2濃度を取得する
  result = co2sensor.getCO2Concentration(co2ppm);

  if (MHZ19X_error_t::success == result) {
    // 取得に成功した場合、「なんらかのディスプレイ」に測定値を表示する
    // display.print(co2ppm);
  }

  delay(3000);
}
```

> [!IMPORTANT]
> センサモジュールを操作する型の名前は、Arduinoおよび同互換ボードでは`MHZ19C`となりますが、これはボード/MCUごとに異なります。　詳細は[examples](./examples/)ディレクトリにあるボード/MCUごとのサンプルコードを参照してください。

> [!IMPORTANT]
> The type name for the sensor module is `MHZ19C` for Arduino-compatible boards. This type name, however, depends on the board/MCU. See [examples](./examples/) for the actual type names.


### 任意のUARTシリアル通信の実装を使用可能
本ライブラリでは、さまざまなボード/MCUで動作させられるよう、UARTシリアル通信を任意の実装に差し替えられるようにしています。　具体的には、[`MHZ19XDriver`](./src/MHZ19XDriver.hpp)クラスのテンプレートパラメータ`TUartStream`に任意の実装を指定することができます。

デフォルトでは、標準で使用できるシリアル通信を利用しますが、必要に応じてUARTシリアル通信の実装を差し替えることにより、任意のGPIOピンを使用するように変更することもできます。

> [!NOTE]
> 標準のシリアル通信が利用できない場合は、ライブラリでのフォールバック実装として[`SloppySoftwareSerialStream`](./src/SloppySoftwareSerialStream.hpp)が利用可能です。
> ただし、これは`digitalRead()`, `digitalWrite()`および`delayMicroseconds()`を使用した*雑な*実装であり、動作が安定せず期待する結果を得られない場合があります。


## Modules confirmed to work / 動作確認済みモジュール
- MH-Z19C
- MH-Z19E

> [!NOTE]
> 他のセンサーモジュールでも動作する可能性はありますが、実機での動作は未検証です。
> Also the library may work with other sensor modules, but has not been tested with actual modules.


## Boards and MCUs confirmed to work / 動作確認済みのボードとMCU
- ESP32
  - ESP32-WROOM-32
- ATtiny
  - ATtiny 402
  - ATtiny 404
- Arduino
  - Arduino Nano
  - Arduino Nano Every

> [!IMPORTANT]
> ATtinyで動作させる場合、20 MHzまたは16 MHzで動作させることを推奨します。　それより低い動作周波数では、シリアル通信が安定しません。
> これは[シリアル通信に使用している実装](./src/SloppySoftwareSerialStream.hpp)が*雑な*実装であるためです。　`delayMicroseconds`に渡す値の調整により改善する場合もありますが、電源電圧の影響を受ける場合もあり、調整の難易度は高いです。
> ATtinyで動作する、より良いシリアル通信を実装してくれる方がいましたら、ぜひIssuesにてご提案ください。

# References / 参考情報
- [Parts:Sensor:MH-Z19C - robot-jp wiki](https://robot-jp.com/wiki/index.php/Parts:Sensor:MH-Z19C)
- https://github.com/SpenceKonde/megaTinyCore/blob/master/megaavr/extras/PlatformIO.md



# For contributers
Contributions are appreciated!

If there's a feature you would like to add or a bug you would like to fix, please read [Contribution guidelines](./CONTRIBUTING.md) and create an Issue or Pull Request.

IssueやPull Requestを送る際は、[Contribution guidelines](./CONTRIBUTING.md)をご覧頂ください。　使用言語は日本語で構いません。



# Notice
(An English translation for the reference follows the text written in Japanese.)

## License
本プロジェクトは[MIT License](./LICENSE.txt)の条件に基づきライセンスされています。

This project is licensed under the terms of the [MIT License](./LICENSE.txt).

## Disclaimer
本プロジェクトは、`MH-Z19`シリーズの製造元・供給元・販売元とは無関係の、非公式なものです。

This is an unofficial project that has no affiliation with the manufacturers/vendors/suppliers of `MH-Z19` series.
