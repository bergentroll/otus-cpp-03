#include <vector>
#include <gtest/gtest.h>
#include "allocator.h"
#include "container.h"

using namespace::std;

TEST(allocator, overflow) {
  vector<int, otus::Allocator<int, 1>> v;
  ASSERT_NO_THROW(v.push_back(0));
  ASSERT_THROW(v.push_back(0), std::bad_alloc);
}

TEST(container, normal_usage) {
  otus::Container<int> c;
  c.push_back(1);
  c.push_back(2);

  ASSERT_EQ(c[0], 1);
  ASSERT_EQ(c[1], 2);

  c[0] = 3;

  ASSERT_EQ(c[0], 3);
}

TEST(container, initializer_list) {
  otus::Container<float> c1 { 0.0, 0.1, 0.2 };

  otus::Container<float> c2 { };
  c2.push_back(0.0);
  c2.push_back(0.1);
  c2.push_back(0.2);

  ASSERT_EQ(c1, c2);
}

TEST(container, size) {
  otus::Container<char> c { 'a', 'b', 'c', '\n' };
  ASSERT_EQ(c.size(), 4);
}

TEST(container, assign_ctr) {
  otus::Container<char> source { 'a', 'b', 'c', '\n' };
  otus::Container<char> c { source };
  ASSERT_EQ(c, source);
}

TEST(container, assign_op) {
  otus::Container<char> c1 { 'a', 'b', 'c', '\n' };
  otus::Container<char> c2 { 'c', 'd', 'e' };
  c2 = c1;
  ASSERT_EQ(c1, c2);
}

TEST(container, at) {
  otus::Container<long> c { 100, 1000, 10000 };
  ASSERT_NO_THROW(c.at(2));
  ASSERT_THROW(c.at(3), std::out_of_range);

  const otus::Container<long> c_const { 100, 1000, 10000 };
  ASSERT_NO_THROW(c_const.at(2));
  ASSERT_THROW(c_const.at(3), std::out_of_range);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
