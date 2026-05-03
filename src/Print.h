#pragma once
#ifndef Print_h
#define Print_h

#include <stdint.h>
#include <stddef.h>

// Base constants for print(value, base)
#define DEC 10
#define HEX 16
#define OCT  8
#define BIN  2

class String; // forward declaration

// ---------------------------------------------------------------------------
// Print – abstract base class that provides print() / println() in terms of
// a single pure-virtual write(uint8_t).
// ---------------------------------------------------------------------------
class Print {
public:
    virtual ~Print() = default;

    // Core write methods – must be overridden by subclasses
    virtual size_t write(uint8_t c) = 0;
    virtual size_t write(const uint8_t *buf, size_t size);

    // Convenience write overloads
    size_t write(const char *str);
    size_t write(const char *buf, size_t size);

    // print – no trailing newline
    size_t print(const char *str);
    size_t print(char c);
    size_t print(unsigned char n, int base = DEC);
    size_t print(int n, int base = DEC);
    size_t print(unsigned int n, int base = DEC);
    size_t print(long n, int base = DEC);
    size_t print(unsigned long n, int base = DEC);
    size_t print(double n, int digits = 2);
    size_t print(const String &s);

    // println – adds "\r\n" (matches Arduino behaviour)
    size_t println(const char *str);
    size_t println(char c);
    size_t println(unsigned char n, int base = DEC);
    size_t println(int n, int base = DEC);
    size_t println(unsigned int n, int base = DEC);
    size_t println(long n, int base = DEC);
    size_t println(unsigned long n, int base = DEC);
    size_t println(double n, int digits = 2);
    size_t println(const String &s);
    size_t println();

    // Returns number of bytes that failed to write (0 = all written)
    int getWriteError() { return _writeError; }
    void clearWriteError() { _writeError = 0; }

protected:
    void setWriteError(int err = 1) { _writeError = err; }

private:
    int _writeError = 0;

    size_t printNumber(unsigned long n, uint8_t base);
    size_t printFloat(double n, uint8_t digits);
};

#endif // Print_h
