#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstdio>

#include <cstdlib>
#include <limits>
#include <new>

namespace otus {
  template <typename T, int ssize = 0>
  class Allocator {
  public:
    using value_type =T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

    Allocator() {
      pool = (pointer)calloc(ssize, sizeof(T));
    }

    ~Allocator() {
      free(pool);
    }

    template<typename U>
    struct rebind { using other = Allocator<U, ssize>; };

    size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }

    [[ nodiscard ]]
    pointer allocate(size_type size) {
      printf("Allocating %i items.\n", size);
      if (cursor + size > ssize) throw std::bad_alloc();
      auto ptr { &pool[cursor] };
      cursor += size;
      return ptr;
    }

    void deallocate(pointer ptr, size_type size) noexcept { }

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
      pointer pool { nullptr };
      size_type cursor { 0 };
  };
}

#endif
