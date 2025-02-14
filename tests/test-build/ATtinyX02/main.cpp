// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT
#include <Arduino.h>
#include <MHZ19X.h>

auto mhz19c = MHZ19C<PIN_PA6, PIN_PA7>();
auto mhz19e = MHZ19E<PIN_PA6, PIN_PA7>();

void setup()
{
  // begin()
  mhz19c.begin();

  mhz19e.begin();

  // switchSelfCalibration()
  mhz19c.switchSelfCalibration(true);

  mhz19e.switchSelfCalibration(true);
}

void loop()
{
  uint16_t co2ppm = 0;
  MHZ19X_error_t result;

  // getCO2Concentration()
  result = mhz19c.getCO2Concentration(co2ppm);

  result = mhz19e.getCO2Concentration(co2ppm);
}
