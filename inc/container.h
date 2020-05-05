#ifndef CONTAINER_H
#define CONTAINER_H

#include <memory>

#include "logger.h"

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
    using allocator_type = typename Allocator::template rebind<Node>::other;

    class Iterator {
    public:
      Iterator(Node *node): node(node) { }

      reference operator*() const { return node->data; }

      void operator++() { node = node-> next; }

      bool operator==(const Iterator &other) const {
        return node == other.node;
      }

      bool operator!=(const Iterator &other) const {
        return !(*this == other);
      }

    private:
      Node *node;
    };

    using iterator = Iterator;

    Container() { }

    // TODO Move semantics.
    // TODO Smart pointers.

    Container(std::initializer_list<value_type> init) {
      for (const_reference item: init) push_back(item);
    }

    Container(const Container<T> &source) {
      for (const_reference item: source)
        push_back(item);
    }

    Container& operator=(const Container<T> &other) {
      deconstruct(); 

      for (const_reference item: other) push_back(item);
      return *this;
    }

    ~Container() {
      deconstruct();
    }

    size_type size() const {
      size_type size { };
      auto cursor { list_head };
      while (cursor) {
        cursor = cursor->next;
        size++;
      }
      return size;
    }

    bool operator==(const Container &other) const {
      if (size() != other.size()) return false;

      for (size_type i { }; i < size(); i++)
        if (operator[](0) != other[0]) return false;

      return true;
    }

    bool operator!=(const Container &other) const {
      return !((*this) == other);
    }

    reference operator[](std::size_t pos) {
      return const_cast<reference>(static_cast<const Container &>(*this)[pos]);
    }

    const_reference operator[](std::size_t pos) const {
      auto cursor { list_head };
      for (size_t i { 0 }; i < pos; i++) {
        cursor = cursor->next;
      }
      return cursor->data;
    }

    reference at(std::size_t pos) {
      return const_cast<reference>(static_cast<const Container &>(*this).at(pos));
    }

    const_reference at(std::size_t pos) const { 
      if (pos >= size())
        throw std::out_of_range("Index " + std::to_string(pos) + " is out of range.");
      return operator[](pos);
    }

    void push_back(value_type item) {
      LOG("Pushing back value " << item << '.' << std::endl);

      auto ptr { allocator.allocate(1) };
      allocator.construct(ptr, Node { item });

      if (!list_head) {
        list_head = ptr;
        return;
      }

      auto cursor { list_head };
      while (cursor->next) { 
        cursor = cursor->next;
      }

      cursor->next = ptr;
    }

    allocator_type& get_allocator() { return allocator; }

    iterator begin() { return iterator(list_head); };

    iterator end() { return Iterator(nullptr); };

    const iterator begin() const { return iterator(list_head); };

    const iterator end() const { return Iterator(nullptr); };

  private:
    struct Node {
      value_type data;
      Node *next { nullptr };
    };

    allocator_type allocator { };
    Node *list_head { nullptr };

    void deconstruct() {
      auto cursor { list_head };
      while (cursor) {
        cursor = list_head->next;
        allocator.deallocate(list_head, 1);
        list_head = cursor;
      }
    }
  };
}

#endif
