#pragma once
#ifndef WString_h
#define WString_h

// WString.h – Arduino-compatible String class.
// Uses a dynamic char* buffer (like the real Arduino String) so that this
// header does NOT pull in <string> / <string_view>.  That keeps it free of
// the min/max/abs macro conflicts that arise when mixing Arduino.h macros
// with the C++ standard library on native/desktop targets.

#include <stdint.h>
#include <stddef.h>

// ---------------------------------------------------------------------------
// String
// ---------------------------------------------------------------------------
class String {
public:
    // -----------------------------------------------------------------------
    // Constructors / destructor
    // -----------------------------------------------------------------------
    String();
    String(const char *cstr);
    String(const char *cstr, size_t length);
    String(const String &other);
    String(String &&other) noexcept;
    explicit String(char c);
    explicit String(unsigned char c);
    explicit String(int value,           unsigned char base = 10);
    explicit String(unsigned int value,  unsigned char base = 10);
    explicit String(long value,          unsigned char base = 10);
    explicit String(unsigned long value, unsigned char base = 10);
    explicit String(float value,         unsigned char decimalPlaces = 2);
    explicit String(double value,        unsigned char decimalPlaces = 2);
    ~String();

    // -----------------------------------------------------------------------
    // Assignment
    // -----------------------------------------------------------------------
    String &operator=(const String &rhs);
    String &operator=(String &&rhs) noexcept;
    String &operator=(const char *cstr);
    String &operator=(char c);

    // -----------------------------------------------------------------------
    // Concatenation
    // -----------------------------------------------------------------------
    String &operator+=(const String &rhs);
    String &operator+=(const char *cstr);
    String &operator+=(char c);
    String &operator+=(unsigned char c);
    String &operator+=(int n);
    String &operator+=(unsigned int n);
    String &operator+=(long n);
    String &operator+=(unsigned long n);
    String &operator+=(float n);
    String &operator+=(double n);

    friend String operator+(String lhs, const String  &rhs) { lhs += rhs; return lhs; }
    friend String operator+(String lhs, const char    *rhs) { lhs += rhs; return lhs; }
    friend String operator+(String lhs, char           rhs) { lhs += rhs; return lhs; }
    friend String operator+(String lhs, int            rhs) { lhs += rhs; return lhs; }
    friend String operator+(String lhs, unsigned int   rhs) { lhs += rhs; return lhs; }
    friend String operator+(String lhs, long           rhs) { lhs += rhs; return lhs; }
    friend String operator+(String lhs, unsigned long  rhs) { lhs += rhs; return lhs; }
    friend String operator+(String lhs, float          rhs) { lhs += rhs; return lhs; }
    friend String operator+(String lhs, double         rhs) { lhs += rhs; return lhs; }

    bool concat(const String &str);
    bool concat(const char *cstr);
    bool concat(char c);
    bool concat(unsigned char c);
    bool concat(int n);
    bool concat(unsigned int n);
    bool concat(long n);
    bool concat(unsigned long n);
    bool concat(float n);
    bool concat(double n);

    // -----------------------------------------------------------------------
    // Comparison operators
    // -----------------------------------------------------------------------
    bool operator==(const String &rhs) const;
    bool operator!=(const String &rhs) const;
    bool operator< (const String &rhs) const;
    bool operator> (const String &rhs) const;
    bool operator<=(const String &rhs) const;
    bool operator>=(const String &rhs) const;
    bool operator==(const char *cstr) const;
    bool operator!=(const char *cstr) const;

    // -----------------------------------------------------------------------
    // Element access
    // -----------------------------------------------------------------------
    char operator[](unsigned int index) const;
    char &operator[](unsigned int index);
    char charAt(unsigned int index) const;
    void setCharAt(unsigned int index, char c);

    // -----------------------------------------------------------------------
    // Info
    // -----------------------------------------------------------------------
    unsigned int length() const { return _len; }
    bool         isEmpty() const { return _len == 0; }

    // -----------------------------------------------------------------------
    // Search
    // -----------------------------------------------------------------------
    int indexOf(char ch, unsigned int fromIndex = 0) const;
    int indexOf(const String &str, unsigned int fromIndex = 0) const;
    int lastIndexOf(char ch) const;
    int lastIndexOf(char ch, unsigned int fromIndex) const;
    int lastIndexOf(const String &str) const;
    int lastIndexOf(const String &str, unsigned int fromIndex) const;

    // -----------------------------------------------------------------------
    // Predicates
    // -----------------------------------------------------------------------
    bool startsWith(const String &prefix) const;
    bool startsWith(const String &prefix, unsigned int offset) const;
    bool endsWith(const String &suffix) const;
    bool equals(const String &s) const;
    bool equals(const char *cstr) const;
    bool equalsIgnoreCase(const String &s) const;
    int  compareTo(const String &s) const;

    // -----------------------------------------------------------------------
    // Transformation (in-place)
    // -----------------------------------------------------------------------
    void toLowerCase();
    void toUpperCase();
    void trim();
    void replace(char find, char rep);
    void replace(const String &find, const String &rep);
    void remove(unsigned int index);
    void remove(unsigned int index, unsigned int count);

    // -----------------------------------------------------------------------
    // Substrings
    // -----------------------------------------------------------------------
    String substring(unsigned int beginIndex) const;
    String substring(unsigned int beginIndex, unsigned int endIndex) const;

    // -----------------------------------------------------------------------
    // Conversion
    // -----------------------------------------------------------------------
    long        toInt()    const;
    float       toFloat()  const;
    double      toDouble() const;
    const char *c_str()    const { return _buf ? _buf : ""; }

    void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index = 0) const;
    void toCharArray(char *buf, unsigned int bufsize, unsigned int index = 0) const {
        getBytes(reinterpret_cast<unsigned char *>(buf), bufsize, index);
    }

    // Pre-allocate capacity
    bool reserve(unsigned int size);

    // Implicit validity test
    explicit operator bool() const { return _len > 0; }

private:
    char        *_buf;      // null-terminated buffer (may be nullptr)
    unsigned int _len;      // string length (excluding null terminator)
    unsigned int _cap;      // allocated capacity (including null terminator)

    // Internal helpers
    bool        _grow(unsigned int newLen);
    void        _copy(const char *cstr, unsigned int len);
    static void _numToStr(char *buf, size_t buflen, long value, unsigned char base);
    static void _unumToStr(char *buf, size_t buflen, unsigned long value, unsigned char base);
};

#endif // WString_h
