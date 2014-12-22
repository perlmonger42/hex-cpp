#include <bitset>
#include <iostream>
#include <stdint.h>
#include <initializer_list>
#include "../quadset/qset1.h"
#include "../quadset/qset2.h"
#include "../quadset/qset3.h"

template<bitpos S, bitpos B = S*S, bitpos Q = (B+63) / 64>
class shared_bits {
public:

  //static constexpr bitpos MAXBITS = Q; // maximum bits this template can handle
  //static constexpr bitpos BITS = B; // number of bits in bitset
  
  static inline constexpr qset<B> repeat_block(
    qset<B> block,
    bitpos blockSize,
    bitpos count,
    qset<B> accumulator = qset<B>{ 0 }
  ) {
    return count == 0 ? accumulator :
      repeat_block(block, blockSize, count-1, (accumulator<<blockSize) | block);
  }

  static inline constexpr qset<B> universe() {
    return repeat_block(qset<B>{ uint64_t(-1LL) >> (64-S)}, S, S);
  }

  static inline constexpr qset<B> top() {
    return qset<B>{ uint64_t(-1LL) >> (64-S) };
  }

  static inline constexpr qset<B> bottom() {
    return top() << (B - S);
  }

  static inline constexpr qset<B> left() {
    return repeat_block(qset<B>{1}, S, S);
  }

  static inline constexpr qset<B> right() {
    return repeat_block(qset<B>{1<<(S-1)}, S, S);
  }

  static inline constexpr qset<B> neighbors(qset<B> a) {
    constexpr auto l = ~left();
    constexpr auto r = ~right();
    return           ( ((a >> (S + 0)))
                     | ((a >> (S - 1)) & l)
                     | ((a >> (0 + 1)) & r)
                     | ((a << (0 + 1)) & l)
                     | ((a << (S - 1)) & r)
                     | ((a << (S + 0)))
                     );
  }

  static std::ostream& emit(std::ostream &out, qset<B> set) {
    out << '{';
    const int LIMIT = B*B;
    int rangeFirst, rangeFinal;
    std::string comma = "";
    rangeFirst = rangeFinal = LIMIT + 1; // deliberately out of range
    for (int i=0; i < LIMIT+1; ++i) {
      if (i < LIMIT && set.test(i)) {
        if (i == rangeFinal + 1) {
          rangeFinal = i;
        } else {
          rangeFirst = rangeFinal = i;
        }
      } else if (rangeFinal < i) {
        // Emit rangeFirst..rangeFinal, but simplify "x..x" to "x", and
        // simplify "x..x+1" to "x, x+1".
        out << comma;
        comma = ", ";
        if (rangeFinal == rangeFirst) {
          out << rangeFirst;
        } else if (rangeFinal == rangeFirst+1) {
          out << rangeFirst << comma << rangeFinal;
        } else {
          out << rangeFirst << ".." << rangeFinal;
        }
        rangeFirst = rangeFinal = LIMIT + 1;
      }
    }
    out << '}';
    return out;
  }
};

// template<bitpos Q, bitpos B>
// const bitpos shared_bits<B,Q>::BITS; // number of bits in bitset

template<bitpos S, bitpos B = S*S, bitpos Q = (B+63) / 64>
class CellSet : public qset<S*S> {
public:
  typedef qset<B> bitset;
  //static constexpr bitpos size = S; // Edge length. A board is size×size cells.
  //static constexpr bitpos max = S*S-1; // Cell numbers run from 0 to max.
  //static constexpr bitpos count = S*S; // The number of cells on an N×N board.


  constexpr CellSet() = default;
  constexpr CellSet(uint64_t val) : bitset{val} { }
  constexpr CellSet(bitset val) : bitset(val) { }
  // constexpr CellSet() : bitset() { }
  // constexpr CellSet(bitset const &other) : bitset(other) { }
  // constexpr CellSet(CellSet const &other) : bitset(other) { }
  // constexpr CellSet(unsigned long long val) : bitset{val} { }

  static CellSet<S> elements(std::initializer_list<int> list) {
    CellSet<S> result;
    result.clear();
    for (int i : list) {
      result.set(i);
    }
    return result;
  }
  
  inline constexpr uint64_t to_ullong() const {
    return this->bitset::to_ullong();
  }

  inline constexpr CellSet<S> operator<<(bitpos n) const {
    return CellSet<S>{ this->bitset::operator<<(n) };
  }

  inline constexpr CellSet<S> operator>>(bitpos n) const {
    return CellSet<S>{ this->bitset::operator>>(n) };
  }

  inline constexpr CellSet<S> operator& (CellSet<S> other) const {
    return CellSet<S>{ this->bitset::operator&(other) };
  }

  inline constexpr CellSet<S> operator| (CellSet<S> other) const {
    return CellSet<S>{ this->bitset::operator|(other) };
  }

  inline constexpr CellSet<S> operator~ () const {
    return CellSet<S>{ this->bitset::operator~() };
  }

  inline constexpr CellSet<S> operator- (CellSet<S> other) const {
    return CellSet<S>{ this->bitset::operator-(other) };
  }

  inline CellSet<S>& operator|= (CellSet<S> other) {
    this->bitset::operator|=(other);
    return *this;
  }

  inline CellSet<S>& operator&= (CellSet<S> other) {
    this->bitset::operator&=(other);
    return *this;
  }

  inline CellSet<S>& operator-= (CellSet<S> other) {
    this->bitset::operator-=(other);
    return *this;
  }

  inline CellSet<S>& set() {
    this->bitset::set();
    return *this;
  }

  inline CellSet<S>& set(bitpos n) {
    this->bitset::set(n);
    return *this;
  }

  inline CellSet<S>& clear() {
    this->bitset::reset();
    return *this;
  }

  inline CellSet<S>& clear(bitpos n) {
    this->bitset::reset(n);
    return *this;
  }

  inline CellSet<S>& reset() {
    this->bitset::reset();
    return *this;
  }

  inline CellSet<S>& reset(bitpos n) {
    this->bitset::reset(n);
    return *this;
  }

  inline CellSet<S>& flip() {
    this->bitset::flip();
    return *this;
  }

  inline CellSet<S>& flip(bitpos n) {
    this->bitset::flip(n);
    return *this;
  }

  static inline constexpr CellSet<S> universe() {
    return CellSet<S>{ shared_bits<S>::universe() };
  }

  static inline constexpr CellSet<S> top() {
    return CellSet<S>{ shared_bits<S>::top() };
  }

  static inline constexpr CellSet<S> bottom() {
    return CellSet<S>{ shared_bits<S>::bottom() };
  }

  static inline constexpr CellSet<S> left() {
    return CellSet<S>{ shared_bits<S>::left() };
  }

  static inline constexpr CellSet<S> right() {
    return CellSet<S>{ shared_bits<S>::right() };
  }

  inline constexpr CellSet<S> neighbors() const {
    return CellSet<S>{ shared_bits<S>::neighbors(*this) };
  }

  std::ostream& print(std::ostream &out) {
    return bitset::print(out);
  }
  std::ostream& emit(std::ostream &out) {
    return bitset::print(out);
  }
};

static_assert (std::is_pod< CellSet<8> >::value, "CellSet must be POD");


template<bitpos S>
std::ostream& operator<< (std::ostream &out, CellSet<S> set) {
  return set.emit(out);
}

