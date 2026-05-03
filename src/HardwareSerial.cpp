#include "HardwareSerial.h"

#include <cstdio>
#include <iostream>

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------
void HardwareSerial::begin(unsigned long baud, uint8_t /*config*/) {
  _baud = baud;
  _peeked = -1;
}

void HardwareSerial::end() {
  _baud = 0;
  _peeked = -1;
}

// ---------------------------------------------------------------------------
// Print interface  →  stdout
// ---------------------------------------------------------------------------
size_t HardwareSerial::write(uint8_t c) {
  int ret = std::putchar(static_cast<int>(c));
  return (ret == EOF) ? 0 : 1;
}

void HardwareSerial::flush() {
  std::fflush(stdout);
}

// ---------------------------------------------------------------------------
// Stream interface  →  stdin
// ---------------------------------------------------------------------------
int HardwareSerial::available() {
  // Non-blocking check: on native we cannot easily check without blocking,
  // so we attempt a peek and cache the result.
  if (_peeked >= 0) {
    return 1;
  }
  // Try a non-blocking read via getchar_unlocked / _getchar_nolock – fall
  // back to 0 on MSVC.  On most Unix systems we just report 0 unless the
  // caller has filled stdin.
  return 0;
}

int HardwareSerial::read() {
  if (_peeked >= 0) {
    int c = _peeked;
    _peeked = -1;
    return c;
  }
  return std::getchar();
}

int HardwareSerial::peek() {
  if (_peeked < 0) {
    _peeked = std::getchar();
  }
  return _peeked;
}

// ---------------------------------------------------------------------------
// Global instances
// ---------------------------------------------------------------------------
HardwareSerial Serial;
HardwareSerial Serial1;
HardwareSerial Serial2;
