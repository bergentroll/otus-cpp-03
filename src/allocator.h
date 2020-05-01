#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstdlib>
#include <climits>

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

    Allocator() = default;
    ~Allocator() = default;

    template<typename U>
    struct rebind {
      using other = Allocator<U, ssize>;
    };

    size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }

    [[ nodiscard ]]
    pointer allocate(size_type size) {
      auto ptr { std::malloc(size * sizeof(T)) };
      if (!ptr) throw std::bad_alloc();
      return reinterpret_cast<pointer>(ptr);
    }

    void deallocate(pointer ptr, size_type size) noexcept {
      std::free(ptr);
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
  };
}

#endif
