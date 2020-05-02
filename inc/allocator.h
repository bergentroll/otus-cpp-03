#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <limits>
#include <new>

#ifdef DEBUG
#include <iostream>
#endif

namespace otus {
  template <typename T, int chunk_size = 0>
  class Allocator {
  public:
    using value_type =T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

    Allocator():
    chunk(reinterpret_cast<pointer>(operator new[](chunk_size * sizeof(T)))) { }

    ~Allocator() {
      #ifdef DEBUG
      std::cerr << "Deleting pool." << std::endl;
      #endif

      operator delete[](chunk);
    }

    template<typename U>
    struct rebind { using other = Allocator<U, chunk_size>; };

    size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }

    [[ nodiscard ]]
    pointer allocate(size_type size) {
      #ifdef DEBUG
      std::cerr << "Allocate memory for " << size << " items." << std::endl;
      #endif

      if (cursor + size > chunk_size) throw std::bad_alloc();
      auto ptr { &chunk[cursor] };
      cursor += size;
      return ptr;
    }

    void deallocate(pointer ptr, size_type size) noexcept {
      #ifdef DEBUG
      std::cerr << "Query for deallocation, do nothing." << std::endl;
      #endif
    }

    template<typename U, typename ...Args>
    void construct(U *ptr, Args &&...args) {
        new(ptr) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U *ptr) { ptr->~U(); }

    // а для соответствия стандарту еще нужно
    // 1. функция address
    // 2. операторы сравнения == и !=
    private:
      pointer chunk;
      size_type cursor { 0 };
  };
}

#endif
