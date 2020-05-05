// TODO Profiling.
// TODO New chunk on overflow.

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

    template<typename U>
    struct rebind { using other = Allocator<U, chunk_size>; };

    Allocator():
    chunk(reinterpret_cast<pointer>(operator new[](chunk_size * sizeof(T)))) { }

    ~Allocator() {
      LOG("Deleting pool." << std::endl);
      operator delete[](chunk);
    }

    size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }

    [[ nodiscard ]]
    pointer allocate(size_type size) {
      LOG("Allocate memory for " << size << " items." << std::endl);
      if (cursor + size > chunk_size) throw std::bad_alloc();
      auto ptr { &chunk[cursor] };
      cursor += size;
      return ptr;
    }

    void deallocate(pointer ptr, size_type size) const noexcept {
      LOG("Query for deallocation, do nothing." << std::endl);
    }

    template<typename U, typename ...Args>
    void construct(U *ptr, Args &&...args) const {
      LOG("Emplace item." << std::endl);
      new(ptr) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U *ptr) const { ptr->~U(); }

    bool operator==(Allocator const &other) const noexcept {
      if (cursor != other.cursor) return false;

      for (size_t i { 0 }; i < cursor; ++i) {
        if (chunk[i] != other.chunk[i]) return false;
      }

      return true;
    }

    bool operator!=(Allocator const &other) const noexcept {
      return !(*this == other);
    }

    [[ deprecated ]]
    pointer address(reference x) const noexcept {
      return &x;
    }

    [[ deprecated ]]
    const_pointer address(const_reference x) const noexcept {
      return &x;
    }

    private:
      pointer chunk;
      size_type cursor { 0 };
  };
}

#endif
