#include <sstream>
#include <vector>
#include "board.h"
#include "winner.h"
#include "gtest/gtest.h"


// TEST has two parameters: the test case name and the test name.
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.

TEST(BoardWinner, BoardSize1) {
  auto test1 = Board<1>("-");
  EXPECT_EQ('?', test1.winner('X'));
  EXPECT_EQ('?', test1.winner('O'));
  EXPECT_EQ('?', test1.winner('?'));
  EXPECT_EQ('?', test1.virtualWinner('X'));
  EXPECT_EQ('?', test1.virtualWinner('O'));
  EXPECT_EQ('?', test1.virtualWinner('?'));

  auto test2 = Board<1>("X");
  EXPECT_EQ('X', test2.winner('X'));
  EXPECT_EQ('?', test2.winner('O'));
  EXPECT_EQ('X', test2.winner('?'));
  EXPECT_EQ('X', test2.virtualWinner('X'));
  EXPECT_EQ('?', test2.virtualWinner('O'));
  EXPECT_EQ('X', test2.virtualWinner('?'));

  auto test3 = Board<1>("O");
  EXPECT_EQ('?', test3.winner('X'));
  EXPECT_EQ('O', test3.winner('O'));
  EXPECT_EQ('O', test3.winner('?'));
  EXPECT_EQ('?', test3.virtualWinner('X'));
  EXPECT_EQ('O', test3.virtualWinner('O'));
  EXPECT_EQ('O', test3.virtualWinner('?'));
}

TEST(BoardWinner, BoardSize2) {
  Board<2> b2;
  b2 = Board<2>("----");
  auto test1 = b2;
  //auto test1 = Board<2>("----");
  EXPECT_EQ('?', test1.winner('X'));
  EXPECT_EQ('?', test1.winner('O'));
  EXPECT_EQ('?', test1.winner('?'));
  EXPECT_EQ('?', test1.virtualWinner('X'));
  EXPECT_EQ('?', test1.virtualWinner('O'));
  EXPECT_EQ('?', test1.virtualWinner('?'));

  auto test2 = Board<2>(R"(X X
                            O -)");
  EXPECT_EQ('?', test2.winner('X'));
  EXPECT_EQ('?', test2.winner('O'));
  EXPECT_EQ('?', test2.winner('?'));
  EXPECT_EQ('?', test2.virtualWinner('X'));
  EXPECT_EQ('?', test2.virtualWinner('O'));
  EXPECT_EQ('?', test2.virtualWinner('?'));

  auto test3 = Board<2>(R"(X X
                            O O)");
  EXPECT_EQ('?', test3.winner('X'));
  EXPECT_EQ('O', test3.winner('O'));
  EXPECT_EQ('O', test3.winner('?'));
  EXPECT_EQ('?', test3.virtualWinner('X'));
  EXPECT_EQ('O', test3.virtualWinner('O'));
  EXPECT_EQ('O', test3.virtualWinner('?'));

  auto test4 = Board<2>(R"(X X
                            X O)");
  EXPECT_EQ('X', test4.winner('X'));
  EXPECT_EQ('?', test4.winner('O'));
  EXPECT_EQ('X', test4.winner('?'));
  EXPECT_EQ('X', test4.virtualWinner('X'));
  EXPECT_EQ('?', test4.virtualWinner('O'));
  EXPECT_EQ('X', test4.virtualWinner('?'));
}

TEST(BoardWinner, BoardSize3) {
  auto test1 = Board<3>(R"(
    - - -
     - - -
      - - -)");
  EXPECT_EQ('?', test1.winner('X'));
  EXPECT_EQ('?', test1.winner('O'));
  EXPECT_EQ('?', test1.winner('?'));
  EXPECT_EQ('?', test1.virtualWinner('X'));
  EXPECT_EQ('?', test1.virtualWinner('O'));
  EXPECT_EQ('?', test1.virtualWinner('?'));

  auto test2 = Board<3>(R"(
    - - X
     - X -
      - X -)");
  EXPECT_EQ('X', test2.winner('X'));
  EXPECT_EQ('?', test2.winner('O'));
  EXPECT_EQ('X', test2.winner('?'));
  EXPECT_EQ('X', test2.virtualWinner('X'));
  EXPECT_EQ('?', test2.virtualWinner('O'));
  EXPECT_EQ('X', test2.virtualWinner('?'));

  auto test3 = Board<3>(R"(
    - - X
     - - -
      - X -)");
  EXPECT_EQ('?', test3.winner('?'));
  EXPECT_EQ('?', test3.winner('O'));
  EXPECT_EQ('?', test3.winner('?'));
  EXPECT_EQ('X', test3.virtualWinner('X'));
  EXPECT_EQ('?', test3.virtualWinner('O'));
  EXPECT_EQ('X', test3.virtualWinner('?'));

  auto test4 = Board<3>(R"(
    O - X
     - O O
      - X -)");
  EXPECT_EQ('?', test4.winner('X'));
  EXPECT_EQ('?', test4.winner('O'));
  EXPECT_EQ('?', test4.winner('?'));
  EXPECT_EQ('?', test4.virtualWinner('X'));
  EXPECT_EQ('O', test4.virtualWinner('O'));
  EXPECT_EQ('O', test4.virtualWinner('?'));
}

template<bitpos N>
struct winnerTest {
  char winner;
  char vcWinner;
  Board<N> board;
};

std::vector< winnerTest<2> > tests2(std::initializer_list< winnerTest<2> >{
    {'?', '?', Board<2>(R"(
      - -
       - -)")},
    {'X', 'X', Board<2>(R"(
      X X
       X X)")},
});

std::vector< winnerTest<3> > tests3(std::initializer_list< winnerTest<3> >{
    {'?', '?', Board<3>(R"(
      - - -
       - - -
        - - -)")},
    {'?', '?', Board<3>(R"(
      - X -
       O X O
        - O X)")},
    {'X', 'X', Board<3>(R"(
      - X -
       - X -
        - X -)")},
    {'O', 'O', Board<3>(R"(
      O X O
       O X O
        O O O)")},
});

std::vector< winnerTest<5> > tests5(std::initializer_list< winnerTest<5> >{
    {'?', 'X', Board<5>(R"(
      - - - - X
       - - X - -
        - - X - -
         - - X - -
          - - X - -)")},
    {'?', '?', Board<5>(R"(
      - - - - -
       - - X - -
        - - X - -
         - - X - -
          - - - - -)")},
    {'?', '?', Board<5>(R"(
      - - - - -
       - - X - -
        - - X - -
         - - - - -
          - - - - -)")},
});

std::vector< winnerTest<7> > tests7(std::initializer_list< winnerTest<7> >{
    {'X', 'X', Board<7>(R"(
      X - - - - - -
       X X X X X X X
        - - - - - - X
         X X X X X X X
          X - - - - - -
           X X X X X X X
            - - - - - - X
                          )")},
    {'X', 'X', Board<7>(R"(
      X X X X X X X
       - - - - - - X
        X X X X X X X
         X - - - - - -
          X X X X X X X
           - - - - - - X
            X X X X X X X
                          )")},
    {'O', 'O', Board<7>(R"(
      O - O O O - O
       O - O - O - O
        O - O - O - O
         O - O - O - O
          O - O - O - O
           O - O - O - O
            O O O - O O O
                          )")},
    {'?', 'X', Board<7>(R"(
      - - X - X - -
       - X - - X - -
        - X - - X - -
         X - - - X - -
          X X - - - - -
           - - - - X - -
            - X - X - - -
                          )")},
    {'?', 'O', Board<7>(R"(
      - - - - - - O
       - - - - - O -
        - - - - O - -
         - - O - - - -
          - O - - - - -
           O - - - - - -
            - - - - - - -
                          )")},
});

std::vector< winnerTest<8> > tests8(std::initializer_list< winnerTest<8> >{
    {'?', '?', Board<8>(R"(
      - - X X - X X -
       - - - - - X O O
        - X X - X O X -
         X - X X - O O -
          X - - O - X O -
           X X X - - X - -
            - O - - X - O -
             O - - X - - - -
                             )")},
    {'?', '?', Board<8>(R"(
   - - X X - X X -
    - - - - - X O O
     - X X - X O X -
      X - O O - O - -
       X O - O X O - -
        X O X - - X - -
         - O - - X - O -
          O - - X - - - -
                          )")},
    {'O', 'O', Board<8>(R"(
   - - X X - X X -
    - - - - - X O O
     - X X - X O X -
      X - O O - O - -
       X O - O X O - -
        X O X O O X - -
         - O - - X - O -
          O - - X - - - -
                          )")},
    {'X', 'X', Board<8>(R"(
   - - X X - X X -
    - - - - - X O O
     - X X - X O X -
      X - X X - O O -
       X - - O X X O -
        X X X X - X - -
         - O - - X - O -
          O - - X - - - -
                          )")},
    {'?', '?', Board<8>(R"(
   X - - - - - - -
    X X X X X X X X
     - - - - - - - X
      - - - - - - X X
       X X X X - - - -
        X - - - - - - -
         X X X X X X X X
          - - - - - - - X
                          )")},
    {'?', '?', Board<8>(R"(
    X X X X X X X X
     - - - - - - - X
      X X X X X X X X
       - - - - - - - -
        - - - - - - - -
         X X X X X X X X
          - - - - - - - X
           X X X X X X X X
                          )")},
    {'?', '?', Board<8>(R"(
    O O - - O O O -
     - O - - O - O -
      - O - - - - O -
       - O - - - - O -
        - O - O - - O -
         - O - O - - O -
          - O - O - - O -
           - O O O - - O O
                          )")},
    {'?', 'O', Board<8>(R"(
    O O - - O O O -
     - O - - O - O -
      - O - - - - O -
       - O - O - - O -
        - O - O - - O -
         - O - O - - O -
          - O - O - - O -
           - O O O - - O O
                          )")},
    {'?', 'X', Board<8>(R"(
   - - X X - X X -
    - - - - - X O O
     - X X - X O X -
      X - X X - O O -
       X - - O - X O -
        X X X X - X - -
         - O - - X - O -
          O - - X - - - -
                          )")},
    {'?', 'O', Board<8>(R"(
   - - X X - X X -
    - - - - - X O O
     - X X - X O X -
      X - O O - O - -
       X O - O X O - -
        X O X - O X - -
         - O - - X - O -
          O - - X - - - -
                          )")},
    {'?', 'O', Board<8>(R"(
    O O O O - O O O
     - - - O - O - -
      - - O - - O O -
       - O - O - - O -
        - O - O O - O -
         - O - - O - O -
          - O O - O - O -
           - - O - O O - -
                          )")},
});

template<bitpos N>
void runWinnerTests(std::vector< winnerTest<N> > tests) {
  int index = -1;
  for (auto test : tests) {
    ++index;
    auto w = test.board.winner('?');
    if (w != test.winner) {
      std::cout << "Expected winner " << test.winner << " but got "
        << w << " for this game state:" << std::endl;
      test.board.IndentedRender(std::cout, "    ");
    }
    EXPECT_EQ(test.winner, test.board.winner('?'));
  }

  index = -1;
  for (auto test : tests) {
    ++index;
    auto w = test.board.virtualWinner('?');
    if (w != test.vcWinner) {
      std::cout << "Expected virtual winner " << test.vcWinner << " but got "
        << w << " for this game state:" << std::endl;
      test.board.IndentedRender(std::cout, "    ");
    }
    EXPECT_EQ(test.vcWinner, test.board.virtualWinner('?'));
  }
}


TEST(BoardWinner, WinnerTestLists) {
  runWinnerTests(tests2);
  runWinnerTests(tests3);
  runWinnerTests(tests5);
  runWinnerTests(tests7);
  runWinnerTests(tests8);
}
