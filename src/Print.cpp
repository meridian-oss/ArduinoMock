#include "Print.h"
#include "WString.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

// ---------------------------------------------------------------------------
// write helpers
// ---------------------------------------------------------------------------
size_t Print::write(const uint8_t *buf, size_t size) {
    size_t n = 0;
    while (size--) {
        if (write(*buf++) != 0) n++;
    }
    return n;
}

size_t Print::write(const char *str) {
    if (!str) return 0;
    return write(reinterpret_cast<const uint8_t *>(str), strlen(str));
}

size_t Print::write(const char *buf, size_t size) {
    return write(reinterpret_cast<const uint8_t *>(buf), size);
}

// ---------------------------------------------------------------------------
// print (no newline)
// ---------------------------------------------------------------------------
size_t Print::print(const char *str) {
    return write(str);
}

size_t Print::print(char c) {
    return write(static_cast<uint8_t>(c));
}

size_t Print::print(unsigned char n, int base) {
    return print(static_cast<unsigned long>(n), base);
}

size_t Print::print(int n, int base) {
    if (base == DEC && n < 0) {
        size_t t = write('-');
        return t + printNumber(static_cast<unsigned long>(-n), base);
    }
    return printNumber(static_cast<unsigned long>(n), base);
}

size_t Print::print(unsigned int n, int base) {
    return printNumber(static_cast<unsigned long>(n), base);
}

size_t Print::print(long n, int base) {
    if (base == DEC && n < 0) {
        size_t t = write('-');
        return t + printNumber(static_cast<unsigned long>(-n), base);
    }
    return printNumber(static_cast<unsigned long>(n), base);
}

size_t Print::print(unsigned long n, int base) {
    return printNumber(n, base);
}

size_t Print::print(double n, int digits) {
    return printFloat(n, static_cast<uint8_t>(digits));
}

size_t Print::print(const String &s) {
    return write(s.c_str());
}

// ---------------------------------------------------------------------------
// println (adds "\r\n")
// ---------------------------------------------------------------------------
size_t Print::println(const char *str) {
    size_t n = print(str);
    n += println();
    return n;
}

size_t Print::println(char c) {
    size_t n = print(c);
    n += println();
    return n;
}

size_t Print::println(unsigned char n, int base) {
    size_t t = print(n, base);
    t += println();
    return t;
}

size_t Print::println(int n, int base) {
    size_t t = print(n, base);
    t += println();
    return t;
}

size_t Print::println(unsigned int n, int base) {
    size_t t = print(n, base);
    t += println();
    return t;
}

size_t Print::println(long n, int base) {
    size_t t = print(n, base);
    t += println();
    return t;
}

size_t Print::println(unsigned long n, int base) {
    size_t t = print(n, base);
    t += println();
    return t;
}

size_t Print::println(double n, int digits) {
    size_t t = print(n, digits);
    t += println();
    return t;
}

size_t Print::println(const String &s) {
    size_t t = print(s);
    t += println();
    return t;
}

size_t Print::println() {
    return write("\r\n");
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------
size_t Print::printNumber(unsigned long n, uint8_t base) {
    if (base < 2) base = 2;

    char buf[8 * sizeof(unsigned long) + 1];
    char *str = &buf[sizeof(buf) - 1];
    *str = '\0';

    do {
        unsigned long rem = n % base;
        *--str = (rem < 10) ? ('0' + rem) : ('A' + rem - 10);
        n /= base;
    } while (n);

    return write(str);
}

size_t Print::printFloat(double number, uint8_t digits) {
    size_t n = 0;

    if (isnan(number))  return print("nan");
    if (isinf(number))  return print("inf");
    if (number >  4294967040.0) return print("ovf");
    if (number < -4294967040.0) return print("ovf");

    if (number < 0.0) {
        n += print('-');
        number = -number;
    }

    // Round
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; i++) rounding /= 10.0;
    number += rounding;

    unsigned long int_part  = static_cast<unsigned long>(number);
    double         rem      = number - static_cast<double>(int_part);

    n += printNumber(int_part, 10);

    if (digits > 0) {
        n += print('.');
        while (digits--) {
            rem *= 10.0;
            unsigned long digit = static_cast<unsigned long>(rem);
            n += printNumber(digit, 10);
            rem -= static_cast<double>(digit);
        }
    }
    return n;
}
