#ifndef __BITOPS_H__
#define __BITOPS_H__ 1
#include <stdint.h>

////////////////////////////////////////////////////////////////////////
// Utility functions
//


// bitRange returns a uint64_t value with nonzero bits from m to n, inclusive.
// Note that is is a different convention from the other Range functions,
// because it INCLUDES m.
// Panics unless 0 ≤ m ≤ n < bpq.
uint64_t bitRange(int m, int n);


// lowestBitPosition returns the position of the lowest-order 1 in q.
// Panics if q = 0.
int lowestBitPositionDeBruijn(uint64_t q);
int lowestBitPositionFloodFill(uint64_t q);
int lowestBitPositionLoop(uint64_t q);
#ifdef __GNUC__
#define lowestBitPosition(q) __builtin_ctzll(q)
#else
#define lowestBitPosition(q) lowestBitPositionDeBruijn(q)
#endif
// I would prefer to use the following inline function than the preceding
// macro definition, but benchmarking shows a 25% slowdown with that approach.
// inline int lowestBitPosition(uint64_t q) {
//   #ifdef __GNUC__
//     return __builtin_ctzll(q); // Count Trailing Zeros
//   #else
//     return lowestBitPositionDeBruijn(q);
//   #endif
// }

// highestBitPosition returns the position of the highest-order 1 in q.
// Panics if q = 0.
int highestBitPositionLoop(uint64_t q);
int highestBitPositionFloodFillAndCount(uint64_t q);
int highestBitPositionDeBruijn(uint64_t q);
int highestBitPositionDivideAndConquer(uint64_t q);
int highestBitPositionExtremeDivideAndConquer(uint64_t q);
int highestBitPositionDoubleConversion(uint64_t q);
#ifdef __GNUC__
#define highestBitPosition(q) (63 - __builtin_clzll(q))
#else
#define highestBitPosition(q) highestBitPositionDeBruijn(q)
#endif
// I would prefer to use the following inline function than the preceding
// macro definition, but benchmarking shows a 25% slowdown with that approach.
// inline int highestBitPosition(uint64_t q) {
//   #ifdef __GNUC__
//     return 63 - __builtin_clzll(q); // Count Leading Zeros
//   #else
//     return highestBitPositionExtremeDivideAndConquer(q);
//   #endif
// }


// countBits returns the number of nonzero bits in q.
int countBitsCunning(uint64_t q);

// Benchmarking shows that this is about 10 times slower than the sophisticated
// countBits implementation above.
int countBitsLoop(uint64_t q);

// Benchmarking shows that this is about twice as slow as the sophisticated
// countBits implementation above.
int countBitsByteTable(uint64_t q);

// Benchmarking shows that this is slightly faster than the sophisticated
// countBits implementation above.
int countBitsWordTable(uint64_t q);

#ifdef __GNUC__
#define countBits(q) __builtin_popcountll(q)
#else
#define countBits(q) countBitsCunning(q)
#endif
// I would prefer to use the following inline function than the preceding
// macro definition, but benchmarking shows a 25% slowdown with that approach.
// inline int countBits(uint64_t q) {
//   #ifdef __GNUC__
//     return __builtin_popcountll(q); // Population Count
//   #else
//     return countBitsCunning(q);
//   #endif
// }

///////////////////////////////////////////////////////////////////////////////|
#endif /* __BITOPS_H__ */
