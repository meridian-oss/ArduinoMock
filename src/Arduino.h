#pragma once
#ifndef Arduino_h
#define Arduino_h

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// ---------------------------------------------------------------------------
// Data types
// ---------------------------------------------------------------------------
typedef bool     boolean;
typedef uint8_t  byte;
typedef uint16_t word;

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------
#define HIGH  0x1
#define LOW   0x0

#define INPUT          0x0
#define OUTPUT         0x1
#define INPUT_PULLUP   0x2
#define INPUT_PULLDOWN 0x3

#ifndef PI
#define PI          3.1415926535897932384626433832795
#endif
#define HALF_PI     1.5707963267948966192313216916398
#define TWO_PI      6.283185307179586476925286766559
#define DEG_TO_RAD  0.017453292519943295769236907684886
#define RAD_TO_DEG  57.295779513082320876798154814105
#define EULER       2.718281828459045235360287471352

#define SERIAL   0x0
#define DISPLAY  0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE  1
#define FALLING 2
#define RISING  3

#define DEFAULT  1
#define EXTERNAL 0
#define INTERNAL 2

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

// NOT_AN_INTERRUPT is returned by digitalPinToInterrupt() for non-interrupt pins.
#define NOT_AN_INTERRUPT -1

// ---------------------------------------------------------------------------
// PROGMEM – no-ops on native platforms
// ---------------------------------------------------------------------------
#define PROGMEM
#define PSTR(s)   (s)
#ifndef F
#define F(string_literal) (string_literal)
#endif

#define pgm_read_byte(addr)  (*(const uint8_t  *)(addr))
#define pgm_read_word(addr)  (*(const uint16_t *)(addr))
#define pgm_read_dword(addr) (*(const uint32_t *)(addr))
#define pgm_read_float(addr) (*(const float    *)(addr))
#define pgm_read_ptr(addr)   (*(const void * const *)(addr))

#define pgm_read_byte_near(addr)  pgm_read_byte(addr)
#define pgm_read_word_near(addr)  pgm_read_word(addr)
#define pgm_read_dword_near(addr) pgm_read_dword(addr)
#define pgm_read_float_near(addr) pgm_read_float(addr)
#define pgm_read_ptr_near(addr)   pgm_read_ptr(addr)

#define strlen_P  strlen
#define strcpy_P  strcpy
#define strcat_P  strcat
#define strcmp_P  strcmp
#define strncmp_P strncmp
#define memcpy_P  memcpy
#define sprintf_P sprintf

// ---------------------------------------------------------------------------
// Math macros
// ---------------------------------------------------------------------------
#undef min
#undef max
#undef abs
#undef round

#define min(a, b)               ((a) < (b) ? (a) : (b))
#define max(a, b)               ((a) > (b) ? (a) : (b))
#define abs(x)                  ((x) > 0 ? (x) : -(x))
#define constrain(amt, lo, hi)  ((amt) < (lo) ? (lo) : ((amt) > (hi) ? (hi) : (amt)))
#define round(x)                ((x) >= 0 ? (long)((x) + 0.5) : (long)((x) - 0.5))
#define radians(deg)            ((deg) * DEG_TO_RAD)
#define degrees(rad)            ((rad) * RAD_TO_DEG)
#define sq(x)                   ((x) * (x))

// ---------------------------------------------------------------------------
// Bit / byte manipulation macros
// ---------------------------------------------------------------------------
#define lowByte(w)              ((uint8_t)((w) & 0xff))
#define highByte(w)             ((uint8_t)((w) >> 8))

#define bitRead(value, bit)            (((value) >> (bit)) & 0x01)
#define bitSet(value, bit)             ((value) |=  (1UL << (bit)))
#define bitClear(value, bit)           ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit)          ((value) ^=  (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b)                         (1UL << (b))

// ---------------------------------------------------------------------------
// Type for interrupt service routines
// ---------------------------------------------------------------------------
typedef void (*voidFuncPtr)(void);

// ---------------------------------------------------------------------------
// Function declarations
// ---------------------------------------------------------------------------

// -- Digital I/O --
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int  digitalRead(uint8_t pin);

// -- Analog I/O --
int  analogRead(uint8_t pin);
void analogReference(uint8_t mode);
void analogWrite(uint8_t pin, int val);

// -- Advanced I/O --
void          tone(uint8_t pin, unsigned int frequency, unsigned long duration = 0);
void          noTone(uint8_t pin);
uint8_t       shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
void          shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000UL);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout = 1000000UL);

// -- Time --
unsigned long millis();
unsigned long micros();
void          delay(unsigned long ms);
void          delayMicroseconds(unsigned int us);

// -- Math --
long map(long x, long in_min, long in_max, long out_min, long out_max);

// -- Random numbers --
void randomSeed(unsigned long seed);
long random(long howbig);
long random(long howsmall, long howbig);

// -- External interrupts --
void attachInterrupt(uint8_t interrupt, voidFuncPtr ISR, uint8_t mode);
void detachInterrupt(uint8_t interrupt);
void interrupts();
void noInterrupts();
int  digitalPinToInterrupt(uint8_t pin);

// ---------------------------------------------------------------------------
// Character-classification inline helpers  (thin wrappers around <ctype.h>)
// ---------------------------------------------------------------------------
inline bool isAlpha(int c)            { return isalpha(c)  != 0; }
inline bool isAlphaNumeric(int c)     { return isalnum(c)  != 0; }
inline bool isAscii(int c)            { return (c & ~0x7f) == 0; }
inline bool isControl(int c)          { return iscntrl(c)  != 0; }
inline bool isDigit(int c)            { return isdigit(c)  != 0; }
inline bool isGraph(int c)            { return isgraph(c)  != 0; }
inline bool isHexadecimalDigit(int c) { return isxdigit(c) != 0; }
inline bool isLowerCase(int c)        { return islower(c)  != 0; }
inline bool isPrintable(int c)        { return isprint(c)  != 0; }
inline bool isPunct(int c)            { return ispunct(c)  != 0; }
inline bool isSpace(int c)            { return isspace(c)  != 0; }
inline bool isUpperCase(int c)        { return isupper(c)  != 0; }
inline bool isWhitespace(int c)       { return isspace(c)  != 0; }

// ---------------------------------------------------------------------------
// Test helper: inject simulated pin values from test code
// ---------------------------------------------------------------------------
void setDigitalValue(uint8_t pin, uint8_t value);
void setAnalogValue(uint8_t pin, int value);

// ---------------------------------------------------------------------------
// Pull in the Arduino String and Serial types
// ---------------------------------------------------------------------------
#include "WString.h"
#include "HardwareSerial.h"

#endif // Arduino_h
