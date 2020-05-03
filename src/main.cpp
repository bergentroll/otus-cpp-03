#include <cstdlib>
#include <iostream>
#include <map>

#include <vector>

#include "allocator.h"
#include "container.h"

using namespace std;

int fac(int x) { return x == 0? 1: x * fac(x - 1); }

template <typename T, typename Key, class Compare, class Allocator>
void print(const map<T, Key, Compare, Allocator> &m) {
  for (const auto &[key, val]: m)
    cout << key << ' ' << val << endl;
}

template <typename T, class Allocator>
void print(otus::Container<T, Allocator> &c) {
  for (const auto &val: c)
    cout << val << endl;
}

int main(int argc, char **argv) {
  map<int, int> m { };
  for (int i { 0 }; i < 10; i++) m[i] = fac(i);
  print(m);

  map<int, int, std::less<int>, otus::Allocator<pair<int, int>, 10>> m_al { };
  for (int i { 0 }; i < 10; i++) m_al[i] = fac(i);
  print(m_al);

  otus::Container<int> c { };
  for (int i { 0 }; i < 10; c.push_back(i++));
  print(c);

  otus::Container<int, otus::Allocator<int, 10>> c_al { };
  for (int i { 0 }; i < 10; c_al.push_back(i++));
  print(c_al);

  return EXIT_SUCCESS;
}
