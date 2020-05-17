#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <limits>
#include <new>
#include <map>

#include "logger.h"

namespace otus {
  /// TODO Use smart pointers.
  template <typename T, size_t base_chunk_size = 1>
  class Allocator {
  public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

    template<typename U>
    struct rebind { using other = Allocator<U, base_chunk_size>; };

    Allocator():
    list_head(create_chunk(base_chunk_size)),
    list_cursor(list_head),
    chunk_index(0) {
      LOG("Allocator::Allocator()." << std::endl);
    }

    Allocator(const Allocator &other): Allocator() {
      LOG("Allocator::Allocator(Allocator &other)." << std::endl);
    }

    /// TODO
    Allocator(const Allocator &&other) = delete;

    /// TODO
    Allocator & operator=(const Allocator &other) = delete;

    /// TODO
    Allocator & operator=(const Allocator &&other) = delete;

    ~Allocator() {
      LOG("Deleting pool." << std::endl);

      list_cursor = list_head;
      Node *list_prev { nullptr };

      while (list_cursor) {
        LOG("|-Deleting chunk." << std::endl);
        operator delete[](list_cursor->chunk);
        list_prev = list_cursor;
        list_cursor = list_cursor->next;
        delete list_prev;
      }
    }

    static size_type max_size(const Allocator &a) noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    [[ nodiscard ]]
    pointer allocate(size_type size) {
      LOG("Allocate memory for " << size << " items." << std::endl);

      if (size > list_cursor->chunk_size - chunk_index) {
        list_cursor->next = new Node;
        list_cursor = list_cursor->next;
        auto new_chunk_size { std::max(base_chunk_size, size) };

        LOG("|-Allocating new chunk of size " << new_chunk_size << std::endl);

        list_cursor->chunk =
          reinterpret_cast<pointer>(
            operator new[](new_chunk_size * sizeof(value_type)));
        list_cursor->chunk_size = new_chunk_size;
        chunk_index = 0;
      }

      auto ptr { &list_cursor->chunk[chunk_index] };
      chunk_index += size;
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
      if (this == &other) return true;
      return false;
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
      struct Node {
        pointer chunk;
        size_type chunk_size;
        Node *next { nullptr };
      };

      Node *list_head;
      Node *list_cursor;
      size_type chunk_index;

      [[ nodiscard ]]
      Node * create_chunk(size_type size) {
        auto ptr { new Node };
        ptr->chunk =
          reinterpret_cast<pointer>(
              operator new[](size * sizeof(value_type)));
        ptr->chunk_size = size;
        return ptr;
      }

  };
}

#endif
