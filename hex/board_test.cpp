#include <sstream>
#include "board.h"
#include "gtest/gtest.h"


// TEST has two parameters: the test case name and the test name.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.


// Tests the default constructor for Board.
TEST(BoardConstruction, BoardDefaultConstructor) {
  // This test is named "BoardDefaultConstructor", and belongs to the
  // "BoardConstruction" test case.
  Board<4> b4;
  EXPECT_EQ(4, b4.size);
  EXPECT_EQ(15, b4.max);
  EXPECT_EQ(16, b4.count);
  EXPECT_EQ('X', b4.player());
  EXPECT_EQ("X:----------------", b4.stateString());
  EXPECT_EQ("X:----------------", b4.reverseStateString());

  Board<13> b13;
  EXPECT_EQ(13, b13.size);
  EXPECT_EQ(168, b13.max);
  EXPECT_EQ(169, b13.count);
  EXPECT_EQ('X', b13.player());
}

// Tests content constructor for Board.
TEST(BoardConstruction, BoardContentConstructor) {
  //auto all = cellset<1>::make({0});
  //auto nil = cellset<1>::make({});
  cellset<1> all, nil;
  all.reset().set(0);
  nil.reset();

  {
    Board<1> sample1("-");
    EXPECT_EQ('X', sample1.player());
    EXPECT_EQ('O', sample1.opponent());
    cellset<1> h{0}, v{0};
    EXPECT_EQ(v, sample1.verticalCells());
    EXPECT_EQ(h, sample1.horizontalCells());
    EXPECT_EQ("X:-", sample1.stateString());
    EXPECT_EQ("X:-", sample1.reverseStateString());
  }

  {
    Board<1> sample2("O");
    EXPECT_EQ('X', sample2.player());
    EXPECT_EQ('O', sample2.opponent());
    EXPECT_EQ(nil, sample2.verticalCells());
    EXPECT_EQ(all, sample2.horizontalCells());
    EXPECT_EQ(all, sample2.occupiedCells());
    EXPECT_EQ(nil, sample2.emptyCells());
    EXPECT_EQ(all, sample2.allCells());
    EXPECT_EQ(all, sample2.topRow());
    EXPECT_EQ(all, sample2.bottomRow());
    EXPECT_EQ(all, sample2.leftColumn());
    EXPECT_EQ(all, sample2.rightColumn());
    EXPECT_EQ("X:O", sample2.stateString());
    EXPECT_EQ("X:O", sample2.reverseStateString());
  }

  {
    Board<1> sample3("X");
    EXPECT_EQ('O', sample3.player());
    EXPECT_EQ('X', sample3.opponent());
    EXPECT_EQ(all, sample3.verticalCells());
    EXPECT_EQ(nil, sample3.horizontalCells());
    EXPECT_EQ(all, sample3.occupiedCells());
    EXPECT_EQ(nil, sample3.emptyCells());
    EXPECT_EQ("O:X", sample3.stateString());
    EXPECT_EQ("O:X", sample3.reverseStateString());
  }

  {
    Board<4> sample4(R"(
      - - - -
       - - X -
        - O - X
         X - - O)");
    cellset<4> x, o, e, n; // owned by X, O, Either, or Neither
    x.reset().set(6).set(11).set(12);
    o.reset().set(9).set(15);
    e.reset().set(6).set(9).set(11).set(12).set(15);
    n = cellset<4>::make({0,1,2,3,4,5,7,8,10,13,14});
    EXPECT_EQ('O', sample4.player());
    EXPECT_EQ('X', sample4.opponent());
    EXPECT_EQ(x, sample4.verticalCells());
    EXPECT_EQ(o, sample4.horizontalCells());
    EXPECT_EQ(e, sample4.occupiedCells());
    EXPECT_EQ(n, sample4.emptyCells());

    EXPECT_EQ(cellset<4>::make({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}),
              sample4.allCells());
    EXPECT_EQ(cellset<4>::make({0,1,2,3}), sample4.topRow());
    EXPECT_EQ(cellset<4>::make({12,13,14,15}), sample4.bottomRow());
    EXPECT_EQ(cellset<4>::make({0,4,8,12}), sample4.leftColumn());
    EXPECT_EQ(cellset<4>::make({3,7,11,15}), sample4.rightColumn());

    EXPECT_EQ("O:------X--O-XX--O", sample4.stateString());
    EXPECT_EQ("O:O--XX-O--X------", sample4.reverseStateString());
  }
}

// Tests the management of board state.
TEST(BoardState, StateManagement) {
  Board<4> b;
  EXPECT_EQ('X', b.player());
  EXPECT_EQ('O', b.opponent());
  EXPECT_EQ(cellset<4>{0}, b.verticalCells());
  EXPECT_EQ(cellset<4>{0}, b.horizontalCells());
  b.setPlayer('O');
  EXPECT_EQ('O', b.player());
  EXPECT_EQ('X', b.opponent());
  b.setPlayer('X');
  EXPECT_EQ('X', b.player());
  EXPECT_EQ('O', b.opponent());
}


// Tests Board stream printing.
TEST(BoardState, BoardPrinting) {
  {
    Board<3> b(R"(- - X
                   X O X
                    - O O)");
    EXPECT_EQ("X:--XXOX-OO", b.stateString());
    EXPECT_EQ("X:OO-XOXX--", b.reverseStateString());
    std::ostringstream stream;
    stream << std::endl; b.IndentedRender(stream, "      HEX!   ");
    std::string expected = R"(
      HEX!     a b c
      HEX!   1: - - X
      HEX!    2: X O X
      HEX!     3: - O O
)";
    EXPECT_EQ(expected, stream.str());
  }

  {
    std::ostringstream stream;
    stream << std::endl;
    Board<6>::MappedRender(stream, "      ", {
      { 'O', cellset<6>::make({ 1, 2, 3, 4, 7, 8, 9, 13 }) },
      { '*', cellset<6>::make({ 14 }) },
    });
    std::string expected = R"(
        a b c d e f
      1: - O O O O -
       2: - O O O - -
        3: - O * - - -
         4: - - - - - -
          5: - - - - - -
           6: - - - - - -
)";
    EXPECT_EQ(expected, stream.str());
  }
}

template<bitpos N>
struct playAndResult {
  int at;  // plays[0].at is ignored; instead, a board is constructed
  bool panics;
  char player;
  std::initializer_list<int> vertical, horizontal;
  std::string state;
};

template<bitpos N>
struct boardStateTest {
  std::string initialContent;
  std::initializer_list< playAndResult<N> > plays;
};

template<bitpos N>
void runBoardStateTest(boardStateTest<N> d) {
  int index = -1, len = d.plays.size();
  Board<N> b;
  for (auto m : d.plays) {
    ++index;
    bool panicked = false;
    try {
      if (index == 0) {
        b = Board<N>(d.initialContent);
      } else {
        b.go(m.at);
      }
    } catch (const Panic& p) {
      panicked = true;
      if (!m.panics) {
        std::cout << "unexpected ";
      }
    }

    EXPECT_EQ(m.panics, panicked);

    if (m.panics || panicked) {
      return;
    }

    char player = m.player;
    char opponent = player == 'X' ? 'O' : 'X';
    cellset<N> vert, horz;
    vert.reset(), horz.reset();
    for (auto i : m.vertical) { vert.set(i); }
    for (auto i : m.horizontal) { horz.set(i); }

    EXPECT_EQ(player, b.player());
    EXPECT_EQ(opponent, b.opponent());
    EXPECT_EQ(vert, b.verticalCells());
    EXPECT_EQ(horz, b.horizontalCells());
    EXPECT_EQ(m.state, b.stateString());

    std::string reversed = m.state;
    std::reverse(reversed.begin()+2, reversed.end());
    EXPECT_EQ(reversed, b.reverseStateString());
  }
}

template<bitpos N>
void runBoardStateTests(std::initializer_list< boardStateTest<N> > tests) {
  for (auto data : tests) {
    runBoardStateTest<N>(data);
  }
}

// Tests Board plays.
TEST(BoardPlay, BoardSize1) {
  runBoardStateTests<1>(std::initializer_list< boardStateTest<1> >{
    {"-", {
      {-1, false, 'X', {}, {}, "X:-"},
      {0, false, 'O', {0}, {}, "O:X"},
      {.at=0, .panics=true}}},
    {"X", {
      {-1, false, 'O', {0}, {}, "O:X"},
      {.at=0, .panics=true}}},
    {"O", {
      {-1, false, 'X', {}, {0}, "X:O"},
      {.at=0, .panics=true}}},
    {"W", {
      {.at=-1, .panics=true}}},
    {"--", {
      {.at=-1, .panics=true}}},
  });
}

TEST(BoardPlay, BoardSize2) {
  runBoardStateTests<2>(std::initializer_list< boardStateTest<2> >{
    {"----", {
      {-1, false, 'X', {}, {}, "X:----"},
      {3, false, 'O', {3}, {}, "O:---X"},
      {1, false, 'X', {3}, {1}, "X:-O-X"},
      {0, false, 'O', {0,3}, {1}, "O:XO-X"},
      {2, false, 'X', {0,3}, {1,2}, "X:XOOX"},
      { .at = 3, .panics=true }}},
    {"-X--", {
      {-1, false, 'O', {1}, {}, "O:-X--"},
      {2, false, 'X', {1}, {2}, "X:-XO-"},
      {0, false, 'O', {0,1}, {2}, "O:XXO-"},
      { .at = 1, .panics=true }}},
    {"O", {
      { .at = -1, .panics = true }}},
    {"XOXa", {
      { .at = -1, .panics = true }}},
    {"XOXX", {
      {-1, false, 'O', {0,2,3}, {1}, "O:XOXX"},
      { .at = 2, .panics = true }}},
    {"OOOO", {
      {-1, false, 'X', {}, {0,1,2,3}, "X:OOOO"},
      { .at = 1, .panics = true }}},
    {"XOX", {
      { .at = -1, .panics = true }}},
    {"--", {
      { .at = -1, .panics = true }}},
  });
}

TEST(BoardPlay, BoardSize5) {
  runBoardStateTests<5>(std::initializer_list< boardStateTest<5> >{
    {R"(X - - - -
         - - - - -
          - - - - -
           - - - - -
            - - - - O)", {
      {-1, false, 'X', {0}, {24}, "X:X-----------------------O"},
      {12, false, 'O', {0,12}, {24}, "O:X-----------X-----------O"},
      {7,  false, 'X', {0,12}, {7,24}, "X:X------O----X-----------O"},
    }},
  });
}
