#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>
#include <stdarg.h>

#undef DEBUG

#ifdef DEBUG
#define DEBUGP(...)                   \
  do {                                \
    Serial.printf(__VA_ARGS__); \
  } while (false)
#else
#define DEBUGP(...) \
  do {              \
  } while (false)
#endif

int freeMemory();

#endif
