#include "board.h"
#include "search.h"
#include "gtest/gtest.h"


// TEST has two parameters: the test case name and the test name.
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.

TEST(TreeSearch, BoardSize1) {
  Pattern<1> patt;
  char winner = search(Board<1>("-"), patt);

  EXPECT_EQ('X', winner);
  EXPECT_EQ(Board<1>::cellSet::make({0}), patt.cells());
}

TEST(TreeSearch, BoardSize2) {
  Board<2> b = Board<2>(R"(- -
                            - -)");
  b.go(1);
  Pattern<2> patt;
  // std::cout << "Searching for winner of" << std::endl << b << std::endl;
  char winner = search(b, patt);
  EXPECT_EQ('X', winner);
  EXPECT_EQ(Board<2>::cellSet::make({2, 3}), patt.cells());
  
  b = Board<2>(R"(- -
                   - -)");
  // std::cout << "Searching for winner of" << std::endl << b << std::endl;
  winner = search(b, patt);
  EXPECT_EQ('X', winner);
  EXPECT_EQ(Board<2>::cellSet::make({0, 1, 2}), patt.cells());
}

TEST(TreeSearch, BoardSize3) {
  Pattern<3> patt;
  Board<3> b = Board<3>(R"(- - -
                            - - -
                             - - -)");
  b.go(4);
  char winner = search(b, patt);
  // std::cout << "Searching for winner of" << std::endl << b << std::endl;
  EXPECT_EQ('X', winner);
  EXPECT_EQ(Board<3>::cellSet::make({1, 2, 6, 7}), patt.cells());

  b = Board<3>(R"(- - -
                   - - -
                    - - -)");
  // std::cout << "Searching for winner of" << std::endl << b << std::endl;
  winner = search(b, patt);
  EXPECT_EQ('X', winner);
  EXPECT_EQ(Board<3>::cellSet::make({1, 2, 4, 6, 7}), patt.cells());
}

TEST(TreeSearch, BoardSize4) {
  Pattern<4> patt;
  auto b = Board<4>(R"(O - - -
                        - X - -
                         - X - -
                          - - - -)");
  // std::cout << "Searching for winner of" << std::endl << b << std::endl;
  char winner = search(b, patt);
  EXPECT_EQ('X', winner);
  EXPECT_EQ(Board<4>::cellSet::make({1, 2, 12, 13}), patt.cells());

  b = Board<4>(R"(O - - X
                   - - - -
                    - - X -
                     - - - -)");
  b.setPlayer('O');
  // std::cout << "Searching for winner of" << std::endl << b << std::endl;
  winner = search(b, patt);
  EXPECT_EQ('X', winner);
  EXPECT_EQ(Board<4>::cellSet::make({6, 7, 13, 14}), patt.cells());
}
