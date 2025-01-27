// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT

#ifndef MHZ19XBase_hpp
#define MHZ19XBase_hpp

#include <stdint.h> // intN_t, uintN_t
#include <stddef.h> // size_t

class MHZ19XBase {
protected:
  static constexpr size_t COMMAND_LENGTH = 9;
  static constexpr size_t RETURNVALUE_LENGTH = 9;

  MHZ19XBase()
  {
  }

  static uint8_t computeChecksum(const uint8_t* const packet)
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
};

#endif // MHZ19XBase_hpp
