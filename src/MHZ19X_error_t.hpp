// SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
// SPDX-License-Identifier: MIT

#ifndef MHZ19X_error_t_hpp
#define MHZ19X_error_t_hpp

enum class MHZ19X_error_t {
  // Represents a value that is neither success nor failure.
  none = -1,

  // Represents that the operation was successfully completed.
  success = 0,

  uart_write_failure,
  uart_read_timeout,
  uart_read_failure,

  returnvalue_startbyte_mismatch,
  returnvalue_commandbyte_mismatch,
  returnvalue_checksum_mismatch,
};

#endif // MHZ19X_error_t_hpp
