#pragma once
#ifndef Wire_h
#define Wire_h

#include "Stream.h"
#include <stddef.h>
#include <stdint.h>

// ---------------------------------------------------------------------------
// Wire constants (same as Arduino's TWI library)
// ---------------------------------------------------------------------------
#define WIRE_HAS_END      1
#define TWI_BUFFER_LENGTH 32

// endTransmission() return codes
#define I2C_ERROR_NONE      0
#define I2C_ERROR_ADDR_NACK 2
#define I2C_ERROR_DATA_NACK 3
#define I2C_ERROR_OTHER     4

// ---------------------------------------------------------------------------
// TwoWire – I2C mock that logs to stdout and returns 0 for reads.
// ---------------------------------------------------------------------------
class TwoWire : public Stream {
public:
  TwoWire();

  // Initialisation
  void begin();
  void begin(uint8_t address);
  void begin(int address);
  void end();
  void setClock(uint32_t clock);

  // Master write
  void beginTransmission(uint8_t address);
  void beginTransmission(int address);
  uint8_t endTransmission(bool sendStop = true);

  // Master read
  uint8_t requestFrom(uint8_t address, uint8_t quantity, bool sendStop = true);
  uint8_t requestFrom(int address, int quantity, bool sendStop = true);

  // Stream write
  size_t write(uint8_t data);
  size_t write(const uint8_t *data, size_t length);
  using Print::write;

  // Stream read (always returns 0 bytes on native)
  int available() override;
  int read() override;
  int peek() override;
  void flush() override;

  // Slave callbacks
  void onReceive(void (*function)(int));
  void onRequest(void (*function)());

private:
  uint8_t _address = 0;
  uint8_t _txAddress = 0;
};

extern TwoWire Wire;
extern TwoWire Wire1;

#endif // Wire_h
