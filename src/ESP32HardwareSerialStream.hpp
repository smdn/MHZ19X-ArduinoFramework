// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT

#ifndef ESP32HardwareSerialStream_hpp
#define ESP32HardwareSerialStream_hpp

#include <HardwareSerial.h>

template <
  int UART_NR,
  int8_t PIN_RX,
  int8_t PIN_TX
>
class ESP32HardwareSerialStream {
public:
  ESP32HardwareSerialStream()
    : serial(UART_NR)
  {
  }

  void begin(
    const uint16_t& baud,
    const uint16_t& configurations
  ) /* const */
  {
    serial.begin(baud, configurations, PIN_RX, PIN_TX);
    serial.setTimeout(5000); // TODO: make configurable
  }

  bool writeBytes(
    const uint8_t* /*const*/ data,
    const size_t& length
  ) /* const */
  {
    if (length != serial.write(data, length))
      return false;

    serial.flush();

    return true;
  }

  size_t readBytes(
    uint8_t* /*const*/ buffer,
    const size_t& length
  ) /* const */
  {
    return serial.readBytes(buffer, length);
  }

private:
  HardwareSerial serial;
};

#endif // ESP32HardwareSerialStream_hpp
