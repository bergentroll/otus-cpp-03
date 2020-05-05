#ifndef LOGGER_H
#define LOGGER_H

#ifdef DEBUG
#include <iostream>
#endif

namespace otus {
  void log(const std::string &s) {
    #ifdef DEBUG
    std::cerr << s << std::endl;
    #endif
    return;
  }
}

#endif
