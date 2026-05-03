// Include Stream's own header and WString first so <string> is processed
// before Arduino.h defines the min/max/abs/round macros.
#include "Stream.h"
#include "WString.h"
#include "Arduino.h"   // for millis()

// ---------------------------------------------------------------------------
// Timed read helpers
// ---------------------------------------------------------------------------
int Stream::timedRead() {
    int c;
    unsigned long start = millis();
    do {
        c = read();
        if (c >= 0) return c;
    } while (millis() - start < _timeout);
    return -1;
}

int Stream::timedPeek() {
    int c;
    unsigned long start = millis();
    do {
        c = peek();
        if (c >= 0) return c;
    } while (millis() - start < _timeout);
    return -1;
}

int Stream::peekNextDigit(bool detectDecimal) {
    int c;
    while (true) {
        c = timedPeek();
        if (c < 0)                              return c;
        if (c == '-')                           return c;
        if (c >= '0' && c <= '9')               return c;
        if (detectDecimal && c == '.')          return c;
        read(); // discard non-digit
    }
}

// ---------------------------------------------------------------------------
// readBytes / readBytesUntil
// ---------------------------------------------------------------------------
size_t Stream::readBytes(char *buffer, size_t length) {
    size_t count = 0;
    while (count < length) {
        int c = timedRead();
        if (c < 0) break;
        buffer[count++] = static_cast<char>(c);
    }
    return count;
}

size_t Stream::readBytesUntil(char terminator, char *buffer, size_t length) {
    if (length == 0) return 0;
    size_t index = 0;
    while (index < length) {
        int c = timedRead();
        if (c < 0 || c == terminator) break;
        buffer[index++] = static_cast<char>(c);
    }
    return index;
}

// ---------------------------------------------------------------------------
// readString / readStringUntil
// ---------------------------------------------------------------------------
String Stream::readString() {
    String ret;
    int c;
    unsigned long start = millis();
    while (millis() - start < _timeout) {
        c = timedRead();
        if (c < 0) break;
        ret += static_cast<char>(c);
    }
    return ret;
}

String Stream::readStringUntil(char terminator) {
    String ret;
    int c;
    while (true) {
        c = timedRead();
        if (c < 0 || c == terminator) break;
        ret += static_cast<char>(c);
    }
    return ret;
}

// ---------------------------------------------------------------------------
// find / findUntil
// ---------------------------------------------------------------------------
bool Stream::find(char *target) {
    return find(target, strlen(target));
}

bool Stream::find(char *target, size_t length) {
    if (length == 0) return true;
    size_t index = 0;
    while (true) {
        int c = timedRead();
        if (c < 0) return false;
        if (c == target[index]) {
            if (++index >= length) return true;
        } else {
            index = (c == target[0]) ? 1 : 0;
        }
    }
}

bool Stream::findUntil(char *target, char *terminator) {
    size_t tLen = strlen(target);
    size_t uLen = strlen(terminator);
    size_t ti = 0, ui = 0;
    while (true) {
        int c = timedRead();
        if (c < 0) return false;
        if (c == target[ti]) {
            if (++ti >= tLen) return true;
        } else {
            ti = (c == target[0]) ? 1 : 0;
        }
        if (uLen > 0) {
            if (c == terminator[ui]) {
                if (++ui >= uLen) return false;
            } else {
                ui = (c == terminator[0]) ? 1 : 0;
            }
        }
    }
}

// ---------------------------------------------------------------------------
// parseInt / parseFloat
// ---------------------------------------------------------------------------
long Stream::parseInt(char skipChar) {
    bool    isNeg = false;
    long    value = 0;
    int     c     = peekNextDigit(false);
    if (c < 0) return 0;

    do {
        if (c == skipChar) {
            // skip
        } else if (c == '-') {
            isNeg = true;
        } else if (c >= '0' && c <= '9') {
            value = value * 10 + (c - '0');
        } else {
            break;
        }
        read();
        c = timedPeek();
    } while (c >= 0);

    return isNeg ? -value : value;
}

float Stream::parseFloat(char skipChar) {
    bool  isNeg    = false;
    bool  isFrac   = false;
    float value    = 0.0f;
    float fraction = 1.0f;
    int   c        = peekNextDigit(true);
    if (c < 0) return 0.0f;

    do {
        if (c == skipChar) {
            // skip
        } else if (c == '-') {
            isNeg = true;
        } else if (c == '.') {
            isFrac = true;
        } else if (c >= '0' && c <= '9') {
            if (isFrac) {
                fraction /= 10.0f;
                value += (c - '0') * fraction;
            } else {
                value = value * 10.0f + (c - '0');
            }
        } else {
            break;
        }
        read();
        c = timedPeek();
    } while (c >= 0);

    return isNeg ? -value : value;
}
