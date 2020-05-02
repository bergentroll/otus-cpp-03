#include <vector>
#include <gtest/gtest.h>
#include "allocator.h"

using namespace::std;

TEST(allocator, overflow) {
  vector<int, otus::Allocator<int, 1>> v;
  ASSERT_NO_THROW(v.push_back(0));
  ASSERT_THROW(v.push_back(0), std::bad_alloc);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
