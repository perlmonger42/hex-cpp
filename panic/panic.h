#ifndef __PANIC_H__
#define __PANIC_H__ 1

#include <exception>
#include <string>

void panic(const char *s, ...) __attribute__((format(printf,1,2)));

// panic() throws an exception of this kind.
struct Panic : std::exception {
  std::string text;
  Panic(std::string t) : text(t) { }
  virtual ~Panic() _NOEXCEPT { };
  char const* what() const throw() { return text.c_str(); }
};
#endif /* __PANIC_H__ */
