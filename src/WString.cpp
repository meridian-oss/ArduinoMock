// WString.cpp – char*-buffer implementation of Arduino's String class.
// No <string> / <algorithm> dependency; avoids min/max macro conflicts.

#include "WString.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------
bool String::_grow(unsigned int newLen) {
    unsigned int needed = newLen + 1u;
    if (needed <= _cap) return true;
    unsigned int newCap = (needed < 16u) ? 16u : (needed + needed / 2u);
    char *p = static_cast<char *>(realloc(_buf, newCap));
    if (!p) return false;
    _buf = p;
    _cap = newCap;
    if (_len == 0) _buf[0] = '\0';
    return true;
}

void String::_copy(const char *cstr, unsigned int len) {
    if (!_grow(len)) return;
    if (cstr) memcpy(_buf, cstr, len);
    _buf[len] = '\0';
    _len = len;
}

void String::_numToStr(char *buf, size_t buflen, long value, unsigned char base) {
    if (base == 10) { snprintf(buf, buflen, "%ld", value); return; }
    _unumToStr(buf, buflen, static_cast<unsigned long>(value), base);
}

void String::_unumToStr(char *buf, size_t buflen, unsigned long value, unsigned char base) {
    if (base < 2 || base > 16 || buflen == 0) { buf[0] = '\0'; return; }
    if (base == 10) { snprintf(buf, buflen, "%lu", value); return; }
    const char digits[] = "0123456789abcdef";
    char tmp[8 * sizeof(unsigned long) + 2];
    char *p = &tmp[sizeof(tmp) - 1];
    *p = '\0';
    do { *--p = digits[value % base]; value /= base; } while (value);
    size_t n = static_cast<size_t>(&tmp[sizeof(tmp) - 1] - p);
    if (n >= buflen) n = buflen - 1;
    memcpy(buf, p, n);
    buf[n] = '\0';
}

// ---------------------------------------------------------------------------
// Constructors / destructor
// ---------------------------------------------------------------------------
String::String()                            : _buf(nullptr), _len(0), _cap(0) {}
String::~String()                           { free(_buf); }

String::String(const char *cstr)            : _buf(nullptr), _len(0), _cap(0) {
    if (cstr) _copy(cstr, static_cast<unsigned int>(strlen(cstr)));
}
String::String(const char *cstr, size_t n)  : _buf(nullptr), _len(0), _cap(0) {
    if (cstr && n) _copy(cstr, static_cast<unsigned int>(n));
}
String::String(const String &o)             : _buf(nullptr), _len(0), _cap(0) {
    if (o._buf) _copy(o._buf, o._len);
}
String::String(String &&o) noexcept
    : _buf(o._buf), _len(o._len), _cap(o._cap)
    { o._buf = nullptr; o._len = 0; o._cap = 0; }

String::String(char c)          : _buf(nullptr), _len(0), _cap(0)
    { char t[2]={c,0}; _copy(t,1); }
String::String(unsigned char c) : _buf(nullptr), _len(0), _cap(0)
    { char t[2]={static_cast<char>(c),0}; _copy(t,1); }

String::String(int v, unsigned char base)           : _buf(nullptr), _len(0), _cap(0)
    { char t[36]; _numToStr(t,sizeof(t),static_cast<long>(v),base); _copy(t,static_cast<unsigned int>(strlen(t))); }
String::String(unsigned int v, unsigned char base)  : _buf(nullptr), _len(0), _cap(0)
    { char t[36]; _unumToStr(t,sizeof(t),static_cast<unsigned long>(v),base); _copy(t,static_cast<unsigned int>(strlen(t))); }
String::String(long v, unsigned char base)          : _buf(nullptr), _len(0), _cap(0)
    { char t[36]; _numToStr(t,sizeof(t),v,base); _copy(t,static_cast<unsigned int>(strlen(t))); }
String::String(unsigned long v, unsigned char base) : _buf(nullptr), _len(0), _cap(0)
    { char t[36]; _unumToStr(t,sizeof(t),v,base); _copy(t,static_cast<unsigned int>(strlen(t))); }
String::String(float v, unsigned char dp)           : _buf(nullptr), _len(0), _cap(0)
    { char fmt[8],t[32]; snprintf(fmt,sizeof(fmt),"%%.%uf",static_cast<unsigned>(dp)); snprintf(t,sizeof(t),fmt,static_cast<double>(v)); _copy(t,static_cast<unsigned int>(strlen(t))); }
String::String(double v, unsigned char dp)          : _buf(nullptr), _len(0), _cap(0)
    { char fmt[8],t[32]; snprintf(fmt,sizeof(fmt),"%%.%uf",static_cast<unsigned>(dp)); snprintf(t,sizeof(t),fmt,v); _copy(t,static_cast<unsigned int>(strlen(t))); }

// ---------------------------------------------------------------------------
// Assignment
// ---------------------------------------------------------------------------
String &String::operator=(const String &rhs) {
    if (this != &rhs) {
        if (rhs._buf) _copy(rhs._buf, rhs._len);
        else { free(_buf); _buf=nullptr; _len=0; _cap=0; }
    }
    return *this;
}
String &String::operator=(String &&rhs) noexcept {
    if (this != &rhs) {
        free(_buf); _buf=rhs._buf; _len=rhs._len; _cap=rhs._cap;
        rhs._buf=nullptr; rhs._len=0; rhs._cap=0;
    }
    return *this;
}
String &String::operator=(const char *cstr) {
    if (cstr) _copy(cstr, static_cast<unsigned int>(strlen(cstr)));
    else { free(_buf); _buf=nullptr; _len=0; _cap=0; }
    return *this;
}
String &String::operator=(char c)
    { char t[2]={c,0}; _copy(t,1); return *this; }

// ---------------------------------------------------------------------------
// reserve
// ---------------------------------------------------------------------------
bool String::reserve(unsigned int size) { return _grow(size); }

bool String::concat(const String &str) {
    if (!str._buf || str._len == 0) return true;
    unsigned int newLen = _len + str._len;
    if (!_grow(newLen)) return false;
    memcpy(_buf + _len, str._buf, str._len);
    _len = newLen;
    _buf[_len] = '\0';
    return true;
}

bool String::concat(const char *cstr) {
    if (!cstr) return true;
    unsigned int addLen = static_cast<unsigned int>(strlen(cstr));
    if (addLen == 0) return true;
    unsigned int newLen = _len + addLen;
    if (!_grow(newLen)) return false;
    memcpy(_buf + _len, cstr, addLen);
    _len = newLen;
    _buf[_len] = '\0';
    return true;
}

bool String::concat(char c) {
    unsigned int newLen = _len + 1;
    if (!_grow(newLen)) return false;
    _buf[_len] = c;
    _len = newLen;
    _buf[_len] = '\0';
    return true;
}
bool String::concat(unsigned char c) { return concat(static_cast<char>(c)); }
bool String::concat(int n)           { return concat(String(n)); }
bool String::concat(unsigned int n)  { return concat(String(n)); }
bool String::concat(long n)          { return concat(String(n)); }
bool String::concat(unsigned long n) { return concat(String(n)); }
bool String::concat(float n)         { return concat(String(n, 2)); }
bool String::concat(double n)        { return concat(String(n, 2)); }

// ---------------------------------------------------------------------------
// operator+=
// ---------------------------------------------------------------------------
String &String::operator+=(const String &rhs)  { concat(rhs);         return *this; }
String &String::operator+=(const char *cstr)   { concat(cstr);        return *this; }
String &String::operator+=(char c)             { concat(c);           return *this; }
String &String::operator+=(unsigned char c)    { concat(c);           return *this; }
String &String::operator+=(int n)              { concat(String(n));   return *this; }
String &String::operator+=(unsigned int n)     { concat(String(n));   return *this; }
String &String::operator+=(long n)             { concat(String(n));   return *this; }
String &String::operator+=(unsigned long n)    { concat(String(n));   return *this; }
String &String::operator+=(float n)            { concat(String(n,2)); return *this; }
String &String::operator+=(double n)           { concat(String(n,2)); return *this; }

// ---------------------------------------------------------------------------
// Comparison operators
// ---------------------------------------------------------------------------
bool String::operator==(const String &rhs) const {
    if (_len != rhs._len) return false;
    if (!_buf && !rhs._buf) return true;
    if (!_buf || !rhs._buf) return (_len == 0 && rhs._len == 0);
    return memcmp(_buf, rhs._buf, _len) == 0;
}
bool String::operator!=(const String &rhs) const { return !(*this == rhs); }
bool String::operator<(const String &rhs)  const
    { return strcmp(_buf?_buf:"", rhs._buf?rhs._buf:"") < 0; }
bool String::operator>(const String &rhs)  const { return rhs < *this; }
bool String::operator<=(const String &rhs) const { return !(rhs < *this); }
bool String::operator>=(const String &rhs) const { return !(*this < rhs); }
bool String::operator==(const char *cstr)  const {
    if (!cstr)  return _len == 0;
    if (!_buf)  return cstr[0] == '\0';
    return strcmp(_buf, cstr) == 0;
}
bool String::operator!=(const char *cstr) const { return !(*this == cstr); }

// ---------------------------------------------------------------------------
// Element access
// ---------------------------------------------------------------------------
char String::operator[](unsigned int i) const
    { return (_buf && i < _len) ? _buf[i] : '\0'; }
char &String::operator[](unsigned int i) {
    static char dummy = '\0'; dummy = '\0';
    return (_buf && i < _len) ? _buf[i] : dummy;
}
char String::charAt(unsigned int i)  const { return (*this)[i]; }
void String::setCharAt(unsigned int i, char c)
    { if (_buf && i < _len) _buf[i] = c; }

// ---------------------------------------------------------------------------
// Search
// ---------------------------------------------------------------------------
int String::indexOf(char ch, unsigned int from) const {
    if (!_buf || from >= _len) return -1;
    const char *p = static_cast<const char *>(memchr(_buf + from, ch, _len - from));
    return p ? static_cast<int>(p - _buf) : -1;
}
int String::indexOf(const String &str, unsigned int from) const {
    if (!_buf || !str._buf || from >= _len || str._len == 0) return -1;
    for (unsigned int i = from; i + str._len <= _len; i++)
        if (memcmp(_buf + i, str._buf, str._len) == 0) return static_cast<int>(i);
    return -1;
}
int String::lastIndexOf(char ch) const {
    if (!_buf) return -1;
    for (int i = static_cast<int>(_len) - 1; i >= 0; i--)
        if (_buf[i] == ch) return i;
    return -1;
}
int String::lastIndexOf(char ch, unsigned int from) const {
    if (!_buf || _len == 0) return -1;
    if (from >= _len) from = _len - 1;
    for (int i = static_cast<int>(from); i >= 0; i--)
        if (_buf[i] == ch) return i;
    return -1;
}
int String::lastIndexOf(const String &str) const { return lastIndexOf(str, _len); }
int String::lastIndexOf(const String &str, unsigned int from) const {
    if (!_buf || !str._buf || str._len > _len) return -1;
    if (from >= _len) from = _len - str._len;
    for (int i = static_cast<int>(from); i >= 0; i--)
        if (static_cast<unsigned int>(_len - i) >= str._len &&
            memcmp(_buf + i, str._buf, str._len) == 0)
            return i;
    return -1;
}

// ---------------------------------------------------------------------------
// Predicates
// ---------------------------------------------------------------------------
bool String::startsWith(const String &pfx) const {
    if (pfx._len > _len) return false;
    if (pfx._len == 0)   return true;
    return memcmp(_buf, pfx._buf, pfx._len) == 0;
}
bool String::startsWith(const String &pfx, unsigned int off) const {
    if (off + pfx._len > _len) return false;
    if (pfx._len == 0)         return true;
    return memcmp(_buf + off, pfx._buf, pfx._len) == 0;
}
bool String::endsWith(const String &sfx) const {
    if (sfx._len > _len) return false;
    if (sfx._len == 0)   return true;
    return memcmp(_buf + _len - sfx._len, sfx._buf, sfx._len) == 0;
}
bool String::equals(const String &s) const { return *this == s; }
bool String::equals(const char *c)   const { return *this == c; }
bool String::equalsIgnoreCase(const String &s) const {
    if (_len != s._len) return false;
    for (unsigned int i = 0; i < _len; i++)
        if (tolower(static_cast<unsigned char>(_buf[i])) !=
            tolower(static_cast<unsigned char>(s._buf[i]))) return false;
    return true;
}
int String::compareTo(const String &s) const
    { return strcmp(_buf?_buf:"", s._buf?s._buf:""); }

// ---------------------------------------------------------------------------
// Transformation
// ---------------------------------------------------------------------------
void String::toLowerCase() {
    for (unsigned int i = 0; i < _len; i++)
        _buf[i] = static_cast<char>(tolower(static_cast<unsigned char>(_buf[i])));
}
void String::toUpperCase() {
    for (unsigned int i = 0; i < _len; i++)
        _buf[i] = static_cast<char>(toupper(static_cast<unsigned char>(_buf[i])));
}
void String::trim() {
    if (!_buf || _len == 0) return;
    // trim leading
    unsigned int start = 0;
    while (start < _len && isspace(static_cast<unsigned char>(_buf[start]))) start++;
    // trim trailing
    unsigned int end = _len;
    while (end > start && isspace(static_cast<unsigned char>(_buf[end-1]))) end--;
    unsigned int newLen = end - start;
    if (start > 0) memmove(_buf, _buf + start, newLen);
    _len = newLen;
    _buf[_len] = '\0';
}
void String::replace(char find, char rep) {
    for (unsigned int i = 0; i < _len; i++)
        if (_buf[i] == find) _buf[i] = rep;
}
void String::replace(const String &find, const String &rep) {
    if (!find._buf || find._len == 0) return;
    String result;
    unsigned int i = 0;
    while (i <= _len) {
        if (i + find._len <= _len && memcmp(_buf + i, find._buf, find._len) == 0) {
            if (rep._buf) result.concat(rep);
            i += find._len;
        } else {
            if (i < _len) result.concat(_buf[i]);
            i++;
        }
    }
    *this = static_cast<String &&>(result);
}
void String::remove(unsigned int index) {
    if (!_buf || index >= _len) return;
    _len = index;
    _buf[_len] = '\0';
}
void String::remove(unsigned int index, unsigned int count) {
    if (!_buf || index >= _len) return;
    if (index + count >= _len) { remove(index); return; }
    memmove(_buf + index, _buf + index + count, _len - index - count);
    _len -= count;
    _buf[_len] = '\0';
}

// ---------------------------------------------------------------------------
// Substrings
// ---------------------------------------------------------------------------
String String::substring(unsigned int begin) const {
    if (!_buf || begin >= _len) return String();
    return String(_buf + begin, _len - begin);
}
String String::substring(unsigned int begin, unsigned int end) const {
    if (!_buf || begin >= _len) return String();
    if (end > _len) end = _len;
    if (end < begin) end = begin;
    return String(_buf + begin, end - begin);
}

// ---------------------------------------------------------------------------
// Conversion
// ---------------------------------------------------------------------------
long String::toInt() const {
    return _buf ? strtol(_buf, nullptr, 10) : 0L;
}
float String::toFloat() const {
    return _buf ? static_cast<float>(strtod(_buf, nullptr)) : 0.0f;
}
double String::toDouble() const {
    return _buf ? strtod(_buf, nullptr) : 0.0;
}
void String::getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index) const {
    if (!bufsize || !buf) return;
    unsigned int n = 0;
    if (_buf && index < _len) {
        n = _len - index;
        if (n > bufsize - 1) n = bufsize - 1;
        memcpy(buf, _buf + index, n);
    }
    buf[n] = '\0';
}
