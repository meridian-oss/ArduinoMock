#pragma once
#ifndef SPI_h
#define SPI_h

#include <stdint.h>
#include <stddef.h>

// ---------------------------------------------------------------------------
// SPI data-mode constants
// ---------------------------------------------------------------------------
#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

// ---------------------------------------------------------------------------
// SPI clock-divider constants (kept for source compatibility; ignored on native)
// ---------------------------------------------------------------------------
#define SPI_CLOCK_DIV2   0x00
#define SPI_CLOCK_DIV4   0x01
#define SPI_CLOCK_DIV8   0x02
#define SPI_CLOCK_DIV16  0x03
#define SPI_CLOCK_DIV32  0x04
#define SPI_CLOCK_DIV64  0x05
#define SPI_CLOCK_DIV128 0x06

// ---------------------------------------------------------------------------
// SPISettings – groups clock speed, bit order and data mode together.
// ---------------------------------------------------------------------------
class SPISettings {
public:
    SPISettings() : _clock(4000000), _bitOrder(1 /*MSBFIRST*/), _dataMode(SPI_MODE0) {}
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
        : _clock(clock), _bitOrder(bitOrder), _dataMode(dataMode) {}

    uint32_t clock()    const { return _clock; }
    uint8_t  bitOrder() const { return _bitOrder; }
    uint8_t  dataMode() const { return _dataMode; }

private:
    uint32_t _clock;
    uint8_t  _bitOrder;
    uint8_t  _dataMode;
};

// ---------------------------------------------------------------------------
// SPIClass – logs operations to stdout; transfer() returns 0xFF.
// ---------------------------------------------------------------------------
class SPIClass {
public:
    void begin();
    void end();

    void beginTransaction(SPISettings settings);
    void endTransaction();

    // Single-byte transfer
    uint8_t  transfer(uint8_t data);
    // 16-bit transfer
    uint16_t transfer16(uint16_t data);
    // Buffer transfer (in-place)
    void     transfer(void *buf, size_t count);

    // Legacy clock-divider API
    void setBitOrder(uint8_t bitOrder);
    void setDataMode(uint8_t dataMode);
    void setClockDivider(uint8_t divider);
};

extern SPIClass SPI;

#endif // SPI_h
