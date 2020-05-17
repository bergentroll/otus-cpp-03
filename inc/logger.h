#ifndef LOGGER_H
#define LOGGER_H

#define LOG(x)

#ifdef DEBUG
  #include <iostream>
  #undef LOG
  #define LOG(x) (std::cerr << x)
#endif

#endif
