// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT
#include <Arduino.h>
#include <MHZ19X.h>

auto mhz19c_uart1 = MHZ19C_UART1();
auto mhz19c_uart2 = MHZ19C_UART2();

auto mhz19e_uart1 = MHZ19C_UART1();
auto mhz19e_uart2 = MHZ19C_UART2();

void setup()
{
  // begin()
  mhz19c_uart1.begin();
  mhz19c_uart2.begin();

  mhz19e_uart1.begin();
  mhz19e_uart2.begin();

  // switchSelfCalibration()
  mhz19c_uart1.switchSelfCalibration(true);
  mhz19c_uart2.switchSelfCalibration(true);

  mhz19e_uart1.switchSelfCalibration(true);
  mhz19e_uart2.switchSelfCalibration(true);
}

void loop()
{
  uint16_t co2ppm = 0;
  MHZ19X_error_t result;

  // getCO2Concentration()
  result = mhz19c_uart1.getCO2Concentration(co2ppm);
  result = mhz19c_uart2.getCO2Concentration(co2ppm);

  result = mhz19e_uart1.getCO2Concentration(co2ppm);
  result = mhz19e_uart2.getCO2Concentration(co2ppm);
}
