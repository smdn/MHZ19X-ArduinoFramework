// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT
#include <Arduino.h>
#include <MHZ19X.h>

// use UART2 (Hardware serial; RX=GPIO16, TX=GPIO17)
auto co2sensor = MHZ19C_UART2();

/*
// use UART1 (Hardware serial; RX=GPIO9, TX=GPIO10)
auto co2sensor = MHZ19C_UART1();
*/

/*
// Any other GPIO pin can also be used.
// In the example below, GPIO32 is used for RX and GPIO33 for TX.
auto co2sensor = MHZ19XDriver<Baud9600SloppySoftwareSerialStream<32, 33>>();
*/

void setup()
{
  Serial.begin(115200);

  co2sensor.begin();
  co2sensor.switchSelfCalibration(true);
}

void loop()
{
  uint16_t co2ppm = 0;
  MHZ19X_error_t result;

  result = co2sensor.getCO2Concentration(co2ppm);

  if (MHZ19X_error_t::success == result) {
    Serial.print(co2ppm);
    Serial.println(" ppm");
  }
  else {
    Serial.print("error: ");
    Serial.println((int)result);
  }

  delay(3000);
}
