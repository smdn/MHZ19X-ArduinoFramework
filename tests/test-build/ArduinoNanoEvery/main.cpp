// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT
#include <Arduino.h>
#include <MHZ19X.h>

auto mhz19c_serial1 = MHZ19C_Serial1();
auto mhz19e_serial1 = MHZ19E_Serial1();

#if defined(HAVE_HWSERIAL2)
auto mhz19c_serial2 = MHZ19C_Serial2();
auto mhz19e_serial2 = MHZ19E_Serial2();
#endif

#if defined(HAVE_HWSERIAL3)
auto mhz19c_serial3 = MHZ19C_Serial3();
auto mhz19e_serial3 = MHZ19E_Serial3();
#endif

void setup()
{
  // begin()
  mhz19c_serial1.begin();

  mhz19e_serial1.begin();

#if defined(HAVE_HWSERIAL2)
  mhz19c_serial2.begin();

  mhz19e_serial2.begin();
#endif

#if defined(HAVE_HWSERIAL3)
  mhz19c_serial3.begin();

  mhz19e_serial3.begin();
#endif

  // switchSelfCalibration()
  mhz19c_serial1.switchSelfCalibration(true);

  mhz19e_serial1.switchSelfCalibration(true);

#if defined(HAVE_HWSERIAL2)
  mhz19c_serial2.switchSelfCalibration(true);

  mhz19e_serial2.switchSelfCalibration(true);
#endif

#if defined(HAVE_HWSERIAL3)
  mhz19c_serial3.switchSelfCalibration(true);

  mhz19e_serial3.switchSelfCalibration(true);
#endif
}

void loop()
{
  uint16_t co2ppm = 0;
  MHZ19X_error_t result;

  // getCO2Concentration()
  result = mhz19c_serial1.getCO2Concentration(co2ppm);

  result = mhz19e_serial1.getCO2Concentration(co2ppm);

#if defined(HAVE_HWSERIAL2)
  result = mhz19c_serial2.getCO2Concentration(co2ppm);

  result = mhz19e_serial2.getCO2Concentration(co2ppm);
#endif

#if defined(HAVE_HWSERIAL3)
  result = mhz19c_serial3.getCO2Concentration(co2ppm);

  result = mhz19e_serial3.getCO2Concentration(co2ppm);
#endif
}
