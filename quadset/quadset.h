#ifndef __QUADSET_H__
#define __QUADSET_H__ 1
#include <iostream>
#include <sstream>

// bit_mask returns a uint64 value with nonzero bits from m to n, inclusive.
// Note that is is a different convention from most other interfaces dealing
// with ranges, because it INCLUDES m.
// Requires 0 ≤ m ≤ n < 64.
inline constexpr uint64_t bit_mask(int m, int n) {
  //static_assert(0 <= m, "bit_mask(m,n) requires 0 <= m");
  //static_assert(m <= n, "bit_mask(m,n) requires m <= n");
  //static_assert(n < 64, "bit_mask(m,n) requires n < 64");
  return (~0ULL >> (63+m-n)) << m;
}
    

// quadset is a "set of integer" type similar to std::bitset, but instead of
// being infinitely variable in size, it is restricted to 256 bits at most.
//
// A quadset inherits from one of four template specializations, having
// capacities of 1, 2, 3, or 4 quadwords (i.e., 64, 128, 192, or 256 bits).
//
// All non-lvalue methods pass quadsets by value. If your set is too
// big to be passed by value, consider using std::bitset instead.


// Since the elements of quadsets are constrained to the range [0..255],
// uint8_t would be large enough to store any element. However, -1 and 256 are
// useful as off-the-end inputs, even though they can't be stored in a quadset.
// So instead, we use the next-larger signed type instead.
typedef int_fast16_t bitpos;


// The inheritance hierarchy for quadset<BITS> is as follows (arrows point
// from subclass to superclass):
//
//                +---->  qset<BITS,1>  --+        
//                |                       |      
//                +---->  qset<BITS,2>  --+      
//                |                       |      
// qbase<BITS>  --+---->  qset<BITS,3>  --+---->  quadset<BITS>
//                |                       |      
//                +---->  qset<BITS,4>  --+      
//
// 
// qbase<BITS> is the base class of all the other bitset classes.
// Code that doesn't need to be specialized based on the number of quadwords
// goes in qbase<BITS>.
//
// qset<BITS,1>, qset<BITS,2>, qset<BITS,3>, and qset<BITS,4> are the four
// template specializations for handling capacities of 1, 2, 3, and 4 quads.
//
//
// quadset<BITS> is simply a subclass of the appropriate qset<BITS,QUADS>.



// Forward declaration of qbase<BITS>.
template<bitpos BITS>
class qbase;

// Forward declaration of qset<BITS,QUADWORDS>.
template<bitpos BITS, bitpos QUADWORDS = (BITS+63) / 64>
class qset;


//  // quadset<BITS> is simply a subclass of the right-sized specialization.
//  // This is the complete implementation of the class, as nothing else is needed.
//  // All the real work is done in qbase<BITS> and qset<BITS,QUADWORDS>.
//  template<bitpos BITS>
//  class quadset : public qset<BITS> {
//  public:
//    static inline constexpr bitpos size() { return BITS; }
//  
//    static inline constexpr quadset<BITS> range(bitpos m, bitpos n) {
//      quadset<BITS> result = {0};
//      result.set(m, n);
//      return result;
//    }
//  
//  };



// qbase<BITS> is the base class of all the other bitset classes.
// Code that doesn't need to be specialized based on the number of quadwords
// goes here.
template<bitpos BITS>
class qbase {
public:
  typedef qbase<BITS> qBase;
  typedef qset<BITS> qSet;

  const static bitpos LIMIT = BITS;
  const static bitpos LAST = BITS - 1;

  static inline constexpr qSet repeat_block(
    qSet block,
    bitpos blockSize,
    bitpos count,
    qSet accumulator = qSet{ 0 }
  ) {
    return count == 0 ? accumulator :
      repeat_block(block, blockSize, count-1, (accumulator<<blockSize) | block);
  }

  static inline constexpr qSet universe() {
    return repeat_block(qSet{1}, 1, BITS);
  }

  static std::ostream& print(std::ostream &out, qSet set) {
    out << '{';
    const int LIMIT = BITS;
    bitpos rangeFirst, rangeFinal;
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
        rangeFirst = rangeFinal = LIMIT + 1;
      }
    }
    out << '}';
    return out;
  }

  static inline std::string to_string(qSet set) {
    std::ostringstream stream;
    set.print(stream);
    return stream.str();
  }
  
};

template<int BITS>
std::ostream& operator<< (std::ostream &out, qset<BITS> set) {
  return qset<BITS>::print(out, set);
}

template<int BITS>
qset<BITS> set(std::initializer_list<bitpos> list) {
  qset<BITS> result;
  result.clear();
  for (int i : list) {
    result.set(i);
  }
  return result;
}

#endif /* __QUADSET_H__ */
