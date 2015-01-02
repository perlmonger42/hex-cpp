#ifndef __QUADSET_H__
#define __QUADSET_H__ 1
#include <iostream>
#include <sstream>

// The Quadset Type
// ================
// The quadset type represents a set of small integers, similar to std::bitset.
// However, instead of being infinitely variable in size, quadsets are
// restricted to at most 256 bits.
//
// A quadset inherits from one of four template specializations, having
// capacities of 1, 2, 3, or 4 quadwords (i.e., 64, 128, 192, or 256 bits).
//
// All methods (except for assignment operators) pass quadsets by value, not by
// reference. If your set is too big to be passed by value, consider using
// std::bitset instead.


// Fast and Slow Operators
// =======================
// Although the C standard does not guarantee it, the typical C programmer
// expects that ｢(n << 1) >> 1｣ will clear the high order bit of n, if n 
// has unsigned integral type. This is a natural result of the fact that most
// hardware architectures have shift instructions that work that way.
//
// However, quadset<N>, for N not a multiple of 64, wouldn't behave that way
// without extra care. The problem is that a left shift isn't necessarily
// shifting elements off the end of a register; rather, it's shifting into
// the unused upper bits of an underlying uint64_t value. Normally that
// wouldn't be problem, since those bits are generally ignored. But certain
// operations, if implemented naively, could be compromised by those bits. For
// example, a subsequent right shift could bring those bits back into the used
// region of the uint64_t. As an another example, if operator== were
// implemented by a simple comparision of the underlying uint64_t values, it
// might erroneously indicate that two sets were different because their unused
// bits differ.
//
// This implementation takes the approach of keeping the unused bits clear,
// but provides alternate operations that give the user flexibility to manage
// those pesky bits at the time of their choosing.
//
// Thus, the standard operators operator<<(n), operator<<=(n), and operator~()
// create answers whose unused bits are guaranteed to be zero. Consequently,
// operator==(), operator<<(), and operator<<=() assume that condition.
//
// In contrast, the "fast" operators fast_lsh(n), fast_lsh_assign(n), and
// fast_not() may leave extraneous set bits in the unused higher order bits of
// a quadset value, if its BITS is not a multiple of 64.  The clean() operator
// exists to clear those extra bits after a sequence of operations that
// may leave garbage bits in the unused area.


// The Bitpos Type
// ===============
// The bitpos type represents an element of a quadset, and is putatively a
// value between 0 and 255, inclusive.  Since the elements of quadsets are
// constrained to this range, uint8_t would be large enough to store any
// element. However, -1 and 256 are useful as off-the-end inputs, even though
// they can't be stored in a quadset.  So instead, we use the next-larger
// signed type instead.
typedef int_fast16_t bitpos;


// bit_mask(m,n) returns a uint64 value with nonzero bits from m to n,
// inclusive.  Note that is is a different convention from most other
// interfaces dealing with ranges, because it INCLUDES m.
// Requires 0 ≤ m ≤ n < 64.
inline constexpr uint64_t bit_mask(int m, int n) {
  //static_assert(0 <= m, "bit_mask(m,n) requires 0 <= m");
  //static_assert(m <= n, "bit_mask(m,n) requires m <= n");
  //static_assert(n < 64, "bit_mask(m,n) requires n < 64");
  return (~0ULL >> (63+m-n)) << m;
}
    

// The inheritance hierarchy for quadset<BITS> is as follows (arrows point
// from subclass to superclass):
//
//   qset<BITS,1>   <--+        
//                     |      
//   qset<BITS,2>   <--+      
//                     |      
//   qset<BITS,3>   <--+----  quadset<BITS>
//                     |      
//   qset<BITS,4>   <--+      
//
// 
// qset<BITS,1>, qset<BITS,2>, qset<BITS,3>, and qset<BITS,4> are the four
// template specializations for handling capacities of 1, 2, 3, and 4 quads.


// Forward declaration of qset<BITS,QUADWORDS>.
template<bitpos BITS, bitpos QUADWORDS = (BITS+63) / 64>
class qset;

// Forward declaration of the ostream insertion function.
template<bitpos BITS>
std::ostream& operator<<(std::ostream &out, qset<BITS> set);

// Forward declaration of the to_string() function.
template<bitpos BITS>
std::string to_string(qset<BITS> set);


#include "../quadset/qset1.h"
#include "../quadset/qset2.h"
#include "../quadset/qset3.h"


// quadset<BITS> is a subclass of the right-sized specialization.
// This is almost all it has to be, since all the real work is done in
// qset<BITS,QUADWORDS>.
// Unfortunately, it's not that simple, because the result type of all the
// operators would be wrong (for example, quadset<70>::operator| would return a
// value of type qset<70,2> instead of quadset<70>). In consequence,
// quadset<BITS> has to provide a bunch of wrappers that adjust the type.
template<bitpos BITS>
class quadset : public qset<BITS> {
public:

  typedef quadset<BITS> quadSet;
  typedef qset<BITS>    qSet;

  constexpr quadset() { qSet::reset(); }
  constexpr quadset(uint64_t val) : qSet{val} { }
  constexpr quadset(qSet val) : qSet(val) { }

  static constexpr quadSet from_ullong(uint64_t val) {
    return quadSet{ qSet::from_ullong(val) };
  }

  static constexpr quadSet make() {
    return quadSet{ qSet::make() };
  }

  template<typename... Args>
  static constexpr quadSet make(bitpos p, Args... args) {
    return quadSet{ qSet::make(p, args...) };
  }

  static quadSet make(std::initializer_list<bitpos> list) {
    return quadSet{ qSet::make(list) };
  }

  static constexpr quadSet range(bitpos m, bitpos n) {
    return quadSet{ qSet::range(m, n) };
  }

  static inline constexpr quadSet repeat_block(
    quadSet block,
    bitpos  blockSize,
    bitpos  count,
    quadSet accumulator = quadSet{ 0 }
  ) {
    return count == 0 ? accumulator :
      repeat_block(block, blockSize, count-1, (accumulator<<blockSize) | block);
  }

  static constexpr quadSet universe() {
    return quadSet{ qSet::universe() };
  }

  inline constexpr quadSet clean() {
    return quadSet{ qSet::clean() };
  }

  constexpr uint64_t to_ullong() const {
    return this->qSet::to_ullong();
  }

  constexpr quadSet operator<<(bitpos n) const {
    return quadSet{ this->qSet::operator<<(n) };
  }

  constexpr quadSet fast_lsh(bitpos n) const {
    return quadSet{ this->qSet::fast_lsh(n) };
  }

  constexpr quadSet operator>>(bitpos n) const {
    return quadSet{ this->qSet::operator>>(n) };
  }

  constexpr quadSet operator& (quadSet other) const {
    return quadSet{ this->qSet::operator& (other) };
  }

  constexpr quadSet operator| (quadSet other) const {
    return quadSet{ this->qSet::operator| (other) };
  }

  constexpr quadSet operator^ (quadSet other) const {
    return quadSet{ this->qSet::operator^ (other) };
  }

  constexpr quadSet operator- (quadSet other) const {
    return quadSet{ this->qSet::operator- (other) };
  }

  constexpr quadSet operator~ () const {
    return quadSet{ this->qSet::operator~ () };
  }

  constexpr quadSet fast_not () const {
    return quadSet{ this->qSet::fast_not () };
  }

  constexpr quadSet& operator<<= (bitpos n) {
    this->qSet::operator<<=(n);
    return *this;
  }

  constexpr quadSet& fast_lsh_assign (bitpos n) {
    this->qSet::fast_lsh_assign(n);
    return *this;
  }

  constexpr quadSet& operator>>= (bitpos n) {
    this->qSet::operator>>=(n);
    return *this;
  }

  constexpr quadSet& operator&= (quadSet other) {
    this->qSet::operator&= (other);
    return *this;
  }

  constexpr quadSet& operator|= (quadSet other) {
    this->qSet::operator|= (other);
    return *this;
  }

  constexpr quadSet& operator^= (quadSet other) {
    this->qSet::operator^= (other);
    return *this;
  }

  constexpr quadSet& operator-= (quadSet other) {
    this->qSet::operator-= (other);
    return *this;
  }

  constexpr quadSet& set() {
    this->qSet::set();
    return *this;
  }

  constexpr quadSet& set(bitpos pos) {
    this->qSet::set(pos);
    return *this;
  }

  constexpr quadSet& set(bitpos m, bitpos n) {
    this->qSet::set(m, n);
    return *this;
  }

  constexpr quadSet& set(std::initializer_list<bitpos> list) {
    this->qSet::set(list);
    return *this;
  }

  constexpr quadSet& reset() {
    this->qSet::reset();
    return *this;
  }

  constexpr quadSet& reset(bitpos pos) {
    this->qSet::reset(pos);
    return *this;
  }

  constexpr quadSet& reset(bitpos m, bitpos n) {
    this->qSet::reset(m, n);
    return *this;
  }

  constexpr quadSet& reset(std::initializer_list<bitpos> list) {
    this->qSet::reset(list);
    return *this;
  }

  constexpr quadSet& flip() {
    this->qSet::flip();
    return *this;
  }

  constexpr quadSet& flip(bitpos pos) {
    this->qSet::flip(pos);
    return *this;
  }

  constexpr quadSet& flip(bitpos m, bitpos n) {
    this->qSet::flip(m, n);
    return *this;
  }

  constexpr quadSet& flip(std::initializer_list<bitpos> list) {
    this->qSet::flip(list);
    return *this;
  }
};


template<bitpos BITS>
std::ostream& operator<<(std::ostream &out, qset<BITS> set) {
  out << '{';
  bitpos rangeFirst, rangeFinal;
  std::string comma = "";
  rangeFirst = rangeFinal = BITS + 1; // deliberately out of range
  for (int i=0; i < BITS+1; ++i) {
    if (i < BITS && set.test(i)) {
      if (i == rangeFinal + 1) {
        rangeFinal = i;
      } else {
        rangeFirst = rangeFinal = i;
      }
    } else if (rangeFinal < i) {
      // Emit rangeFirst..rangeFinal,
      // but simplify "x..x" to "x", and
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
      rangeFirst = rangeFinal = BITS + 1;
    }
  }
  out << '}';
  return out;
}

template<bitpos BITS>
std::string to_string(qset<BITS> set) {
  std::ostringstream stream;
  set.print(stream);
  return stream.str();
}

#endif /* __QUADSET_H__ */
