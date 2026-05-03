// System headers MUST come before Arduino.h to prevent its min/max/abs/round
// macros from conflicting with C++ standard library templates.
#include <chrono>
#include <thread>
#include <iostream>

#include "Arduino.h"

// ---------------------------------------------------------------------------
// Internal pin state storage
// ---------------------------------------------------------------------------
static const uint8_t MAX_PINS = 128;

static uint8_t _pinModes[MAX_PINS]   = {};
static uint8_t _digitalValues[MAX_PINS] = {};
static int     _analogValues[MAX_PINS]  = {};

// ---------------------------------------------------------------------------
// Start-time reference for millis() / micros()
// ---------------------------------------------------------------------------
static const auto _startTime = std::chrono::steady_clock::now();

// ---------------------------------------------------------------------------
// Digital I/O
// ---------------------------------------------------------------------------
void pinMode(uint8_t pin, uint8_t mode) {
    if (pin < MAX_PINS) {
        _pinModes[pin] = mode;
    }
}

void digitalWrite(uint8_t pin, uint8_t val) {
    if (pin < MAX_PINS) {
        _digitalValues[pin] = val;
    }
}

int digitalRead(uint8_t pin) {
    if (pin < MAX_PINS) {
        return _digitalValues[pin];
    }
    return LOW;
}

// ---------------------------------------------------------------------------
// Analog I/O
// ---------------------------------------------------------------------------
int analogRead(uint8_t pin) {
    if (pin < MAX_PINS) {
        return _analogValues[pin];
    }
    return 0;
}

void analogReference(uint8_t /*mode*/) {
    // No-op on native
}

void analogWrite(uint8_t pin, int val) {
    if (pin < MAX_PINS) {
        _analogValues[pin] = val;
    }
}

// ---------------------------------------------------------------------------
// Advanced I/O
// ---------------------------------------------------------------------------
void tone(uint8_t pin, unsigned int frequency, unsigned long duration) {
    std::cout << "[tone] pin=" << static_cast<int>(pin)
              << " freq=" << frequency << "Hz"
              << " duration=" << duration << "ms" << std::endl;
}

void noTone(uint8_t pin) {
    std::cout << "[noTone] pin=" << static_cast<int>(pin) << std::endl;
}

uint8_t shiftIn(uint8_t /*dataPin*/, uint8_t /*clockPin*/, uint8_t /*bitOrder*/) {
    return 0;
}

void shiftOut(uint8_t /*dataPin*/, uint8_t /*clockPin*/,
              uint8_t /*bitOrder*/, uint8_t /*val*/) {
    // No-op on native
}

unsigned long pulseIn(uint8_t /*pin*/, uint8_t /*state*/, unsigned long /*timeout*/) {
    return 0;
}

unsigned long pulseInLong(uint8_t /*pin*/, uint8_t /*state*/, unsigned long /*timeout*/) {
    return 0;
}

// ---------------------------------------------------------------------------
// Time
// ---------------------------------------------------------------------------
unsigned long millis() {
    auto now = std::chrono::steady_clock::now();
    return static_cast<unsigned long>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now - _startTime).count());
}

unsigned long micros() {
    auto now = std::chrono::steady_clock::now();
    return static_cast<unsigned long>(
        std::chrono::duration_cast<std::chrono::microseconds>(now - _startTime).count());
}

void delay(unsigned long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void delayMicroseconds(unsigned int us) {
    std::this_thread::sleep_for(std::chrono::microseconds(us));
}

// ---------------------------------------------------------------------------
// Math
// ---------------------------------------------------------------------------
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// ---------------------------------------------------------------------------
// Random numbers
// ---------------------------------------------------------------------------
void randomSeed(unsigned long seed) {
    srand(static_cast<unsigned int>(seed));
}

long random(long howbig) {
    if (howbig == 0) return 0;
    return static_cast<long>(rand() % howbig);
}

long random(long howsmall, long howbig) {
    if (howsmall >= howbig) return howsmall;
    return howsmall + random(howbig - howsmall);
}

// ---------------------------------------------------------------------------
// External interrupts  (stubs – no hardware on native)
// ---------------------------------------------------------------------------
void attachInterrupt(uint8_t /*interrupt*/, voidFuncPtr /*ISR*/, uint8_t /*mode*/) {}
void detachInterrupt(uint8_t /*interrupt*/) {}
void interrupts() {}
void noInterrupts() {}

int digitalPinToInterrupt(uint8_t pin) {
    return static_cast<int>(pin);
}

// ---------------------------------------------------------------------------
// Test helpers
// ---------------------------------------------------------------------------
void setDigitalValue(uint8_t pin, uint8_t value) {
    if (pin < MAX_PINS) {
        _digitalValues[pin] = value;
    }
}

void setAnalogValue(uint8_t pin, int value) {
    if (pin < MAX_PINS) {
        _analogValues[pin] = value;
    }
}
