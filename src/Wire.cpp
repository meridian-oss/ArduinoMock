#include "Wire.h"

#include <iomanip>
#include <iostream>

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
TwoWire::TwoWire() : _address(0), _txAddress(0) {}

// ---------------------------------------------------------------------------
// Initialisation
// ---------------------------------------------------------------------------
void TwoWire::begin() {
  std::cout << "[Wire] begin (master)" << std::endl;
}

void TwoWire::begin(uint8_t address) {
  _address = address;
  std::cout << "[Wire] begin slave address=0x"
            << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << static_cast<int>(address) << std::dec << std::endl;
}

void TwoWire::begin(int address) {
  begin(static_cast<uint8_t>(address));
}

void TwoWire::end() {
  std::cout << "[Wire] end" << std::endl;
}

void TwoWire::setClock(uint32_t clock) {
  std::cout << "[Wire] setClock " << clock << " Hz" << std::endl;
}

// ---------------------------------------------------------------------------
// Master write
// ---------------------------------------------------------------------------
void TwoWire::beginTransmission(uint8_t address) {
  _txAddress = address;
  std::cout << "[Wire] beginTransmission address=0x"
            << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << static_cast<int>(address) << std::dec << std::endl;
}

void TwoWire::beginTransmission(int address) {
  beginTransmission(static_cast<uint8_t>(address));
}

uint8_t TwoWire::endTransmission(bool sendStop) {
  std::cout << "[Wire] endTransmission sendStop=" << sendStop << std::endl;
  return I2C_ERROR_NONE;
}

// ---------------------------------------------------------------------------
// Master read
// ---------------------------------------------------------------------------
uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, bool sendStop) {
  std::cout << "[Wire] requestFrom address=0x"
            << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << static_cast<int>(address) << std::dec
            << " quantity=" << static_cast<int>(quantity)
            << " sendStop=" << sendStop << std::endl;
  return 0; // no data available in mock
}

uint8_t TwoWire::requestFrom(int address, int quantity, bool sendStop) {
  return requestFrom(static_cast<uint8_t>(address),
                     static_cast<uint8_t>(quantity),
                     sendStop);
}

// ---------------------------------------------------------------------------
// Stream write
// ---------------------------------------------------------------------------
size_t TwoWire::write(uint8_t data) {
  std::cout << "[Wire] write 0x"
            << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << static_cast<int>(data) << std::dec << std::endl;
  return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t length) {
  std::cout << "[Wire] write " << length << " bytes:" << std::hex << std::uppercase;
  for (size_t i = 0; i < length; i++) {
    std::cout << " 0x" << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
  }
  std::cout << std::dec << std::endl;
  return length;
}

// ---------------------------------------------------------------------------
// Stream read (returns 0 bytes – no hardware on native)
// ---------------------------------------------------------------------------
int TwoWire::available() { return 0; }
int TwoWire::read() { return -1; }
int TwoWire::peek() { return -1; }
void TwoWire::flush() {}

// ---------------------------------------------------------------------------
// Slave callbacks (registered but never invoked on native)
// ---------------------------------------------------------------------------
void TwoWire::onReceive(void (*function)(int)) {
  std::cout << "[Wire] onReceive registered" << std::endl;
  (void)function;
}

void TwoWire::onRequest(void (*function)()) {
  std::cout << "[Wire] onRequest registered" << std::endl;
  (void)function;
}

// ---------------------------------------------------------------------------
// Global instances
// ---------------------------------------------------------------------------
TwoWire Wire;
TwoWire Wire1;
