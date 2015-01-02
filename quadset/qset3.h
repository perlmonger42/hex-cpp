#ifndef __QSET3_H__
#define __QSET3_H__
#include <stdint.h>
#include <stdexcept>
#include "quadset.h"
#include "bitops.h"

// This is qset<BITS,3>, a bitset template class specialized for holding three
// quadwords' worth of bits -- i.e., between 129 and 192 bits.

template<bitpos BITS>
class qset<BITS,3> {
public:
  uint64_t b0;
  uint64_t b1;
  uint64_t b2;

  static_assert (129 <= BITS && BITS <= 192,
      "qset<BITS,3>: BITS must be in 129..192");

  typedef qset<BITS,3> qSet;

  static inline constexpr qSet from_ullong(uint64_t val) {
    return qSet{val};
  }

  static constexpr qSet make() {
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

  //static constexpr qSet make(std::initializer_list<bitpos>) {
  //  qSet result;
  //  result.clear();
  //  for (int i : list) {
  //    result.set(i);
  //  }
  //  return result;
  //}

  static inline constexpr qSet universe() {
    return qSet{uint64_t(~0ULL), uint64_t(~0ULL), bit_mask(0, BITS-129)};
  }

  // Clear any set bits in the unused part of the storage.  This method exists
  // for use as needed after fast_lsh(), fast_lsh_assign(), and/or fast_not().
  inline constexpr qSet clean() {
    return qSet{ b0, b1, b2 & bit_mask(0, BITS-129) };
  }

  inline constexpr bitpos size() { return BITS; }
  
  // constexpr qSet() : b0(0) { }
  // constexpr qSet(uint64_t val) : b0(val) { }
  // constexpr qSet(qSet const &other) : b0(other.b0) { }

  // Return true iff the set is nonempty.
  inline constexpr bool any() const {
    return b0 != 0 || b1 != 0 || b2 != 0;
  }

  // Return true iff the set is empty.
  inline constexpr bool none() const {
    return !any();
  }

  // Return true iff all the bits in the set are 1.
  inline constexpr bool all() const {
    return this->operator==(universe());
  }

  // Return the number of elements in the set.
  // Said another way, returns the number of bits that have the value 1.
  inline constexpr bitpos count() const {
    return countBits(b0) + countBits(b1) + countBits(b2);
  }

  inline constexpr bool operator==(qSet other) const {
    return b0 == other.b0 && b1 == other.b1 && b2 == other.b2;
  }

  inline constexpr bool operator!=(qSet other) const {
    return !this->operator==(other);
  }

  // Returns whether the bit at position pos is set (i.e., whether it is 1).
  inline constexpr bool test(bitpos pos) const {
    switch (pos >> 6) {
      case  0: return (b0 & (1ULL << pos)) != 0;
      case  1: return (b1 & (1ULL << (pos & 0x3F))) != 0;
      case  2: return (b2 & (1ULL << (pos & 0x3F))) != 0;
      default: return false;
      //default: throw std::out_of_range("qset.set()");
    }
  }

  // Returns the quadword representation of the low-order 64 elements of the
  // set. Throws std::overflow_error if any high-order elements are present.
  inline constexpr uint64_t to_ullong() const {
    if (b1 != 0 || b2 != 0) {
      throw std::overflow_error("qset.to_ullong()");
    }
    return b0;
  }

  inline constexpr qSet operator<<(bitpos n) const {
    if (n < 0) {
      throw std::out_of_range("qset.operator<<(): negative shift");
    } else if (n == 0) {
      return *this;
    } else if (n < 64) {
      return qSet{b0 << n,
                  (b1 << n) | (b0 >> (64 - n)),
                  ((b2 << n) | (b1 >> (64 - n))) & bit_mask(0, BITS-129)};
    } else if (n == 64) {
      return qSet{0, b0, b1 & bit_mask(0, BITS-129)};
    } else if (n < 128) {
      return qSet{0,
                  b0 << (n - 64),
                  (b0 >> (128 - n) | b1 << (n - 64)) & bit_mask(0, BITS-129)};
    } else if (n == 128) {
      return qSet{0, 0, b0 & bit_mask(0, BITS-129)};
    } else if (n < 192) {
      return qSet{0, 0, (b0 << (n - 128)) & bit_mask(0, BITS-129)};
    } else {
      return qSet{0};
    }
  }

  inline constexpr qSet fast_lsh(bitpos n) const {
    if (n < 0) {
      throw std::out_of_range("qset.operator<<(): negative shift");
    } else if (n == 0) {
      return *this;
    } else if (n < 64) {
      return qSet{b0 << n,
                  (b1 << n) | (b0 >> (64 - n)),
                  ((b2 << n) | (b1 >> (64 - n)))};
    } else if (n == 64) {
      return qSet{0, b0, b1};
    } else if (n < 128) {
      return qSet{0,
                  b0 << (n - 64),
                  (b0 >> (128 - n) | b1 << (n - 64))};
    } else if (n == 128) {
      return qSet{0, 0, b0};
    } else if (n < 192) {
      return qSet{0, 0, (b0 << (n - 128))};
    } else {
      return qSet{0};
    }
  }

  inline constexpr qSet operator>>(bitpos n) const {
    if (n < 0) {
      throw std::out_of_range("qset.operator>>(): negative shift");
    } else if (n == 0) {
      return *this;
    } else if (n < 64) {
      return qSet{b0 >> n | b1 << (64 - n),
                  b1 >> n | b2 << (64 - n),
                  b2 >> n};
    } else if (n == 64) {
      return qSet{b1, b2, 0};
    } else if (n < 128) {
      return qSet{b2 << (128 - n) | b1 >> (n - 64), b2 >> (n - 64), 0};
    } else if (n == 128) {
      return qSet{b2, 0, 0};
    } else if (n < 192) {
      return qSet{b2 >> (n - 128), 0, 0};
    } else {
      return qSet{0};
    }
  }

  inline constexpr qSet operator& (qSet other) const {
    return qSet{b0 & other.b0, b1 & other.b1, b2 & other.b2};
  }

  inline constexpr qSet operator| (qSet other) const {
    return qSet{b0 | other.b0, b1 | other.b1, b2 | other.b2};
  }

  inline constexpr qSet operator^ (qSet other) const {
    return qSet{b0 ^ other.b0, b1 ^ other.b1, b2 ^ other.b2};
  }

  // Returns the set difference: {x | x in this and x not in other}.
  inline constexpr qSet operator- (qSet other) const {
    return qSet{b0 & ~other.b0, b1 & ~other.b1, b2 & ~other.b2};
  }

  inline constexpr qSet operator~ () const {
    return *this ^ universe();
  }

  inline constexpr qSet fast_not () const {
    return qSet{ ~b0, ~b1, ~b2 };
  }

  // MUST NOT IMPLEMENT operator=, or qset won't be POD.
  // qSet& operator= (qSet other) {
  //   b0 = other.b0;
  //   b1 = other.b1;
  //   return *this;
  // }

  inline qSet& operator<<=(bitpos n) {
    if (n < 0) {
      throw std::out_of_range("qset.operator<<=(): negative shift");
    } else if (n == 0) {
      ;
    } else if (n < 64) {
      b2 = (b2 << n | b1 >> (64 - n)) & bit_mask(0, BITS - 129);
      b1 = (b1 << n | b0 >> (64 - n));
      b0 = b0 << n;
    } else if (n == 64) {
      b2 = b1 & bit_mask(0, BITS - 129);
      b1 = b0;
      b0 = 0;
    } else if (n < 128) {
      b2 = (b0 >> (128 - n) | b1 << (n - 64)) & bit_mask(0, BITS - 129);
      b1 = (b0 << (n - 64));
      b0 = 0;
    } else if (n == 128) {
      b2 = b0 & bit_mask(0, BITS - 129);
      b1 = 0;
      b0 = 0;
    } else if (n < 192) {
      b2 = (b0 << (n - 128)) & bit_mask(0, BITS - 129);
      b1 = 0;
      b0 = 0;
    } else {
      b0 = b1 = b2 = 0;
    }
    return *this;
  }

  inline qSet& fast_lsh_assign(bitpos n) {
    if (n < 0) {
      throw std::out_of_range("qset.operator<<=(): negative shift");
    } else if (n == 0) {
      ;
    } else if (n < 64) {
      b2 = (b2 << n | b1 >> (64 - n));
      b1 = (b1 << n | b0 >> (64 - n));
      b0 = b0 << n;
    } else if (n == 64) {
      b2 = b1;
      b1 = b0;
      b0 = 0;
    } else if (n < 128) {
      b2 = (b0 >> (128 - n) | b1 << (n - 64));
      b1 = (b0 << (n - 64));
      b0 = 0;
    } else if (n == 128) {
      b2 = b0;
      b1 = 0;
      b0 = 0;
    } else if (n < 192) {
      b2 = (b0 << (n - 128));
      b1 = 0;
      b0 = 0;
    } else {
      b0 = b1 = b2 = 0;
    }
    return *this;
  }

  inline qSet& operator>>=(bitpos n) {
    if (n < 0) {
      throw std::out_of_range("qset.operator>>=(): negative shift");
    } else if (n == 0) {
      ;
    } else if (n < 64) {
      b0 = b0 >> n | b1 << (64 - n);
      b1 = b1 >> n | b2 << (64 - n);
      b2 = (b2 >> n) & bit_mask(0, BITS - 129);
    } else if (n == 64) {
      b0 = b1;
      b1 = b2;
      b2 = 0;
    } else if (n < 128) {
      b0 = b1 >> (n - 64) | b2 << (128 - n);
      b1 = b2 >> (n - 64);
      b2 = 0;
    } else if (n == 128) {
      b0 = b2;
      b1 = 0;
      b2 = 0;
    } else if (n < 192) {
      b0 = b2 >> (n - 128);
      b1 = 0;
      b2 = 0;
    } else {
      b0 = b1 = b2 = 0;
    }
    return *this;
  }

  inline qSet& operator&= (qSet other) {
    b0 &= other.b0;
    b1 &= other.b1;
    b2 &= other.b2;
    return *this;
  }

  inline qSet& operator|= (qSet other) {
    b0 |= other.b0;
    b1 |= other.b1;
    b2 |= other.b2;
    return *this;
  }

  inline qSet& operator^= (qSet other) {
    b0 ^= other.b0;
    b1 ^= other.b1;
    b2 ^= other.b2;
    return *this;
  }

  // Removes from this set any elements in other.
  inline qSet& operator-= (qSet other) {
    b0 &= ~other.b0;
    b1 &= ~other.b1;
    b2 &= ~other.b2;
    return *this;
  }

  // Set all bits.
  inline qSet& set() {
    *this = universe();
    return *this;
  }

  // Set the indicated bit.
  inline qSet& set(bitpos pos) {
    switch (pos >> 6) {
      case  0: b0 |= 1ULL << (pos -  0); break;
      case  1: b1 |= 1ULL << (pos & 0x3F); break;
      case  2: b2 |= 1ULL << (pos & 0x3F); break;
      default: throw std::out_of_range("qset.set(pos)");
    }
    return *this;
  }

  // Set the range of bits [m..n-1].
  inline qSet& set(bitpos m, bitpos n) {
    if (m == n) {
      return *this;
    }
    if (m < 0 || BITS <= m || n < m || BITS < n) {
      throw std::out_of_range("qset.set(m,n)");
    }

    n--;
    bitpos lo = m >> 6;
    bitpos hi = n >> 6;
    if (lo == hi) {
      // The requested range fits in a single quadword.
      switch (lo) {
        case  0: b0 |= bitRange(m, n); break;
        case  1: b1 |= bitRange(m & 0x3F, n & 0x3F); break;
        case  2: b2 |= bitRange(m & 0x3F, n & 0x3F); break;
      }
    } else if (hi == lo+1) {
      // The requested range spans a quadword boundary.
      switch (lo) {
        case 0: b0 |= bitRange(m, 63);
                b1 |= bitRange(0, n & 0x3F);
                break;
        case 1: b1 |= bitRange(m & 0x3F, 63);
                b2 |= bitRange(0, n & 0x3F);
                break;
      }
    } else {
      // The requested range spans two quadword boundaries.
      b0 |= bitRange(m, 63);
      b1 = ~0ULL;
      b2 |= bitRange(0, n & 0x3F);
    }
    return *this;
  }

  // Set bits a, b, c, ....
  inline qSet& set(std::initializer_list<bitpos> list) {
    for (bitpos i : list) {
      this->set(i);
    }
    return *this;
  }

  // Clear all the bits.
  inline qSet& reset() {
    b0 = 0;
    b1 = 0;
    b2 = 0;
    return *this;
  }

  // Clear the indicated bit.
  inline qSet& reset(bitpos pos) {
    switch (pos >> 6) {
      case  0: b0 &= ~(1ULL << (pos -  0)); break;
      case  1: b1 &= ~(1ULL << (pos & 0x3F)); break;
      case  2: b2 &= ~(1ULL << (pos & 0x3F)); break;
      default: throw std::out_of_range("qset.reset(pos)");
    }
    return *this;
  }

  // Clear the range of bits [m..n-1].
  inline qSet& reset(bitpos m, bitpos n) {
    if (m == n) {
      return *this;
    }
    if (m < 0 || BITS <= m || n < m || BITS < n) {
      throw std::out_of_range("qset.reset(m,n)");
    }

    n--;
    bitpos lo = m >> 6;
    bitpos hi = n >> 6;
    if (lo == hi) {
      // The requested range fits in a single quadword.
      switch (lo) {
        case  0: b0 &= ~bitRange(m, n); break;
        case  1: b1 &= ~bitRange(m & 0x3F, n & 0x3F); break;
        case  2: b2 &= ~bitRange(m & 0x3F, n & 0x3F); break;
      }
    } else if (lo+1 == hi) {
      // The requested range spans a quadword boundary.
      switch (lo) {
        case 0: b0 &= ~bitRange(m, 63);
                b1 &= ~bitRange(0, n & 0x3F);
                break;
        case 1: b1 &= ~bitRange(m & 0x3F, 63);
                b2 &= ~bitRange(0, n & 0x3F);
                break;
    }
    } else {
      // The requested range spans two quadword boundaries.
      b0 &= ~bitRange(m, 63);
      b1 = 0ULL;
      b2 &= ~bitRange(0, n & 0x3F);
    }
    return *this;
  }

  // Clear bits a, b, c, ....
  inline qSet& reset(std::initializer_list<bitpos> list) {
    for (bitpos i : list) {
      this->reset(i);
    }
    return *this;
  }

  // Toggle all the bits.
  inline qSet& flip() {
    b0 = ~b0;
    b1 = ~b1;
    b2 = ~b2;
    return *this;
  }

  // Toggle the indicated bit.
  inline qSet& flip(bitpos pos) {
    switch (pos >> 6) {
      case  0: b0 ^= (1ULL << (pos -  0)); break;
      case  1: b1 ^= (1ULL << (pos & 0x3F)); break;
      case  2: b2 ^= (1ULL << (pos & 0x3F)); break;
      default: throw std::out_of_range("qset.flip(pos)");
    }
    return *this;
  }

  // Toggle the range of bits [m..n-1].
  inline qSet& flip(bitpos m, bitpos n) {
    if (m == n) {
      return *this;
    }
    if (m < 0 || BITS <= m || n < m || BITS < n) {
      throw std::out_of_range("qset.flip(m,n)");
    }

    n--;
    bitpos lo = m >> 6;
    bitpos hi = n >> 6;
    if (lo == hi) {
      // The requested range fits in a single quadword.
      switch (lo) {
        case  0: b0 ^= bitRange(m, n); break;
        case  1: b1 ^= bitRange(m & 0x3F, n & 0x3F); break;
        case  2: b2 ^= bitRange(m & 0x3F, n & 0x3F); break;
      }
    } else if (lo+1 == hi) {
      // The requested range spans a quadword boundary.
      switch (lo) {
        case 0: b0 ^= bitRange(m, 63);
                b1 ^= bitRange(0, n & 0x3F);
                break;
        case 1: b1 ^= bitRange(m & 0x3F, 63);
                b2 ^= bitRange(0, n & 0x3F);
                break;
      }
    } else {
      // The requested range spans two quadword boundaries.
      b0 ^= bitRange(m, 63);
      b1 = ~b1;
      b2 ^= bitRange(0, n & 0x3F);
    }
    return *this;
  }

  // Toggle bits a, b, c, ....
  inline qSet& flip(std::initializer_list<bitpos> list) {
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

static_assert (std::is_pod< qset<192,3> >::value, "qset<BITS,3> must be POD");

#endif /* __QSET3_H__ */
