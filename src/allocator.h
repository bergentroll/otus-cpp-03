#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstdlib>

namespace otus {
  template <typename T>
  class Allocator {
  public:
    using value_type =T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

    Allocator() = default;

    [[ nodiscard ]]
    pointer allocate(const size_type size) {
      return reinterpret_cast<pointer>(std::malloc(size));
    }

    void deallocate(pointer ptr, size_type size) noexcept {
      std::free(ptr);
    }
  };
}

#endif
