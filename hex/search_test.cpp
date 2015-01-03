#include "board.h"
#include "search.h"
#include "gtest/gtest.h"


// TEST has two parameters: the test case name and the test name.
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.

TEST(TreeSearch, BoardSize1) {
  char winner;
  pattern<1> *patt;
  std::tie(winner, patt) = search(Board<1>("-"));

  EXPECT_EQ('X', winner);
  EXPECT_EQ(Board<1>::cellSet::make({0}), patt->via);
}

TEST(TreeSearch, BoardSize2) {
  char winner;
  pattern<2> *patt;
  std::tie(winner, patt) = search(Board<2>("-- --").go(1));

  EXPECT_EQ('X', winner);
  EXPECT_EQ(Board<2>::cellSet::range(0, 4), patt->via);
  //EXPECT_EQ(Board<2>::cellSet::make({2, 3}), patt->via);
}
