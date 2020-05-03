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

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
