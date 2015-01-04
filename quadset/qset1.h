#ifndef __QSET1_H__
#define __QSET1_H__
#include <stdint.h>
//#include <initializer_list>
#include "quadset.h"
#include "bitops.h"

// This is qset<BITS,1>, a bitset template class specialized for holding one
// quadword's worth of bits -- i.e., between 1 and 64 bits.

template<bitpos BITS>
class qset<BITS,1> {
public:
  uint64_t b0;

  static_assert (1 <= BITS && BITS <= 64,
      "qset<BITS,1>: BITS must be in 1..63");

  typedef qset<BITS,1> qSet;

  static inline constexpr qSet from_ullong(uint64_t val) {
    return qSet{val};
  }

  static inline constexpr qSet make() {
    return qSet{0};
  }

  static inline constexpr qSet make(std::initializer_list<bitpos> list) {
    qSet result;
    result.reset();
    for (int i : list) {
      result.set(i);
    }
    return result;
  }

  static inline constexpr qSet range(bitpos m, bitpos n) {
    qSet result = {0};
    result.set(m, n);
    return result;
  }

  static inline constexpr qSet universe() {
    return qSet{bit_mask(0, BITS-1)};
  }

  // Clear any set bits in the unused part of the storage.  This method exists
  // for use as needed after fast_lsh(), fast_lsh_assign(), and/or fast_not().
  inline constexpr qSet clean() {
    return qSet{ b0 & bit_mask(0, BITS-1) };
  }

  inline constexpr bitpos size() { return BITS; }

  // constexpr qset<BITS,1>() : b0(0) { }
  // constexpr qset<BITS,1>(uint64_t val) : b0(val) { }
  // constexpr qset<BITS,1>(qset<BITS,1> const &other) : b0(other.b0) { }

  // Return true iff the set is nonempty.
  inline constexpr bool any() const {
    return b0 != 0;
  }

  // Return true iff the set is empty.
  inline constexpr bool none() const {
    return !any();
  }

  // Return true iff all the bits in the set are 1.
  inline constexpr bool all() const {
    return this->operator==(universe());
  }

  // Return the minimum element of the set.
  inline constexpr bitpos min() const {
    if (b0 != 0) {
      return lowestBitPosition(b0);
    } else {
      panic("qset1.min(): cannot find any element in the empty set");
      return 64;
    }
  }

  // Return the maximum element of the set.
  inline constexpr bitpos max() const {
    if (b0 != 0) {
      return highestBitPosition(b0);
    } else {
      panic("qset1.max(): cannot find any element in the empty set");
      return -1;
    }
  }

  // Returns the number of elements in the set.
  // Said another way, returns the number of bits that have the value 1.
  inline constexpr bitpos count() const {
    return countBits(b0);
  }

  inline constexpr bool operator==(qSet other) const {
    return b0 == other.b0;
  }

  inline constexpr bool operator!=(qSet other) const {
    return !this->operator==(other);
  }

  // Returns whether the bit at position pos is set (i.e., whether it is 1).
  inline constexpr bool test(bitpos pos) const {
    switch (pos >> 6) {
      case  0: 
        return (b0 & (1ULL << pos)) != 0;
      default:
        return false;
      //default: throw std::out_of_range("qset.set()");
    }
  }

  // Returns the quadword representation of the set.
  constexpr uint64_t to_ullong() const { return b0; }

  inline constexpr qSet operator<<(bitpos n) const {
    switch (n >> 6) {
      case  0: return qSet{(b0 << n) & bit_mask(0, BITS-1)};
      default: return qSet{0};
      //default: throw std::out_of_range("qset.set()");
    }
  }

  inline constexpr qSet fast_lsh(bitpos n) const {
    switch (n >> 6) {
      case  0: return qSet{(b0 << n)};
      default: return qSet{0};
    }
  }

  inline constexpr qSet operator>>(bitpos n) const {
    switch (n >> 6) {
      case  0: return qSet{b0 >> n};
      default: return qSet{0};
      //default: throw std::out_of_range("qset.set()");
    }
  }

  inline constexpr qSet operator& (qSet other) const {
    return qSet{b0 & other.b0};
  }

  inline constexpr qSet operator| (qSet other) const {
    return qSet{b0 | other.b0};
  }

  inline constexpr qSet operator^ (qSet other) const {
    return qSet{b0 ^ other.b0};
  }

  // Returns the set difference: {x | x in this and x not in other}.
  inline constexpr qSet operator- (qSet other) const {
    return qSet{b0 & ~other.b0};
  }

  inline constexpr qSet operator~ () const {
    return *this ^ universe();
  }

  inline constexpr qSet fast_not () const {
    return qSet{ ~b0 };
  }

  // MUST NOT IMPLEMENT operator=, or qset won't be POD.
  // qSet& operator= (qSet other) {
  //   b0 = other.b0;
  //   return *this;
  // }

  inline constexpr qSet& operator<<= (bitpos n) {
    switch (n >> 6) {
      case  0: *this = qSet{(b0 << n) & bit_mask(0, BITS-1)}; break;
      default: *this = qSet{0}; break;
      //default: throw std::out_of_range("qset.set()");
    }
    return *this;
  }

  inline constexpr qSet& fast_lsh_assign (bitpos n) {
    switch (n >> 6) {
      case  0: *this = qSet{(b0 << n)}; break;
      default: *this = qSet{0}; break;
      //default: throw std::out_of_range("qset.set()");
    }
    return *this;
  }

  inline constexpr qSet& operator>>= (bitpos n) {
    switch (n >> 6) {
      case  0: *this = qSet{b0 >> n}; break;
      default: *this = qSet{0}; break;
      //default: throw std::out_of_range("qset.set()");
    }
    return *this;
  }

  inline constexpr qSet& operator&= (qSet other) {
    b0 &= other.b0;
    return *this;
  }

  inline constexpr qSet& operator|= (qSet other) {
    b0 |= other.b0;
    return *this;
  }

  inline constexpr qSet& operator^= (qSet other) {
    b0 ^= other.b0;
    return *this;
  }

  // Removes from this set any elements in other.
  inline constexpr qSet& operator-= (qSet other) {
    b0 &= ~other.b0;
    return *this;
  }

  // Set all the bits.
  inline constexpr qSet& set() {
    *this = universe();
    return *this;
  }

  // Set the indicated bit.
  inline constexpr qSet& set(bitpos pos) {
    b0 |= 1ULL << pos;
    return *this;
  }

  // Set the range of bits [m..n-1].
  inline constexpr qSet& set(bitpos m, bitpos n) {
    if (m == n) {
      return *this;
    }
    if (m < 0 || BITS <= m || n < m || BITS < n) {
      throw std::out_of_range("qset.set(m,n)");
    }
    b0 |= bitRange(m, n-1);
    return *this;
  }

  // Set bits a, b, c, ....
  inline constexpr qSet& set(std::initializer_list<bitpos> list) {
    for (bitpos i : list) {
      this->set(i);
    }
    return *this;
  }

  // Clear all the bits.
  inline constexpr qSet& reset() {
    b0 = 0;
    return *this;
  }

  // Clear the indicated bit.
  inline constexpr qSet& reset(bitpos pos) {
    b0 &= ~(1ULL << pos);
    return *this;
  }

  // Clear the range of bits [m..n-1].
  inline constexpr qSet& reset(bitpos m, bitpos n) {
    if (m == n) {
      return *this;
    }
    if (m < 0 || BITS <= m || n < m || BITS < n) {
      throw std::out_of_range("qset.reset(m,n)");
    }
    b0 &= ~bitRange(m, n-1);
    return *this;
  }

  // Clear bits a, b, c, ....
  inline constexpr qSet& reset(std::initializer_list<bitpos> list) {
    for (bitpos i : list) {
      this->reset(i);
    }
    return *this;
  }

  // Toggle all the bits.
  inline constexpr qSet& flip() {
    b0 = ~b0;
    return *this;
  }

  // Toggle the indicated bit.
  inline constexpr qSet& flip(bitpos pos) {
    b0 ^= 1ULL << pos;
    return *this;
  }

  // Toggle the range of bits [m..n-1].
  inline constexpr qSet& flip(bitpos m, bitpos n) {
    if (m == n) {
      return *this;
    }
    if (m < 0 || BITS <= m || n < m || BITS < n) {
      throw std::out_of_range("qset.flip(m,n)");
    }
    b0 ^= bitRange(m, n-1);
    return *this;
  }

  // Toggle bits a, b, c, ....
  inline constexpr qSet& flip(std::initializer_list<bitpos> list) {
    for (bitpos i : list) {
      this->flip(i);
    }
    return *this;
  }

  // Print a representation of the set; for example: {1, 3..5, 8, 10, 12}
  std::ostream& print(std::ostream &out) {
    return out << *this;
  }

  // Return a string representation of the set; for example: {1, 3..5, 8, 12}
  std::string to_string() const {
    return ::to_string(*this);
  }
  
};

static_assert (std::is_pod< qset<64,1> >::value, "qset<BITS,1> must be POD");

#endif /* __QSET1_H__ */
