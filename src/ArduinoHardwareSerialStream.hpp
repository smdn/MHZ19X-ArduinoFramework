// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT

#ifndef ArduinoHardwareSerialStream_hpp
#define ArduinoHardwareSerialStream_hpp

#include <HardwareSerial.h>

class ArduinoHardwareSerialStream {
public:
  ArduinoHardwareSerialStream(const HardwareSerial* serial)
    : serial(serial)
  {
  }

  void begin(
    const uint16_t& baud,
    const uint16_t& configurations
  ) /* const */
  {
    if (!serial)
      return;

    serial->begin(baud, configurations);
    serial->setTimeout(1000); // TODO: make configurable
  }

  bool writeBytes(
    const uint8_t* /*const*/ data,
    const size_t& length
  ) /* const */
  {
    if (!serial)
      return false;

    if (length != serial->write(data, length))
      return false;

    serial->flush();

    return true;
  }

  size_t readBytes(
    uint8_t* /*const*/ buffer,
    const size_t& length
  ) /* const */
  {
    if (!serial)
      return 0;

    return serial->readBytes(buffer, length);
  }

private:
  const HardwareSerial* serial;
};

#endif // ArduinoHardwareSerialStream_hpp
