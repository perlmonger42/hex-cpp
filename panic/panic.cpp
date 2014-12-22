#include <cstdarg>
#include <cstdio>
#include "panic.h"

void panic(const char *fmt, ...) {
  va_list ap;
  char text[1000];
  va_start(ap, fmt);
  vsnprintf(text, sizeof text, fmt, ap);
  va_end(ap);
  throw Panic(std::string(text));
}
