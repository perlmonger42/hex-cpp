// "board.h" should be inluded before this file is included.


// winner() returns 'X', 'O', or '?', depending on whether the game has been
// won or not, and by whom (vertical == 'X', horizontal == 'O').
// If testFor is 'X', only a vertical win is tested, and if it is not found,
// winner() will return '?' even if there is a win for horizontal.
// If testFor is 'O', only a horizontal win is tested, and if it is not found,
// winner() will return '?' even if there is a win for vertical.
// If testFor is any other char, a win will be sought for either player, and
// the return value will reflect any win found.
template<bitpos N>
char Board<N>::winner(char testFor) const {
  if (testFor != 'O' && verticalHasWon()) {
    return 'X';
  }
  if (testFor != 'X' && horizontalHasWon()) {
    return 'O';
  }
  return '?';
}

template<>
bool Board<1>::verticalHasWon() const {
  return vert.test(0);
}

template<>
bool Board<1>::horizontalHasWon() const {
  return horz.test(0);
}

template<bitpos N>
bool Board<N>::verticalHasWon() const {
  //std::cout << "Is vertical connected?" << std::endl << *this;
  return isConnected(topRow(), verticalCells(), bottomRow());
}

template<bitpos N>
bool Board<N>::horizontalHasWon() const {
  //std::cout << "Is horizontal connected?" << std::endl << *this;
  return isConnected(leftColumn(), horizontalCells(), rightColumn());
}

template<bitpos N>
bool Board<N>::isConnected(cellset<N> start, cellset<N> owned, cellset<N> stop){
  typedef cellset<N> Set;
  constexpr Set l = ~Board<N>::leftColumn();
  constexpr Set r = ~Board<N>::rightColumn();

  Set startGroup = owned & start;
  Set stopGroup = owned & stop;

  // First, a quick check whether any start and stop cells are owned.
  if (startGroup.none() || stopGroup.none()) {
    return false;
  }

  // Start with the owned cells in start and stop. Add into each set any owned
  // neighbors of that set. Continue until the sets overlap, or until neither
  // set changes.
  for (;;) {
    Set startNeighbors = startGroup.fast_neighbors();
    Set stopNeighbors = stopGroup.fast_neighbors();

    Set newStartGroup = (startNeighbors & owned) | startGroup;
    Set newStopGroup = (stopNeighbors & owned) | stopGroup;

    if (newStartGroup == startGroup && newStopGroup == stopGroup) {
      // No progress was made.
      return false;
    }

    // If flood from start overlaps flood from stop, there is a connection.
    if ((newStartGroup & newStopGroup).any()) {
      return true;
    }

    startGroup = newStartGroup;
    stopGroup = newStopGroup;
  }
}

// virtualWinner() returns 'X', 'O', or '?', depending on whether the game has
// been won or not, and by whom (vertical == 'X', horizontal == 'O').
// If testFor is 'X', only a vertical win is tested, and if it is not found,
// winner() will return '?' even if there is a win for horizontal.
// If testFor is 'O', only a horizontal win is tested, and if it is not found,
// winner() will return '?' even if there is a win for vertical.
// If testFor is any other char, a win will be sought for either player, and
// the return value will reflect any win found.
template<bitpos N>
char Board<N>::virtualWinner(char testFor) const {
  if (testFor != 'O' && verticalHasVirtualConnection()) {
    return 'X';
  }
  if (testFor != 'X' && horizontalHasVirtualConnection()) {
    return 'O';
  }
  return '?';
}

template<>
bool Board<1>::verticalHasVirtualConnection() const {
  return vert.test(0);
}

template<>
bool Board<1>::horizontalHasVirtualConnection() const {
  return horz.test(0);
}

template<bitpos N>
bool Board<N>::verticalHasVirtualConnection() const {
  //std::cout << "Is vertical virtually connected?" << std::endl << *this;
  return isVirtuallyConnected(
      topRow(), verticalCells(), bottomRow(), emptyCells());
}

template<bitpos N>
bool Board<N>::horizontalHasVirtualConnection() const {
  //std::cout << "Is horizontal virtually connected?" << std::endl << *this;
  return isVirtuallyConnected(
      leftColumn(), horizontalCells(), rightColumn(), emptyCells());
}

template<bitpos N>
bool Board<N>::isVirtuallyConnected(
    cellset<N> start,
    cellset<N> owned,
    cellset<N> stop,
    cellset<N> vacant
) {
  typedef cellset<N> Set;
  constexpr Set l = ~Board<N>::leftColumn();
  constexpr Set r = ~Board<N>::rightColumn();

  Set startGroup = owned & start;
  Set stopGroup = owned & stop;

  // First, a quick check whether any start and stop cells are owned.
  if (startGroup.none() || stopGroup.none()) {
    return false;
  }

  // Start with the owned cells in start and stop. Add into each set any owned
  // neighbors of that set. Continue until the sets overlap, or until neither
  // set changes.
  for (;;) {
    Set startNeighbors = startGroup.fast_neighbors();
    Set newStartGroup = (startNeighbors & owned) | startGroup;

    Set stopNeighbors = stopGroup.fast_neighbors();
    Set newStopGroup = (stopNeighbors & owned) | stopGroup;

    if (newStartGroup == startGroup && newStopGroup == stopGroup) {
      // No progress was made.
      auto commonEmptyNeighbors = startNeighbors & stopNeighbors & vacant;
      return commonEmptyNeighbors.any() && commonEmptyNeighbors.count() > 1;
    }

    // If flood from start overlaps flood from stop, there is a connection.
    if ((newStartGroup & newStopGroup).any()) {
      return true;
    }

    startGroup = newStartGroup;
    stopGroup = newStopGroup;
  }
}
