// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT

#ifndef MHZ19XDriver_hpp
#define MHZ19XDriver_hpp

#include <Arduino.h>
#include "MHZ19X_error_t.hpp"

template <typename TUartStream>
class MHZ19XDriver {
public:
  MHZ19XDriver()
    : stream()
  {
  }

  void begin();

  MHZ19X_error_t getCO2Concentration(uint16_t& co2ConcentrationInPpm) /*const*/;
  MHZ19X_error_t switchSelfCalibration(const bool& enableAutoCalibration) /*const*/;

protected:
  static constexpr size_t COMMAND_LENGTH = 9;
  static constexpr size_t RETURNVALUE_LENGTH = 9;

  MHZ19X_error_t sendCommand(const uint8_t* /*const*/ command, uint8_t* returnValue) /*const*/;
  uint8_t computeChecksum(const uint8_t* const packet) const;

private:
  using module_t = MHZ19XDriver<TUartStream>;

  TUartStream stream;
};

template <typename TUartStream>
void MHZ19XDriver<TUartStream>::begin()
{
  // baud rate: 9600
  constexpr uint16_t baud = 9600u;

  // data bit: 8 bytes
  // stop bit: 1 byte
  // parity bit: none
  constexpr uint16_t configurations = (uint16_t)SERIAL_8N1;

  stream.begin(baud, configurations);
}

template <typename TUartStream>
MHZ19X_error_t MHZ19XDriver<TUartStream>::getCO2Concentration(
  uint16_t& co2ConcentrationInPpm
) /*const*/
{
  co2ConcentrationInPpm = 0;

  // 0x86: Read CO2 concentration
  constexpr uint8_t startByte = 0xFF;
  constexpr uint8_t commandByte = 0x86;
  uint8_t commandReadCo2Concentration[module_t::COMMAND_LENGTH] = {
    startByte, // Start Byte
    0x01, // Reserved
    commandByte, // Command
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x79, // Checksum
  };
  uint8_t returnValue[module_t::RETURNVALUE_LENGTH] = { 0 };

  auto result = sendCommand(commandReadCo2Concentration, returnValue);

  if (MHZ19X_error_t::success != result)
    return result; // send or receive failed

  if (returnValue[0] != startByte) // Start Byte
    return MHZ19X_error_t::returnvalue_startbyte_mismatch;

  if (returnValue[1] != commandByte) // Command
    return MHZ19X_error_t::returnvalue_commandbyte_mismatch;

  if (returnValue[module_t::RETURNVALUE_LENGTH - 1] != computeChecksum(returnValue)) // Checksum
    return MHZ19X_error_t::returnvalue_checksum_mismatch;

  // CO2 concentration = HIGH * 256 + LOW
  co2ConcentrationInPpm =
    returnValue[2] << 8 | // HIGH
    returnValue[3]; // LOW

  return MHZ19X_error_t::success;
}

template <typename TUartStream>
MHZ19X_error_t MHZ19XDriver<TUartStream>::switchSelfCalibration(
  const bool& enableAutoCalibration
) /*const*/
{
  // 0x79: On/Off Self-calibration for Zero Point
  uint8_t commandOnOffSelfCalibration[module_t::COMMAND_LENGTH] = {
    0xFF, // Start Byte
    0x01, // Reserved
    0x79, // Command
    (uint8_t)(enableAutoCalibration ? 0xA0 : 0x00), // on/off auto calibration function
    0x00,
    0x00,
    0x00,
    0x00,
    0xCD, // Checksum (placeholder)
  };

  commandOnOffSelfCalibration[module_t::COMMAND_LENGTH - 1] = computeChecksum(commandOnOffSelfCalibration);

  return sendCommand(commandOnOffSelfCalibration, nullptr); // no return value
}

template <typename TUartStream>
MHZ19X_error_t MHZ19XDriver<TUartStream>::sendCommand(
  const uint8_t* command,
  uint8_t* returnValue
) /*const*/
{
  if (!stream.writeBytes(command, module_t::COMMAND_LENGTH))
    return MHZ19X_error_t::uart_write_failure;

  if (!returnValue)
    return MHZ19X_error_t::success; // send only (no return value)

  auto len = stream.readBytes(returnValue, module_t::RETURNVALUE_LENGTH);

  if (len == 0)
    return MHZ19X_error_t::uart_read_timeout;
  else if (len != module_t::RETURNVALUE_LENGTH)
    return MHZ19X_error_t::uart_read_failure;

  return MHZ19X_error_t::success;
}

template <typename TUartStream>
uint8_t MHZ19XDriver<TUartStream>::computeChecksum(
  const uint8_t* const packet
) const
{
  uint8_t checksum = 0u;

  // 1. Add Byte 1 to Byte 7
  for (auto i = 1; i <= 7; i++) {
    checksum += packet[i];
  }

  // 2. Negative
  checksum = 0xFFu - checksum;

  // 3. Then+1
  checksum += 1u;

  return checksum;
}

#endif // MHZ19XDriver_hpp
