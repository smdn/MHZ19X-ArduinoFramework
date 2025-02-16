// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT
#include <MHZ19X.h>
#include <SO2002A_I2C.h>

auto co2sensor = MHZ19C<PIN_PA6, PIN_PA7>();
auto display = SO2002A_I2C(0x3C);

constexpr int updateIntervalInMillisecs = 2500;

void setup()
{
  display.begin(16, 2);
  display.clear();
  display.print("setup ");

  co2sensor.begin();
  co2sensor.switchSelfCalibration(true);

  display.print("ready");
}

void loop()
{
  uint16_t co2ppm = 0;
  MHZ19X_error_t result;

  result = co2sensor.getCO2Concentration(co2ppm);

  if (MHZ19X_error_t::success == result) {
    display.clear();
    display.print("CO2: ");
    display.print(co2ppm, DEC);
    display.print(" ppm");
  }
  else {
    display.clear();

    switch (result) {
      case MHZ19X_error_t::uart_write_failure:
        display.print("write fail");
        break;
      case MHZ19X_error_t::uart_read_timeout:
        display.print("read timeout");
        break;
      case MHZ19X_error_t::uart_read_failure:
        display.print("read fail");
        break;
      case MHZ19X_error_t::returnvalue_startbyte_mismatch:
        display.print("start byte");
        break;
      case MHZ19X_error_t::returnvalue_commandbyte_mismatch:
        display.print("command byte");
        break;
      case MHZ19X_error_t::returnvalue_checksum_mismatch:
        display.print("checksum error");
        break;
      default:
        display.print("FAILURE");
        break;
    }
  }

  delay(updateIntervalInMillisecs);
}
