// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT
#include <Arduino.h>
#include <MHZ19X.h>

// This code uses the `Serial1` port to connect to the MH-Z19C.
auto co2sensor = MHZ19C_Serial1();

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
