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

    Container(std::initializer_list<value_type> init) {
      for (const_reference item: init) push_back(item);
    }

    // TODO
    Container(const Container<T> &source);

    // TODO
    Container& operator=(const Container<T> &other);

    ~Container() {
      auto cursor { list_head };
      while (cursor) {
        cursor = list_head->next;
        allocator.deallocate(list_head, 1);
        list_head = cursor;
      }
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
        if ((*this)[0] != other[0]) return false;

      return true;
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
      log("Pushing back value " + std::to_string(item));

      auto ptr { allocator.allocate(1) };
      allocator.construct(ptr, Node { item });

      if (!list_head) {
        list_head = ptr;
        return;
      }

      auto cursor { list_head };
      while (cursor->next) { 
        log("VAL: " + std::to_string(cursor->data));
        cursor = cursor->next;
      }

      cursor->next = ptr;
    }

    allocator_type& get_allocator() { return allocator; }

    iterator begin() { return iterator(list_head); };
    iterator end() { return Iterator(nullptr); };

  private:
    struct Node {
      value_type data;
      Node *next { nullptr };
    };

    allocator_type allocator { };
    Node *list_head { nullptr };
  };
}

#endif
