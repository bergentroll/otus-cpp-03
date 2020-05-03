#ifndef CONTAINER_H
#define CONTAINER_H

#include <memory>

namespace otus {
  template <typename T, class Allocator = std::allocator<T>>
  class Container {
    struct Node;

  public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    //using allocator_type = Allocator;
    using allocator_type = typename Allocator::template rebind<Node>::other;

    class Iterator {
    public:
      reference operator*() const { }
      void operator++() { }
    private:
    };

    using iterator = Iterator;

    Container() { }

    Container(std::initializer_list<value_type> list) { }

    Container(const Container<T> &source);

    Container& operator=(const Container<T> &other);

    ~Container() {
      auto cursor { list_head };
      while (cursor) {
        cursor = list_head->next;
        allocator.deallocate(list_head, 1);
        list_head = cursor;
      }
    }

    reference operator[](std::size_t i) { }

    reference at(std::size_t i) { }

    void push_back(value_type item) {
      auto ptr { allocator.allocate(1) };
      allocator.construct(ptr, Node { item });
      if (!list_head) {
        list_head = ptr;
        list_cursor = list_head;
      } else {
        list_cursor->next = ptr;
        list_cursor = list_cursor->next;
      }
    }

    allocator_type& get_allocator() { return allocator; }

    iterator begin() { };
    iterator end() { };

  private:
    struct Node {
      reference data;
      Node *next { nullptr };
    };

    allocator_type allocator { };
    Node *list_head { nullptr }, *list_cursor { nullptr };
  };
}

#endif
