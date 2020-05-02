#ifndef LOGGER_H
#define LOGGER

#ifdef DEBUG
#include <iostream>
#endif

void log(const std::string &s) {
  #ifdef DEBUG
  std::cerr << s << std::endl;
  #endif
  return;
}

#endif
