// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT

#ifndef MHZ19X_h
#define MHZ19X_h

#include <MHZ19XDriver.hpp>

//
// Baud9600SloppySoftwareSerialStream
//
#include <SloppySoftwareSerialStream.hpp>

template <
  int8_t PIN_RX,
  int8_t PIN_TX,
  signed DELAY_FOR_TUNE = 0
>
using Baud9600SloppySoftwareSerialStream = SloppySoftwareSerialStream<
  PIN_RX,
  PIN_TX,
  104 + DELAY_FOR_TUNE, // delay for TX bits: 9600 bps = approx 104 micro secs per bit
  104 + DELAY_FOR_TUNE, // delay for RX data bits: 9600 bps = approx 104 micro secs per bit
  (104 + DELAY_FOR_TUNE) / 3, // delay for RX start bit: delay by 1/3 of a bit width to read from 1/3 of the data bit waveform
  (104 + DELAY_FOR_TUNE) * 2 / 5 // delay for RX stop bit: 2/5 of a bit width
>;

//
// espressif32 + esp32dev
//
#if defined(ARDUINO_ARCH_ESP32)
#include <ESP32HardwareSerialStream.hpp>

// MHZ19C + UART1/2
using MHZ19C_UART1 = MHZ19XDriver<ESP32HardwareSerialStream<1, 9, 10>>;
using MHZ19C_UART2 = MHZ19XDriver<ESP32HardwareSerialStream<2, 16, 17>>;
#endif // ARDUINO_ARCH_ESP32

//
// atmelavr + ATmega328 (Arduino-compatible boards)
//
#if defined (__AVR_ATmega328P__)
#include <DefaultSerialStream.hpp>

// MHZ19C
using MHZ19C = MHZ19XDriver<DefaultSerialStream>;
#endif

//
// atmelmegaavr + ATtinyX02 (202/402)
// atmelmegaavr + ATtinyX04 (204/404/804/1604)
//
#if defined(__AVR_ATtinyx02__) || defined(__AVR_ATtinyx04__)
template <
  int8_t PIN_RX,
  int8_t PIN_TX
>
using TunedBaud9600SloppySoftwareSerialStream = Baud9600SloppySoftwareSerialStream <
  PIN_RX,
  PIN_TX,
#if (F_CPU == 20000000) // 20 MHz
  0
#elif (F_CPU == 16000000) // 16 MHz
  20
#elif (F_CPU == 10000000) // 10 MHz
  - 8
#elif (F_CPU == 8000000) // 8 MHz
  16
#elif (F_CPU == 5000000) // 5 MHz
  - 16
#elif (F_CPU == 4000000) // 4 MHz
  4
#elif (F_CPU == 2000000) // 2 MHz
  0
#elif (F_CPU == 1000000) // 1 MHz
  0
#else
#error "unsupported F_CPU"
#endif // F_CPU == ***
> ;

// MHZ19C
template <
  int8_t PIN_RX,
  int8_t PIN_TX
>
using MHZ19C = MHZ19XDriver<TunedBaud9600SloppySoftwareSerialStream<PIN_RX, PIN_TX>>;
#endif // __AVR_ATtinyx02__

#endif // MHZ19X_h
