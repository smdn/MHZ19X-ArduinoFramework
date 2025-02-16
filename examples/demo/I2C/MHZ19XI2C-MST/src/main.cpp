// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT
#include <Arduino.h>
#include <Wire.h>
#include <MHZ19XI2C.hpp>

#define ENABLE_OLED_DISPLAY 0
#if ENABLE_OLED_DISPLAY
#include <SO2002A_I2C.h>

auto display = SO2002A_I2C(0x3C);
#endif

void setup()
{
  Serial.begin(115200);

  while (!Serial)
    ;

#if ENABLE_OLED_DISPLAY
  display.begin(20, 2);
  display.clear();
  display.print("setting up ...");
#endif

  Wire.begin();

  Wire.beginTransmission(MHZ19XI2C_DEVICE_ADDRESS);
  Wire.write((uint8_t)MHZ19XI2C_command_t::write);
  Wire.write((uint8_t)MHZ19XI2C_register_t::enable_self_calibration);
  Wire.write((uint8_t)0);
  Wire.endTransmission(true);

  delay(100);

  Wire.beginTransmission(MHZ19XI2C_DEVICE_ADDRESS);
  Wire.write((uint8_t)MHZ19XI2C_command_t::write);
  Wire.write((uint8_t)MHZ19XI2C_register_t::interval);
  Wire.write((uint8_t)3);
  Wire.endTransmission(true);

  delay(100);

  Wire.beginTransmission(MHZ19XI2C_DEVICE_ADDRESS);
  Wire.write((uint8_t)MHZ19XI2C_command_t::read);
  Wire.write((uint8_t)MHZ19XI2C_register_t::enable_self_calibration);
  Wire.endTransmission(false);

  if (1 == Wire.requestFrom(MHZ19XI2C_DEVICE_ADDRESS, (uint8_t)1)) {
    Serial.print("self calibration enabled: ");
    Serial.println(Wire.read() == 0 ? "false" : "true");
  }

  delay(100);

  Wire.beginTransmission(MHZ19XI2C_DEVICE_ADDRESS);
  Wire.write((uint8_t)MHZ19XI2C_command_t::read);
  Wire.write((uint8_t)MHZ19XI2C_register_t::interval);
  Wire.endTransmission(false);

  if (1 == Wire.requestFrom(MHZ19XI2C_DEVICE_ADDRESS, (uint8_t)1)) {
    Serial.print("interval: ");
    Serial.println(Wire.read(), DEC);
  }

  delay(100);

  Wire.beginTransmission(MHZ19XI2C_DEVICE_ADDRESS);
  Wire.write((uint8_t)MHZ19XI2C_command_t::read);
  Wire.write((uint8_t)MHZ19XI2C_register_t::error_code);
  Wire.endTransmission(false);

  if (1 == Wire.requestFrom(MHZ19XI2C_DEVICE_ADDRESS, (uint8_t)1)) {
    Serial.print("error code: ");
    Serial.println(Wire.read(), HEX);
  }

  Serial.println("---------");
}

void loop()
{
  uint16_t co2ppm = 0;
  uint8_t error_code = 0;
  constexpr uint8_t error_code_i2c = 0xFF;

  Wire.beginTransmission(MHZ19XI2C_DEVICE_ADDRESS);
  Wire.write((uint8_t)MHZ19XI2C_command_t::read);
  Wire.write((uint8_t)MHZ19XI2C_register_t::latest_co2ppm);
  Wire.endTransmission(false);

  if (2 == Wire.requestFrom(MHZ19XI2C_DEVICE_ADDRESS, (uint8_t)2)) {
    co2ppm = Wire.read() << 8;
    co2ppm |= Wire.read();

    Serial.print("co2ppm: ");
    Serial.println(co2ppm);
  }

  delay(100);

  Wire.beginTransmission(MHZ19XI2C_DEVICE_ADDRESS);
  Wire.write((uint8_t)MHZ19XI2C_command_t::read);
  Wire.write((uint8_t)MHZ19XI2C_register_t::error_code);
  Wire.endTransmission(true);

  if (1 == Wire.requestFrom(MHZ19XI2C_DEVICE_ADDRESS, (uint8_t)1)) {
    error_code = Wire.read();

    Serial.print("error_code: ");
    Serial.println(error_code, HEX);
  }
  else {
    error_code = error_code_i2c;
  }

  Serial.println("-----------");

#if ENABLE_OLED_DISPLAY
  if (0 == error_code) {
    display.clear();
    display.print("CO2: ");
    display.print(co2ppm, DEC);
    display.print(" ppm");
  }
  else {
    display.clear();

    switch (error_code) {
      case 1:
        display.print("write fail");
        break;
      case 2:
        display.print("read timeout");
        break;
      case 3:
        display.print("read fail");
        break;
      case 4:
        display.print("start byte");
        break;
      case 5:
        display.print("command byte");
        break;
      case 6:
        display.print("checksum error");
        break;
      case error_code_i2c:
        display.print("I2C error");
        break;
      default:
        display.print("FAILURE");
        break;
    }
  }
#endif

  delay(2000);
}
