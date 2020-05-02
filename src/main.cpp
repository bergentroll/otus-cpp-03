#include <cstdlib>
#include <iostream>
#include <map>

#include <vector>

#include "allocator.h"

using namespace std;

int fac(int x) { return x == 0? 1: x * fac(x - 1); }

template <typename T>
void print(const T &t) {
  for (const auto &[key, val]: t)
    cout << key << ' ' << val << endl;
}

int main(int argc, char **argv) {
  map<int, int> m { };
  for (int i { 0 }; i < 10; i++) m[i] = fac(i);
  print(m);

  map<int, int, std::less<int>, otus::Allocator<pair<int, int>, 10>> m_al { };
  for (int i { 0 }; i < 10; i++) m_al[i] = fac(i);
  print(m_al);

  //container<int> c { };
  //for (int i { 0 }; i < 10; c.push_back(i++));
  //print(c);

  //container<int> c_al { };
  //for (int i { 0 }; i < 10; c_al.push_back(i++));
  //print(c_al);

  return EXIT_SUCCESS;
}
