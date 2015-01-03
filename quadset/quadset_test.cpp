#include <bitset>
#include <iostream>
#include <sstream>
#include <random>
#include <functional>
#include <vector>
#include "quadset.h"
#include "../hex/cellset.h"
#include "bitops.h"
#include "samplesets.h"
#include "gtest/gtest.h"

typedef quadset<63> set63;
typedef quadset<64> set64;
typedef quadset<65> set65;
typedef quadset<96> set96;
typedef quadset<127> set127;
typedef quadset<128> set128;
typedef quadset<129> set129;
typedef quadset<160> set160;
typedef quadset<191> set191;
typedef quadset<192> set192;
typedef cellset<8>   cellset8;


template<bitpos BITS>
void expect_eq_set(
    std::string label,
    std::bitset<BITS> bset,
    quadset<BITS> quad
) {
  for (bitpos i = 0; i < BITS; ++i) {
    EXPECT_EQ((bset.test(i)?1000:0)+i, (quad.test(i)?1000:0)+i) << label;
  }
}

enum speed_tradeoff {
  ACCURATE,
  FAST
};

template<bitpos BITS>
void expect_op_result(
    std::string op,
    std::bitset<BITS> bits,
    quadset<BITS> q1,
    quadset<BITS> q2,
    quadset<BITS> quad,
    speed_tradeoff tradeoff = ACCURATE
) {
  for (bitpos i = 0; i < BITS; ++i) {
    if (bits.test(i) != quad.test(i)) {
      EXPECT_EQ(false, true) << q1.to_string() << ' ' << op << ' '
        << q2.to_string() << " = " << quad.to_string()
        << "; expected " << bits;
    }
  }
  if (tradeoff != FAST) {
    if ((BITS & 0x3F) != 0) {
      for (bitpos i = BITS; i < ((BITS+63) & ~0x3F); ++i) {
        EXPECT_EQ(false, quad.test(i)) << "found set bit (#"
          << i << ") out of set range in " << quad.to_string() << op;
      }
    }
  }
}

template<bitpos BITS>
int loopCountBits(quadset<BITS> s) {
  int count = 0;
  for (int i=0; i < s.size(); ++i) {
    if (s.test(i)) {
      ++count;
    }
  }
  return count;
}

template<bitpos BITS>
void testShift(std::string label, quadset<BITS> got, quadset<BITS> base, bitpos shift, speed_tradeoff tradeoff) {
  for (bitpos i = 0; i < BITS; ++i) {
    if (got.test(i) != base.test(i-shift)) {
      std::cout << label << ": "
                << "got[" << i << "] is " << got.test(i) << "; base["
                << i-shift << "] is " << base.test(i-shift) << "; "
                << "(i-shift) / 64 is " << ((i-shift)/64)
                << "; 1ULL << " << (i-shift) << " is "
                << (1ULL << (i-shift))
                << std::endl;
      EXPECT_EQ(base.to_string(), got.to_string()) << label << " " << shift;
    }
  }
  if (tradeoff != FAST) {
    if ((BITS & 0x3F) != 0) {
      for (bitpos i = BITS; i < ((BITS+63) & ~0x3F); ++i) {
        EXPECT_EQ(false, got.test(i)) << "found set bit (#"
          << i << ") out of set range in " << got.to_string() << "<<" << shift;
      }
    }
  }
}


#define SET set63
#define SIZE 63
#include "quadset_test.inc"
#undef SET
#undef SIZE

#define SET set64
#define SIZE 64
#include "quadset_test.inc"
#undef SET
#undef SIZE

#define SET set65
#define SIZE 65
#include "quadset_test.inc"
#undef SET
#undef SIZE

#define SET set96
#define SIZE 96
#include "quadset_test.inc"
#undef SET
#undef SIZE

#define SET set127
#define SIZE 127
#include "quadset_test.inc"
#undef SET
#undef SIZE

#define SET set128
#define SIZE 128
#include "quadset_test.inc"
#undef SET
#undef SIZE

#define SET set129
#define SIZE 129
#include "quadset_test.inc"
#undef SET
#undef SIZE

#define SET set160
#define SIZE 160
#include "quadset_test.inc"
#undef SET
#undef SIZE

#define SET set191
#define SIZE 191
#include "quadset_test.inc"
#undef SET
#undef SIZE

#define SET set192
#define SIZE 192
#include "quadset_test.inc"
#undef SET
#undef SIZE

#define SET cellset8
#define SIZE 64
#include "quadset_test.inc"
#undef SET
#undef SIZE
