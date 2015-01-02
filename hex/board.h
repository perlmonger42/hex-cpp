#include <algorithm>
#include <iostream>
#include <sstream>
#include <locale>
#include <cstdarg>
#include <map>
#include "panic.h"
#include "cellset.h"

template<bitpos N>
using PrintMap = std::map< char, cellset<N> >;

// A Board represents the state of a Hex game, including information about
// whose turn it is to play, and which cells are owned by each player.
template<bitpos N>
class Board {
public:
  static constexpr int size = N; // Edge length -- a board is size×size cells.
  static constexpr int max = N*N-1; // Cell numbers run from 0 to max.
  static constexpr int count = N*N; // The number of cells on an N×N board.
  typedef cellset<N> cellSet; // The type of a set of cell-numbers.

private:
  char play; // 'X' if vertical to play, 'O' if horizontal
  cellSet vert; // Bitset of cells owned by the vertical player.
  cellSet horz; // Bitset of cells owned by the horizontal player.

public:
  // Board() creates an empty board, with X to play.
  Board() : play('X'), vert{0}, horz{0} { }

  // Board(string) creates a game from a textual representation of a game
  // state.  The string may contain whitespace, which is ignored, and that
  // makes it possible to create a visual representation of a board state.
  // For example:
  //
  //     board1 := hex.NewFromState(`
  //       O O O O O
  //        O O O O O
  //         O O - X -
  //          O - - - -
  //           - - O - -
  //     `)
  //
  // Empty cells are represented in the input string by a period ('.'), hyphen
  // ('-'), or middle dot ('·'). Cells owned by the vertical player are
  // indicated with an 'X', and cells owned by the horizontal player are an
  // 'O'.  Any other character induces panic.
  //
  // If there are more X's than O's, the game state will indicate that it is O's
  // turn to play; otherwise, X's. SetPlayer() may be used to override that
  // result.
  //
  // Note the orientation implied by the example above: the gameboards
  // represented by type Board "lean to the left". The following input will be
  // accepted, and no error will be detected, but the implied layout is WRONG:
  //     board2 := hex.NewFromState(`
  //          O O O O O
  //         O O O O O
  //        O O - X -
  //       O - - - -
  //      - - O - -
  //     `)
  // board1 and board2 represent exactly the same game state, and that state is
  // the left-leaning board shown by board1's state string.
  Board(std::string content);

  // Returns the player whose turn it is to move:
  // 'X' if vertical, 'O' if horizontal.
  char player() const { return play; }

  // Returns the opponent of player(): 'O' if it is X's turn, else 'X'.
  char opponent() const { return 'X' + 'O' - play; }

  // Set the current player. Argument must be 'X' or 'O'.
  Board<N>& setPlayer(char newPlayer) {
    if (newPlayer == 'X' || newPlayer == 'O') {
      play = newPlayer;
    } else {
      panic("Board.setPlayer(p): p must be 'X' or 'O'; was '%c'\n", newPlayer);
    }
    return *this;
  }

  // go(at) updates the Board to represent the game state after the current
  // player claims the named cell. The cell ownership information is updated,
  // and the player() toggles.
  //
  // Cells are numbered from zero, left to right, top to bottom, starting in the
  // upper left corner, like so:
  //   0 1 2
  //    3 4 5
  //     6 7 8
  //
  // Panics if `at` is an illegal index, or if `at` is already owned.
  Board<N>& go(int at) {
    if (at < 0 || at > max) {
      panic("Board.go(%d): cell numbers must be between 0 and %d", at, max);
    } else if (vert.test(at)) {
      panic("Board.go(%d) by %c: X already owns that cell", at, play);
    } else if (horz.test(at)) {
      panic("Board.play(%d) by %c: O already owns that cell", at, play);
    }

    if (play == 'X') {
      vert.set(at);
      play = 'O';
    } else {
      horz.set(at);
      play = 'X';
    }

    return *this;
  }


  // winner() returns 'X', 'O', or '?', depending on whether the game has been
  // won or not, and by whom (vertical == 'X', horizontal == 'O').
  // If testFor is 'X', only a vertical win is tested, and if it is not found,
  // winner() will return '?' even if there is a win for horizontal.
  // If testFor is 'O', only a horizontal win is tested, and if it is not
  // found, winner() will return '?' even if there is a win for vertical.
  // If testFor is any other char, a win will be sought for either player, and
  // the return value will reflect any win found.
  char winner(char testFor='?') const;
  char virtualWinner(char testFor='?') const;

  cellSet verticalCells() const { return this->vert; }
  cellSet horizontalCells() const { return this->horz; }
  cellSet occupiedCells() const { return this->vert | this->horz; }
  cellSet emptyCells() const { return allCells() - occupiedCells(); }
  static inline constexpr cellSet allCells() { return cellSet::universe(); }
  static inline constexpr cellSet topRow() { return cellSet::top(); }
  static inline constexpr cellSet bottomRow() { return cellSet::bottom(); }
  static inline constexpr cellSet leftColumn() { return cellSet::left(); }
  static inline constexpr cellSet rightColumn() { return cellSet::right(); }

  std::string stateString() const;
  std::string reverseStateString() const;

  // MappedRender writes to w a textual representation of a game board,
  // including column and row labels. The map argument associates chars with
  // bitsets, and a cell is rendered by the char corresponding to the bitset that
  // contains it, or '-' if the cell is not in any of the bitsets. The indent
  // argument is printed before each line of output, and is nominally a string
  // of whitespace.
  //
  //      a b c d
  //    1: - - O -
  //     2: - X O -
  //      3: - - X -
  //       4: - - - -
  static std::ostream& MappedRender(
      std::ostream& w, std::string indent, PrintMap<N> show);

  // Equivalent to MappedRender with a PrintMap that uses 'X' for the vertical
  // player's cells and 'O' for the horizontal player's cells.
  std::ostream& IndentedRender(std::ostream& w, std::string indent) const {
    //PrintMap<N> show;
    //show['X'] = this->vert;
    //show['O'] = this->horz;
    //return this->MappedRender(w, indent, show);
    return this->MappedRender(w, indent, {{'X', this->vert}, {'O', this->horz}});
  }

  // Equivalent to IndentedRender with an ‹indent› of "".
  std::ostream& Render(std::ostream& w) const {
    return this->IndentedRender(w, "");
  }

private:
  bool verticalHasWon() const;
  bool horizontalHasWon() const;
  static bool isConnected(cellset<N> start, cellset<N> owned, cellset<N> stop);
  bool verticalHasVirtualConnection() const;
  bool horizontalHasVirtualConnection() const;
  static bool isVirtuallyConnected(
      cellset<N> start, cellset<N> owned, cellset<N> stop, cellset<N> vacant);

  static_assert (N >= 1 && N <= 13, "Board size must be in 1..13");
};

template<bitpos N> const int Board<N>::size; // Edge length of the board.
template<bitpos N> const int Board<N>::max; // Cell numbers run from 0 to max.
template<bitpos N> const int Board<N>::count; // Count of cells on the board.

// Writes to w a formatted view of a game board, including column and row
// labels. Cells owned by the vertical player are indicated by an X, those
// owned by the horiontal player are an O, and the rest are shown as a hyphen
// ('-'). 
//
// Equivalent to MappedRender with an empty indent string and the PrintMap
// {{'X', board.vert}, {'O', board.horz}}.
//
// See MappedRender for an example of the output generated by this function.
template<bitpos N>
std::ostream& operator<< (std::ostream &w, const Board<N> &board) {
  return board.Render(w);
}

template<bitpos N>
Board<N>::Board(std::string content) {
  vert = cellSet{0};
  horz = cellSet{0};
  content.erase(
    std::remove_if(content.begin(), content.end(), 
      [](char ch) { return std::isspace<char>(ch, std::locale::classic()); }),
    content.end());

  if (content.size() != N*N) {
    panic("Board(content): content.size() is %lu; want N×N (%d)",
        content.size(), N*N);
  }

  int x = 0, o = 0, i = -1;
  for (char c : content) {
    i++;
    switch (c) {
    case 'X':
      // X's goal is to connect the top edge to the bottom edge (vertical).
      x++;
      // std::cout << "Adding cell " << i << " to vert " << std::endl
      //           << "  was: " << this->vert << std::endl;
      this->vert.set(i);
      // std::cout << "  now: " << this->vert << std::endl;
      break;
    case 'O':
      o++;
      // O's goal is to connect the left edge to the right edge (horizontal).
      // std::cout << "Adding cell " << i << " to horz " << std::endl
      //           << "  was: " << this->horz << std::endl;
      this->horz.set(i);
      // std::cout << "  now: " << this->horz << std::endl;
      break;
    case '.':
    case '-':
    case '\xb7':
      // These characters represent empty cells (U+00B7 is MIDDLE DOT).
      break;
    default:
      panic("Board(state): state string may only contain "
        "whitespace, 'X', 'O', '.', '-', and '·' (middle-dot) characters");
    }
  }
  this->play = x > o ? 'O' : 'X';
}


template<bitpos N>
std::string Board<N>::stateString() const {
  std::ostringstream stream;
  stream << play << ':';
  for (int i=0; i < count; ++i) {
    stream << (vert.test(i) ? 'X' : horz.test(i) ? 'O' : '-');
  }
  return stream.str();
}

template<bitpos N>
std::string Board<N>::reverseStateString() const {
  std::ostringstream stream;
  stream << play << ':';
  for (int i=count-1; i >= 0; --i) {
    stream << (vert.test(i) ? 'X' : horz.test(i) ? 'O' : '-');
  }
  return stream.str();
}

// displayChar decides what character should be printed to represent the state
// of the indicated cell. `show` is a map from rune to bitset, and each
// rune/bitset pairing indicates that the cells in the bitset should be
// rendered using the corresponding rune. If more than one bitset contains the
// target cell, one is chosen in an implementation-dependent way. If none of
// the bitsets includes the cell, returns '-'.
template<bitpos N>
static char displayChar(int cell, const PrintMap<N>& show) {
  for (auto kv : show) {
    if (kv.second.test(cell)) {
      return kv.first;
    }
  }
  return '-';
}


template<bitpos N>
std::ostream& Board<N>::MappedRender(std::ostream& w, std::string indent, const PrintMap<N> show) {

  // Print header row: a b c ...
  w << indent << " ";
  for (int i=0; i < N; i++) {
    w << ' ' << (char)('a' + i);
  }
  w  << std::endl;

  // Print board lines, prefixing each with its row number.
  int cellIndex = 0;
  for (int row=0; row < N; row++) {
    if (row != 0 && row+1 != 10) {
      indent += " ";
    }
    w << indent << row+1 << ": ";
    for (int col=0; col < N; col++) {
      char ch = displayChar<N>(cellIndex, show);
      cellIndex++;
      if (col != 0) {
        w << ' ';
      }
      w << ch;
    }
    w << std::endl;
  }
  return w;
}
