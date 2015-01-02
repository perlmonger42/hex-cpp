#include <bitset>
#include <iostream>
#include <sstream>
#include <random>
#include <functional>
#include <vector>
#include "quadset.h"
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


constexpr int smallPrimes[] = {
     2,    3,    5,    7,   11,   13,   17,   19,   23,   29,  
    31,   37,   41,   43,   47,   53,   59,   61,   67,   71,  
    73,   79,   83,   89,   97,  101,  103,  107,  109,  113,  
   127,  131,  137,  139,  149,  151,  157,  163,  167,  173,
   179,  181,  191,  193,  197,  199,  211,  223,  227,  229,
   233,  239,  241,  251,  257,  263,  269,  271,  277,  281,
   283,  293,  307,  311,  313,  317,  331,  337,  347,  349,
   353,  359,  367,  373,  379,  383,  389,  397,  401,  409,
   419,  421,  431,  433,  439,  443,  449,  457,  461,  463,
   467,  479,  487,  491,  499,  503,  509,  521,  523,  541,
   547,  557,  563,  569,  571,  577,  587,  593,  599,  601,
   607,  613,  617,  619,  631,  641,  643,  647,  653,  659,
   661,  673,  677,  683,  691,  701,  709,  719,  727,  733,
   739,  743,  751,  757,  761,  769,  773,  787,  797,  809,
   811,  821,  823,  827,  829,  839,  853,  857,  859,  863,
   877,  881,  883,  887,  907,  911,  919,  929,  937,  941,
   947,  953,  967,  971,  977,  983,  991,  997, 1009, 1013,
  1019, 1021, 1031,
};
constexpr int N_SMALL_PRIMES = sizeof(smallPrimes)/sizeof(int);
constexpr int LARGEST_TESTABLE_PRIME = smallPrimes[N_SMALL_PRIMES-1];


template<bitpos BITS>
struct SampleSets {
  std::vector< quadset<BITS>     > quadsets;
  std::vector< std::bitset<BITS> > bitsets;
};


// includePrimeSets() adds two sets to samples[], containing respectively
// all the prime numbers and all the conjugates in the range 0..BITS-1.
template<bitpos BITS>
void includePrimeSets(SampleSets<BITS> &samples) {

  static_assert (BITS <= LARGEST_TESTABLE_PRIME, "can't check primes that big");

  quadset<BITS> primeSet, compositeSet;
  std::bitset<BITS> primeBits, compositeBits;

  primeSet.reset();
  compositeSet.set();
  primeBits.reset();
  compositeBits.set();

  for (int i = 0; i < N_SMALL_PRIMES && smallPrimes[i] < BITS; ++i) {
    primeSet.set(smallPrimes[i]);
    compositeSet.reset(smallPrimes[i]);
    primeBits.set(smallPrimes[i]);
    compositeBits.reset(smallPrimes[i]);
  }
  expect_eq_set<BITS>("primes", primeBits, primeSet);
  expect_eq_set<BITS>("composites", compositeBits, compositeSet);

  EXPECT_EQ(~primeSet, compositeSet);
  EXPECT_EQ(~compositeSet, primeSet);
  samples.quadsets.push_back(primeSet);
  samples.quadsets.push_back(compositeSet);
  samples.bitsets.push_back(primeBits);
  samples.bitsets.push_back(compositeBits);
}

// includeAlternatingBitGroups adds two Sets to samples[]; the first
// is a bitset of the form 11001100... and the second is of the form
// 0011001100 -- where the length of each run is groupSize bits long.
template<bitpos BITS>
void includeAlternatingBitGroups(SampleSets<BITS> &samples, int groupSize) {
  std::bitset<BITS> onOffVec, offOnVec;
	quadset<BITS> onOffSet, offOnSet;

  onOffSet.reset();
  offOnSet.reset();

	for (int i = 0; i < BITS; ++i) {
		if ((i/groupSize) % 2 == 0) {
			onOffSet.set(i);
			onOffVec.set(i);
		} else {
			offOnSet.set(i);
			offOnVec.set(i);
		}
	}

	expect_eq_set<BITS>("on-off set", onOffVec, onOffSet);
	expect_eq_set<BITS>("off-on set", offOnVec, offOnSet);

  samples.quadsets.push_back(onOffSet);
  samples.quadsets.push_back(offOnSet);
  samples.bitsets.push_back(onOffVec);
  samples.bitsets.push_back(offOnVec);
}

// includeRandomSet adds a random set to samples[]. The set is created by
// inserting n randomly chosen elements into an empty set.
template<bitpos BITS>
void includeRandomSet(SampleSets<BITS> &samples, int n) {
  std::minstd_rand generator(1 + samples.quadsets.size());
  std::uniform_int_distribution<uint32_t> distS(0, BITS-1);
  auto randS = std::bind(distS, generator);

	auto set = quadset<BITS>::make();
  std::bitset<BITS> vec;
  
	for (int i = 0; i < n; ++i) {
    int r = randS();
		set.set(r);
    vec.set(r);
	}
	samples.quadsets.push_back(set);
	samples.bitsets.push_back(vec);
}

template<bitpos BITS>
SampleSets<BITS> *getTestSets() {
  static SampleSets<BITS> samples;
  if (samples.quadsets.size() == 0) {

    quadset<BITS> empty, full;
    std::bitset<BITS> emptyVec, fullVec;
    empty.reset();
    full.set();
    emptyVec.reset();
    fullVec.set();
    samples.quadsets.push_back(empty);
    samples.quadsets.push_back(full);
    samples.bitsets.push_back(emptyVec);
    samples.bitsets.push_back(fullVec);

    includePrimeSets<BITS>(samples);
    includeAlternatingBitGroups(samples, 7);
    includeAlternatingBitGroups(samples, 10);
    for (int i = 1; i < BITS; i *= 2) {
      includeAlternatingBitGroups(samples, i);
    }
    constexpr int randomCount = 100;
    for (int i = 1; i <= randomCount; ++i) {
      includeRandomSet(samples, 1 + (i * BITS) / randomCount);
    }
  }
  return &samples;
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
