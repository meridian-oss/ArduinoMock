#pragma once
#ifndef HardwareSerial_h
#define HardwareSerial_h

#include "Stream.h"
#include <stdint.h>

// ---------------------------------------------------------------------------
// UART configuration bitmasks  (same names as real Arduino)
// ---------------------------------------------------------------------------
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06   // default
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36

// ---------------------------------------------------------------------------
// HardwareSerial – maps to stdout (print/write) and stdin (read/available).
// ---------------------------------------------------------------------------
class HardwareSerial : public Stream {
public:
    HardwareSerial() : _baud(0) {}

    // Lifecycle
    void begin(unsigned long baud, uint8_t config = SERIAL_8N1);
    void end();

    // Stream interface
    int  available() override;
    int  read()      override;
    int  peek()      override;
    void flush()     override;

    // Print interface
    size_t write(uint8_t c) override;
    using Print::write;   // pull in write(const char*) etc.

    // Convenience: true while active
    explicit operator bool() const { return _baud != 0; }

private:
    unsigned long _baud;
    int           _peeked = -1; // one-char lookahead for peek()
};

// Three hardware serial ports (match common Arduino boards)
extern HardwareSerial Serial;
extern HardwareSerial Serial1;
extern HardwareSerial Serial2;

#endif // HardwareSerial_h
