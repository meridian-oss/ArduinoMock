#include "SPI.h"

#include <iostream>
#include <iomanip>
#include <cstring>

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------
void SPIClass::begin() {
    std::cout << "[SPI] begin" << std::endl;
}

void SPIClass::end() {
    std::cout << "[SPI] end" << std::endl;
}

// ---------------------------------------------------------------------------
// Transaction
// ---------------------------------------------------------------------------
void SPIClass::beginTransaction(SPISettings settings) {
    std::cout << "[SPI] beginTransaction clock=" << settings.clock()
              << " bitOrder=" << static_cast<int>(settings.bitOrder())
              << " dataMode=" << static_cast<int>(settings.dataMode())
              << std::endl;
}

void SPIClass::endTransaction() {
    std::cout << "[SPI] endTransaction" << std::endl;
}

// ---------------------------------------------------------------------------
// Transfer
// ---------------------------------------------------------------------------
uint8_t SPIClass::transfer(uint8_t data) {
    std::cout << "[SPI] transfer tx=0x"
              << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
              << static_cast<int>(data) << std::dec
              << " rx=0xFF" << std::endl;
    return 0xFF;
}

uint16_t SPIClass::transfer16(uint16_t data) {
    std::cout << "[SPI] transfer16 tx=0x"
              << std::hex << std::uppercase << std::setw(4) << std::setfill('0')
              << data << std::dec
              << " rx=0xFFFF" << std::endl;
    return 0xFFFF;
}

void SPIClass::transfer(void *buf, size_t count) {
    std::cout << "[SPI] transfer buf count=" << count << std::endl;
    memset(buf, 0xFF, count);
}

// ---------------------------------------------------------------------------
// Legacy clock / mode setters
// ---------------------------------------------------------------------------
void SPIClass::setBitOrder(uint8_t bitOrder) {
    std::cout << "[SPI] setBitOrder " << static_cast<int>(bitOrder) << std::endl;
}

void SPIClass::setDataMode(uint8_t dataMode) {
    std::cout << "[SPI] setDataMode " << static_cast<int>(dataMode) << std::endl;
}

void SPIClass::setClockDivider(uint8_t divider) {
    std::cout << "[SPI] setClockDivider " << static_cast<int>(divider) << std::endl;
}

// ---------------------------------------------------------------------------
// Global instance
// ---------------------------------------------------------------------------
SPIClass SPI;
