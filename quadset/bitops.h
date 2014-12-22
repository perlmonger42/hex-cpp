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
int lowestBitPosition(uint64_t q);


// highestBitPosition returns the position of the maximum nonzero bit in q.
// Panics if q = 0.
int highestBitPosition(uint64_t q);


// countBits returns the number of nonzero bits in q.
int countBits(uint64_t q);


// loopCountBits returns the number of nonzero bits in a uint64_t.
// Benchmarking shows that this is about 10 times slower than the sophisticated
// countBits implementation above.
int loopCountBits(uint64_t q);


// byteTableCountBits returns the number of nonzero bits in a uint64_t.
// Benchmarking shows that this is about twice as slow as the sophisticated
// countBits implementation above.
int byteTableCountBits(uint64_t q);


// wordTableCountBits returns the number of nonzero bits in a uint64_t.
// Benchmarking shows that this is slightly faster than the sophisticated
// countBits implementation above.
int wordTableCountBits(uint64_t q);


// floodFillLowestBitPosition returns the position of the maximum nonzero bit
// in q.  Panics if q = 0.
int floodFillLowestBitPosition(uint64_t q);


// loopLowestBitPosition returns the position of the minimum nonzero bit
// in q.  Panics if q = 0.
int loopLowestBitPosition(uint64_t q);


// loopHighestBitPosition returns the bit index of the highest-order 1 in q.
// Panics if q = 0.
int loopHighestBitPosition(uint64_t q);

///////////////////////////////////////////////////////////////////////////////|
