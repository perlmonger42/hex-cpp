#ifndef __QUADSET_H__
#define __QUADSET_H__ 1
#include <iostream>
#include <sstream>

// The quadset type represents a set of small integers, similar to std::bitset.
// However, instead of being infinitely variable in size, quadsets are
// restricted to at most 256 bits.
//
// A quadset inherits from one of four template specializations, having
// capacities of 1, 2, 3, or 4 quadwords (i.e., 64, 128, 192, or 256 bits).
//
// All non-lvalue methods pass quadsets by value. If your set is too
// big to be passed by value, consider using std::bitset instead.


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
//
//
// quadset<BITS> is simply a subclass of the appropriate qset<BITS,QUADS>.



// Forward declaration of qset<BITS,QUADWORDS>.
template<bitpos BITS, bitpos QUADWORDS = (BITS+63) / 64>
class qset;


// quadset<BITS> is a subclass of the right-sized specialization.
// This is almost all it has to be, since all the real work is done in
// qset<BITS,QUADWORDS>.
// Unfortunately, it's not that simple, because the result type of all the
// operators would be wrong (for example, quadset<70>::operator| returns a
// value of type qset<70,2> instead of quadset<70>). In consequence,
// quadset<BITS> has to provide a bunch of wrappers that adjust the type.
template<bitpos BITS>
class quadset : public qset<BITS> {
public:

  typedef quadset<BITS> quadSet;
  typedef qset<BITS>    qSet;

  static constexpr quadSet mk(qSet set) {
    return * (quadSet*) &set;
  }

  static constexpr quadSet make() {
    return mk(qSet::make());
  }

  template<typename... Args>
  static constexpr quadSet make(bitpos p, Args... args) {
    return mk(qSet::make(p, args...));
  }

};


template<bitpos BITS>
std::ostream& print(std::ostream &out, qset<BITS> set) {
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

//template<int BITS>
//qset<BITS> set(std::initializer_list<bitpos> list) {
//  qset<BITS> result;
//  result.clear();
//  for (int i : list) {
//    result.set(i);
//  }
//  return result;
//}

#endif /* __QUADSET_H__ */
