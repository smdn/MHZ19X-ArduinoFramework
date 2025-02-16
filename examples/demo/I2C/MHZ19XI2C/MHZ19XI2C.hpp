// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT
#include <stdint.h>

#ifndef MHZ19XI2C_hpp
#define MHZ19XI2C_hpp

// this value can be configured to any I2C device address
constexpr uint8_t MHZ19XI2C_DEVICE_ADDRESS = 0x19;

enum class MHZ19XI2C_command_t : uint8_t {
  read = 0x00,
  write = 0x01,
};

enum class MHZ19XI2C_register_t : uint8_t {
  enable_self_calibration = 0x79, // read/write
  latest_co2ppm = 0x86, // read only
  interval = 0xF0, // read/write
  error_code = 0xFE, // read only
};

#endif // MHZ19XI2C_hpp
