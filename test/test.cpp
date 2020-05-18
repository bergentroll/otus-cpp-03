#include <vector>
#include <gtest/gtest.h>
#include "allocator.h"
#include "container.h"

using namespace::std;

TEST(allocator, overflow) {
  vector<int, otus::Allocator<int, 1>> v;
  ASSERT_NO_THROW(v.push_back(0));
  ASSERT_NO_THROW(v.push_back(0));
}

TEST(allocator, assign_ctr) {
  vector<char, otus::Allocator<char, 3>> source { };
  source.push_back('a');
  source.push_back('b');
  source.push_back('c');
  source.push_back('\0');

  vector<char, otus::Allocator<char, 3>> v { source };

  ASSERT_EQ(v, source);
}

TEST(allocator, comparison) {
  vector<int, otus::Allocator<int, 3>> v1 { 1, 2, 3 };
  vector<int, otus::Allocator<int, 3>> v2 { 1, 2, 3 };

  EXPECT_NE(v1.get_allocator(), v2.get_allocator());

  otus::Allocator<int> alloc1 { };
  otus::Allocator<int> &alloc2 { alloc1 };
  EXPECT_EQ(alloc1, alloc2);
}

TEST(container, normal_usage) {
  otus::Container<int> c;
  c.push_back(1);
  c.push_back(2);

  EXPECT_EQ(c[0], 1);
  EXPECT_EQ(c[1], 2);

  c[0] = 3;

  EXPECT_EQ(c[0], 3);
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

  EXPECT_EQ(c1, c2);

  c2.push_back('X');
  EXPECT_NE(c1, c2);
}

TEST(container, eq_op) {
  otus::Container<char> c1 { 'a', 'b', 'c', '\n' };
  otus::Container<char> c2 { 'a', 'b', 'c' };

  EXPECT_NE(c1, c2);

  c2.push_back('\n');
  EXPECT_EQ(c1, c2);

  c1.push_back('1');
  c2.push_back('2');
  EXPECT_NE(c1, c2);
}

TEST(container, at) {
  otus::Container<long> c { 100, 1000, 10000 };
  EXPECT_NO_THROW(c.at(2));
  EXPECT_THROW(c.at(3), std::out_of_range);

  const otus::Container<long> c_const { 100, 1000, 10000 };
  EXPECT_NO_THROW(c_const.at(2));
  EXPECT_THROW(c_const.at(3), std::out_of_range);
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
