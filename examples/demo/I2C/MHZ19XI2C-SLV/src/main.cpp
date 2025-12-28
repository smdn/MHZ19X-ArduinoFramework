// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT
#include <Wire.h>
#include <MHZ19X.h>
#include <MHZ19XI2C.hpp>
#include <avr/wdt.h>

constexpr bool enableSelfCalibrationInitialValue = true;

// MHZ19XI2C_register_t::enable_self_calibration
volatile bool enableSelfCalibration = enableSelfCalibrationInitialValue;

// MHZ19XI2C_register_t::latest_co2ppm
uint16_t latestCO2ppm = 0;

// MHZ19XI2C_register_t::interval
volatile uint8_t getCO2ConcentrationIntervalInSeconds = 5;

// MHZ19XI2C_register_t::error_code
auto getCO2ConcentrationResult = MHZ19X_error_t::none;

// If errors on measurement reading occur consecutively beyond the following count,
// a software reset using watchdog will be triggered as there could be
// an inconsistency in serial reading.
constexpr size_t MaxConsecutiveErrorCount = 10;

auto sensor = MHZ19C<PIN_PA6, PIN_PA7>();

// requested register address for reading
MHZ19XI2C_register_t i2cCommandRegisterAddress = (MHZ19XI2C_register_t)0;

void receiveEvent(int);
void requestEvent();

void setup()
{
  // enable 1-seconds watchdog timer to trigger a software reset if any operation hangs
  wdt_enable(WDTO_1S);

  sensor.begin();

  sensor.switchSelfCalibration(enableSelfCalibrationInitialValue);

  wdt_reset();

  Wire.begin(MHZ19XI2C_DEVICE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  wdt_reset();
}

void loop()
{
  static bool isSelfCalibrationEnabled = enableSelfCalibrationInitialValue;
  static size_t getCO2ConcentrationConsecutiveErrorCount = 0;

  wdt_reset();

  noInterrupts();

  auto intervalInMilliseconds = getCO2ConcentrationIntervalInSeconds * 1000;
  auto switchSelfCalibrationTo = enableSelfCalibration;

  interrupts();

  if (intervalInMilliseconds == 0)
    return; // disable retrieving measurement value

  // apply changes to the value of isSelfCalibrationEnabled
  if (isSelfCalibrationEnabled != switchSelfCalibrationTo) {
    sensor.switchSelfCalibration(switchSelfCalibrationTo);

    isSelfCalibrationEnabled = switchSelfCalibrationTo;
  }

  delay(intervalInMilliseconds);

  wdt_reset();

  // retrieve the latest measurement value from sensor
  uint16_t co2ppm = 0;

  getCO2ConcentrationResult = sensor.getCO2Concentration(co2ppm);

  if (MHZ19X_error_t::success == getCO2ConcentrationResult) {
    getCO2ConcentrationConsecutiveErrorCount = 0; // reset the number of consecutive errors
    latestCO2ppm = co2ppm; // set the latest measurement value
  }
  else {
    getCO2ConcentrationConsecutiveErrorCount++; // increment the number of consecutive errors
    latestCO2ppm = 0;
  }

  if (MaxConsecutiveErrorCount <= getCO2ConcentrationConsecutiveErrorCount) {
    // trigger a software reset using watchdog timer
    wdt_enable(WDTO_15MS);
    delay(10000);
  }
}

void receiveEvent(int numberOfBytes)
{
  if (numberOfBytes < 1)
    return; // too short

  switch ((MHZ19XI2C_command_t)Wire.read()) { // switch by requested command
    case MHZ19XI2C_command_t::read:
      if (numberOfBytes < 2)
        return; // too short

      i2cCommandRegisterAddress = (MHZ19XI2C_register_t)Wire.read();

      break;

    case MHZ19XI2C_command_t::write:
      if (numberOfBytes < 2)
        return; // too short

      switch ((MHZ19XI2C_register_t)Wire.read()) { // switch by requested register address
        case MHZ19XI2C_register_t::enable_self_calibration:
          if (numberOfBytes < 3)
            return; // too short

          // false for 0, otherwise true
          enableSelfCalibration = (Wire.read() != 0);

          break;

        case MHZ19XI2C_register_t::interval:
          if (numberOfBytes < 3)
            return; // too short

          getCO2ConcentrationIntervalInSeconds = Wire.read();

          break;
      }

      break;

    default:
      break; // ignore unknown command
  }
}

void requestEvent()
{
  switch (i2cCommandRegisterAddress) {
    case MHZ19XI2C_register_t::enable_self_calibration:
      Wire.write((uint8_t)(enableSelfCalibration ? 1 : 0));
      break;

    case MHZ19XI2C_register_t::latest_co2ppm:
      Wire.write((uint8_t)((latestCO2ppm & 0xFF00u) >> 8));
      Wire.write((uint8_t)(latestCO2ppm & 0x00FFu));
      break;

    case MHZ19XI2C_register_t::interval:
      Wire.write((uint8_t)getCO2ConcentrationIntervalInSeconds);
      break;

    case MHZ19XI2C_register_t::error_code:
      Wire.write((uint8_t)getCO2ConcentrationResult);
      break;

    default:
      break; // ignore unknown register
  }
}
