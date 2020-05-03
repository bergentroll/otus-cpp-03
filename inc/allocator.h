// TODO Profiling.

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <limits>
#include <new>
#include <map>

#include "logger.h"

namespace otus {
  template <typename T, int chunk_size = 0>
  class Allocator {
  public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

    Allocator():
    chunk(reinterpret_cast<pointer>(operator new[](chunk_size * sizeof(T)))) { }

    ~Allocator() {
      log("Deleting pool.");
      operator delete[](chunk);
    }

    template<typename U>
    struct rebind { using other = Allocator<U, chunk_size>; };

    size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }

    [[ nodiscard ]]
    pointer allocate(size_type size) {
      log("Allocate memory for " + std::to_string(size) + " items.");
      if (cursor + size > chunk_size) throw std::bad_alloc();
      auto ptr { &chunk[cursor] };
      cursor += size;
      return ptr;
    }

    void deallocate(pointer ptr, size_type size) noexcept {
      log("Query for deallocation, do nothing.");
    }

    template<typename U, typename ...Args>
    void construct(U *ptr, Args &&...args) {
      log("Emplace item.");
      new(ptr) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U *ptr) { ptr->~U(); }

    // TODO функция address
    // TODO операторы сравнения == и !=

    private:
      pointer chunk;
      size_type cursor { 0 };
  };
}

#endif
