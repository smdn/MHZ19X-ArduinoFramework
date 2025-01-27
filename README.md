# MH-Z19 CO2 sensor library for Arduino Framework
Winsen Sensors製 NDIR方式 CO2センサーモジュール、**MH-Z19シリーズ**用のライブラリです。

Arduino Frameworkが利用可能なMCU/マイクロコントローラでMH-Z19シリーズを動作させることができます。

# Usage / 使用例
[examples](./examples/)ディレクトリから、各ボード/MCU向けのサンプルコードを閲覧できます。

The sample codes for various boards/MCUs can be found in the [examples](./examples/) directory.

# Features / 特徴
Arduino Framework用のライブラリです。　[PlatformIO](https://platformio.org/)での動作を確認していますが、Arduino IDEでも動作が期待できます。

This library has been tested with [PlatformIO](https://platformio.org/), is also expected to work with the Arduino IDE.

`MH-Z19`はPWM信号/シリアル通信の二種類で計測値を取得できますが、現時点では本ライブラリは**シリアル通信**のみをサポートしています。

This library only supports retrieving measurement values by the serial communication.　Retrieving measurement values from the PWM signals is not supported.

## APIとサポートする機能
### 任意のUARTシリアル通信の実装を使用可能
本ライブラリでは、さまざまなMCU/マイクロコントローラで動作させられるよう、UARTシリアル通信を任意の実装に差し替えられるようにしています。　具体的には、[`MHZ19XDriver`](./src/MHZ19XDriver.hpp)クラスのテンプレートパラメータ`TUartStream`に任意の実装を指定することができます。

デフォルトでは、標準で使用できるシリアル通信を利用しますが、必要に応じてUARTシリアル通信の実装を差し替えることにより、任意のGPIOピンを使用するように変更することもできます。

> [!NOTE]
> 標準のシリアル通信が利用できない場合は、ライブラリでのフォールバック実装として[`SloppySoftwareSerialStream`](./src/SloppySoftwareSerialStream.hpp)が利用可能です。
> ただし、これは`digitalRead()`, `digitalWrite()`および`delayMicroseconds()`を使用した*雑な*実装であり、動作が安定せず期待する結果を得られない場合があります。

## Modules confirmed to work / 動作確認済みモジュール
- MH-Z19C

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

> [!WARNING]
> ATtinyで動作させる場合、20 MHzまたは16 MHzで動作させることを推奨します。　それより低い動作周波数では、シリアル通信が安定しません。
> これは[シリアル通信に使用している実装](./src/SloppySoftwareSerialStream.hpp)が*雑な*実装であるためです。　`delayMicroseconds`に渡す値の調整により改善する場合もありますが、電源電圧の影響を受ける場合もあり、調整の難易度は高いです。
> ATtinyで動作する、より良いシリアル通信を実装してくれる方がいましたら、ぜひIssuesにてご提案ください。

# References / 参考情報
- [Parts:Sensor:MH-Z19C - robot-jp wiki](https://robot-jp.com/wiki/index.php/Parts:Sensor:MH-Z19C)
- https://github.com/SpenceKonde/megaTinyCore/blob/master/megaavr/extras/PlatformIO.md

# Notice
(An English translation for the reference follows the text written in Japanese.)

## License
本プロジェクトは[MIT License](./LICENSE.txt)の条件に基づきライセンスされています。

This project is licensed under the terms of the [MIT License](./LICENSE.txt).

## Disclaimer
本プロジェクトは、`MH-Z19`シリーズの製造元・供給元・販売元とは無関係の、非公式なものです。

This is an unofficial project that has no affiliation with the manufacturers/vendors/suppliers of `MH-Z19` series.
