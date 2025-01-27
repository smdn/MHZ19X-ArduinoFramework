// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT

#ifndef MHZ19XPseudoModuleStream_hpp
#define MHZ19XPseudoModuleStream_hpp

#include "MHZ19XBase.hpp"

class MHZ19XPseudoModuleStream : MHZ19XBase {
public:
  MHZ19XPseudoModuleStream() :
    command(0),
    co2ConcentrationInPpm(0)
  {
  }

  void setCO2Concentration(const uint16_t& co2ConcentrationInPpm)
  {
    this->co2ConcentrationInPpm = co2ConcentrationInPpm;
  }

  void begin(
    const uint16_t& baud,
    const uint16_t& configurations
  ) /* const */
  {
    // do nothing
  }

  bool writeBytes(
    const uint8_t* /*const*/ data,
    const size_t& length
  ) /* const */
  {
    if (length < COMMAND_LENGTH)
      return false; // command too short

    if (data[0] == 0xFF)
      command = data[2]; // store the requested command (skip checksum validation)
    else
      command = 0xFF;

    return true;
  }

  size_t readBytes(
    uint8_t* /*const*/ buffer,
    const size_t& length
  ) /* const */
  {
    switch (command) {
      // 0x86: Read CO2 concentration
      case 0x86:
      {
        if (length < RETURNVALUE_LENGTH)
          return 0; // buffer too short

        auto high = (uint8_t)(co2ConcentrationInPpm >> 8);
        auto low = (uint8_t)(co2ConcentrationInPpm & 0xFF);

        buffer[0] = 0xFF; // Start Byte
        buffer[1] = command; // Command
        buffer[2] = high; // HIGH
        buffer[3] = low; // LOW
        buffer[4] = 0x00;
        buffer[5] = 0x00;
        buffer[6] = 0x00;
        buffer[7] = 0x00;
        buffer[8] = computeChecksum(buffer); // checksum

        return RETURNVALUE_LENGTH;
      }

      default:
        return 0; // no return value for this command
    }
  }

private:
  uint8_t command;
  uint16_t co2ConcentrationInPpm;
};

#endif // MHZ19XPseudoModuleStream_hpp
