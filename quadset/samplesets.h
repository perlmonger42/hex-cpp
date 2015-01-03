#include <cassert>
//#include <bitset>
//#include <iostream>
//#include <sstream>
#include <random>
#include <functional>
#include <vector>
//#include "quadset.h"
//#include "../hex/cellset.h"
//#include "gtest/gtest.h"

template<bitpos BITS>
struct SampleSets {
  std::vector< quadset<BITS>     > quadsets;
  std::vector< std::bitset<BITS> > bitsets;
};

template<bitpos BITS>
void assert_bitset_eq_quadset(
    std::string label,
    std::bitset<BITS> bset,
    quadset<BITS> quad
) {
  for (bitpos i = 0; i < BITS; ++i) {
    assert(bset.test(i) == quad.test(i));
  }
}

// includePrimeSets() adds two sets to samples[], containing respectively
// all the prime numbers and all the composites in the range 0..BITS-1.
template<bitpos BITS>
void includePrimeSets(SampleSets<BITS> &samples) {
  extern int smallPrimes[];
  extern int N_SMALL_PRIMES;
  extern int LARGEST_TESTABLE_PRIME;

  assert(BITS <= LARGEST_TESTABLE_PRIME); // can't check primes that big

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

	assert_bitset_eq_quadset<BITS>("prime set", primeBits, primeSet);
	assert_bitset_eq_quadset<BITS>("composite set", compositeBits, compositeSet);

  assert(~primeSet == compositeSet);
  assert(~compositeSet == primeSet);
  assert(~primeBits == compositeBits);
  assert(~compositeBits == primeBits);

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

	assert_bitset_eq_quadset<BITS>("on-off set", onOffVec, onOffSet);
	assert_bitset_eq_quadset<BITS>("off-on set", offOnVec, offOnSet);

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
