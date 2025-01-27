// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT
#include <Arduino.h>
#include <MHZ19X.h>

// use PA6 is used for RX and PA7 for TX.
auto co2sensor = MHZ19C<PIN_PA6, PIN_PA7>();

void setup()
{
  co2sensor.begin();
  co2sensor.switchSelfCalibration(true);
}

void loop()
{
  uint16_t co2ppm = 0;
  MHZ19X_error_t result;

  result = co2sensor.getCO2Concentration(co2ppm);

  if (MHZ19X_error_t::success == result) {
    // display.print(co2ppm);
  }

  delay(3000);
}
