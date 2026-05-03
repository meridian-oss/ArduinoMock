#pragma once
#ifndef Stream_h
#define Stream_h

#include "Print.h"
#include <stddef.h>
#include <stdint.h>

class String; // forward declaration

// ---------------------------------------------------------------------------
// Stream – extends Print with read() / available() / peek().
// Mirrors the Arduino Stream class API.
// ---------------------------------------------------------------------------
class Stream : public Print {
public:
  virtual ~Stream() = default;

  // -- Must be implemented by subclasses --
  virtual int available() = 0;
  virtual int read() = 0;
  virtual int peek() = 0;
  virtual void flush() {}

  // -- Timeout for blocking read operations (default 1000 ms) --
  void setTimeout(unsigned long timeout) { _timeout = timeout; }
  unsigned long getTimeout() const { return _timeout; }

  // -- Blocking read helpers --
  size_t readBytes(char *buffer, size_t length);
  size_t readBytesUntil(char terminator, char *buffer, size_t length);
  String readString();
  String readStringUntil(char terminator);

  // -- Parsing helpers --
  bool find(char *target);
  bool find(char *target, size_t length);
  bool findUntil(char *target, char *terminator);
  long parseInt(char skipChar = '\0');
  float parseFloat(char skipChar = '\0');

protected:
  unsigned long _timeout = 1000;

private:
  int timedRead();
  int timedPeek();
  int peekNextDigit(bool detectDecimal);
};

#endif // Stream_h
