#include "board.h"
#include "winner.h"
#include "../benchmark.h"

template<bitpos N>
struct winnerTest {
  char winner;
  char vcWinner;
  Board<N> board;
};

std::vector< winnerTest<2> > t2(std::initializer_list< winnerTest<2> >{
    {'?', '?', Board<2>(R"(
      - -
       - -)")},
    {'X', 'X', Board<2>(R"(
      X X
       X X)")},
});

std::vector< winnerTest<3> > t3(std::initializer_list< winnerTest<3> >{
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

std::vector< winnerTest<5> > t5(std::initializer_list< winnerTest<5> >{
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

std::vector< winnerTest<7> > t7(std::initializer_list< winnerTest<7> >{
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

std::vector< winnerTest<8> > t8(std::initializer_list< winnerTest<8> >{
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
    {'X', 'X', Board<8>(R"(
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

int globalSum = 0;

template<bitpos N>
void runWinnerTests(std::vector< winnerTest<N> > tests) {
  for (winnerTest<N> test : tests) {
    char w = test.board.winner('?');
    if (w == test.winner) {
      ++globalSum;
    }
  }
}

template<bitpos N>
void runSimpleVirtualWinnerTests(std::vector< winnerTest<N> > tests) {
  for (winnerTest<N> test : tests) {
    char w = test.board.virtualWinner('?');
    if (w == test.vcWinner) {
      ++globalSum;
    }
  }
}


void Benchmark_Winner(Benchmark &b) {
  globalSum = 0;
  for (int i = 0; i < b.N; ++i) {
    runWinnerTests(t2);
    runWinnerTests(t3);
    runWinnerTests(t5);
    runWinnerTests(t7);
    runWinnerTests(t8);
  }
}

void Benchmark_SimpleVirtualWinner(Benchmark &b) {
  globalSum = 0;
  for (int i = 0; i < b.N; ++i) {
    runSimpleVirtualWinnerTests(t2);
    runSimpleVirtualWinnerTests(t3);
    runSimpleVirtualWinnerTests(t5);
    runSimpleVirtualWinnerTests(t7);
    runSimpleVirtualWinnerTests(t8);
  }
}

int main() {
  BENCHMARK(Benchmark_Winner);
  BENCHMARK(Benchmark_SimpleVirtualWinner);
  return 0;
}
