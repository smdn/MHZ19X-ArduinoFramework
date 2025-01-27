// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT

#ifndef DefaultSerialStream_hpp
#define DefaultSerialStream_hpp

#include <Arduino.h>

//
// The default implementation for serial communication using with
// `Serial` class provided from Arduino framework.
//
// ref: https://docs.arduino.cc/language-reference/en/functions/communication/serial/
//
class DefaultSerialStream {
public:
  DefaultSerialStream()
  {
  }

  void begin(
    const uint16_t& baud,
    const uint16_t& configurations
  ) /* const */
  {
    Serial.begin(baud, configurations);
    Serial.setTimeout(1000); // TODO: make configurable
  }

  bool writeBytes(
    const uint8_t* /*const*/ data,
    const size_t& length
  ) /* const */
  {
    if (length != Serial.write(data, length))
      return false;

    Serial.flush();

    return true;
  }

  size_t readBytes(
    uint8_t* /*const*/ buffer,
    const size_t& length
  ) /* const */
  {
    return Serial.readBytes(buffer, length);
  }
};

#endif // DefaultSerialStream_hpp
