// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT

#ifndef SloppySoftwareSerialStream_hpp
#define SloppySoftwareSerialStream_hpp

#include <Arduino.h>

// The *sloppy* software serial implementation using with digitalRead(), digitalWrite() and delayMicroseconds().
template <
  int8_t PIN_RX,
  int8_t PIN_TX,
  unsigned TX_DELAY_MICROSECS,
  unsigned RX_DATA_DELAY_MICROSECS,
  unsigned RX_START_DELAY_MICROSECS,
  unsigned RX_STOP_DELAY_MICROSECS
>
class SloppySoftwareSerialStream {
private:
  using self_t = SloppySoftwareSerialStream<
    PIN_RX,
    PIN_TX,
    TX_DELAY_MICROSECS,
    RX_DATA_DELAY_MICROSECS,
    RX_START_DELAY_MICROSECS,
    RX_STOP_DELAY_MICROSECS
  >;
  using index_t = uint8_t;

public:
  SloppySoftwareSerialStream()
  {
  }

  void begin(
    const uint16_t& baud,
    const uint16_t& configurations
  ) /* const */
  {
    pinMode(PIN_TX, OUTPUT);
    digitalWrite(PIN_TX, HIGH);

    pinMode(PIN_RX, INPUT);
    attachInterrupt(PIN_RX, self_t::handleReceive, CHANGE);

    attachedInstance = this;
    receiveBufferIndexForRead = 0;
    receiveBufferIndexForWrite = 0;
  }

  bool writeBytes(
    const uint8_t* /*const*/ data,
    const size_t& length
  ) /* const */
  {
    for (auto index = 0; index < length; index++) {
      // start bit
      digitalWrite(PIN_TX, LOW);
      delayMicroseconds(TX_DELAY_MICROSECS);

      auto d = data[index];

      // data bits
      for (auto i = 0; i < dataSizeInBits; i++) {
        digitalWrite(PIN_TX, d & 0b1 ? HIGH : LOW);
        delayMicroseconds(TX_DELAY_MICROSECS);

        d >>= 1;
      }

      // stop bit
      digitalWrite(PIN_TX, HIGH);
      delayMicroseconds(TX_DELAY_MICROSECS);
    }

    return true;
  }

  size_t readBytes(
    uint8_t* /*const*/ buffer,
    const size_t& length
  ) /* const */
  {
    size_t count = 0;
    uint8_t* buf = buffer;

    for (;;) {
      if (length <= count)
        break;

      auto b = readWithTimeout();

      if (b < 0)
        break;

      *buf++ = (uint8_t)b;

      count++;
    }

    return count;
  }

private:
  static constexpr size_t dataSizeInBits = 8; // size of data bits
  static constexpr size_t receiveBufferSize = 9 * 2; // = return value length * 2

  static self_t* attachedInstance;

  uint8_t receiveBuffer[receiveBufferSize];
  volatile index_t receiveBufferIndexForRead;
  volatile index_t receiveBufferIndexForWrite;

  static inline void handleReceive()
  {
    if (!attachedInstance)
      return;

    // disable interrupt while receiving
    noInterrupts();

    attachedInstance->receive();

    // re-enable interrupt
    interrupts();
  }

  void receive()
  {
    // expect the start bit
    if (digitalRead(PIN_RX))
      return; // not a start bit

    delayMicroseconds(RX_START_DELAY_MICROSECS);

    // read data bits
    uint8_t data = 0x00;

    for (size_t i = 0; i < dataSizeInBits; i++) {
      delayMicroseconds(RX_DATA_DELAY_MICROSECS);

      data >>= 1;

      if (digitalRead(PIN_RX))
        data |= 0b10000000;
    }

    // store to the buffer
    auto nextIndex = (index_t)((receiveBufferIndexForWrite + 1) % receiveBufferSize);

    if (nextIndex == receiveBufferIndexForRead) {
      // buffer overflow, discard the received data
    }
    else {
      receiveBuffer[receiveBufferIndexForWrite] = data;
      receiveBufferIndexForWrite = nextIndex;
    }

    // expect the stop bit
    delayMicroseconds(RX_STOP_DELAY_MICROSECS);
  }

  int read()
  {
    if (receiveBufferIndexForRead == receiveBufferIndexForWrite)
      return -1; // buffer empty or overflow

    index_t index = receiveBufferIndexForRead;

    receiveBufferIndexForRead = (index_t)((receiveBufferIndexForRead + 1) % receiveBufferSize);

    return receiveBuffer[index];
  }

  int readWithTimeout()
  {
    constexpr unsigned int timeoutMilliseconds = 500;
    unsigned int startAt = millis();
    unsigned int timeoutAt = startAt + timeoutMilliseconds;

    for (;;) {
      auto b = read();

      if (0 <= b)
        return b;

      unsigned int now = millis();

      if (now < startAt)
        return -1; // counter rewound
      if (timeoutAt <= now)
        return -1; // timed out
    }
  }
};

template <
  int8_t PIN_RX,
  int8_t PIN_TX,
  unsigned TX_DELAY_MICROSECS,
  unsigned RX_DATA_DELAY_MICROSECS,
  unsigned RX_START_DELAY_MICROSECS,
  unsigned RX_STOP_DELAY_MICROSECS
>
SloppySoftwareSerialStream<
  PIN_RX,
  PIN_TX,
  TX_DELAY_MICROSECS,
  RX_DATA_DELAY_MICROSECS,
  RX_START_DELAY_MICROSECS,
  RX_STOP_DELAY_MICROSECS
>*
SloppySoftwareSerialStream<
  PIN_RX,
  PIN_TX,
  TX_DELAY_MICROSECS,
  RX_DATA_DELAY_MICROSECS,
  RX_START_DELAY_MICROSECS,
  RX_STOP_DELAY_MICROSECS
>::attachedInstance = nullptr;

#endif // SloppySoftwareSerialStream_hpp
