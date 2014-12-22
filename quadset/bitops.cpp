#include "panic.h"
#include "bitops.h"

////////////////////////////////////////////////////////////////////////
// Utility functions
//

constexpr uint64_t ones = (uint64_t)(-1LL);
constexpr int maxbit = 63;
constexpr int bpq = 64;

// bitRange returns a uint64_t value with nonzero bits from m to n, inclusive.
// Note that this is a different convention from the other Range functions,
// because it INCLUDES m.
// Panics unless 0 ≤ m ≤ n < bpq.
uint64_t bitRange(int m, int n) {
	if (0 <= m && m <= n && n < bpq) {
		return (ones >> (maxbit+m-n)) << uint64_t(m);
	}
	panic("bitRange(%d, %d): illegal arguments", m, n);
  return ones;
}

// A uint64_t value that contains all possible 6-bit patterns as subseqences.
// This bit sequence starts with 6 zeros, and ends with 5 implicit zero bits
// that are created when the value is left-shifted.
// This is a de Bruijn sequence B(2, 6) -- 2 for binary, 6 for the length.
const uint64_t deBruijn26 = 0x022fdd63cc95386d;

struct hashToBitPos {
  int pos[bpq];
};

static hashToBitPos initHashToBitPos() {
  hashToBitPos h;
	for (unsigned int p = 0; p < bpq; ++p) {
		h.pos[(deBruijn26<<p)>>58] = int(p);
	}
  return h;
}

static hashToBitPos bitPosition = initHashToBitPos();

// lowestBitPosition returns the position of the lowest-order 1 in q.
// Panics if q = 0.
//
// For an explanation of how this works, see “Using de Bruijn Sequences to
// Index a 1 in a Computer Word” by Leiserson, Prokop, and Randall, MIT, 1998.
int lowestBitPosition(uint64_t q) {
	if (q == 0) {
		panic("lowestBitPosition(0) is undefined");
	}

	// q & -q clears all bits except the one at minimum position p.
	// Hence, deBruijn26 * (q & -q) is equivalent to deBruijn26 << p.
	// Shifting that result 58 bits to the right extracts a 6-bit
	// subsequence of deBruijn26. Since all its 6-bit subsequences are
	// distinct, this is a minimal perfect hash from position-of-lowest-set-bit
	// to a value in 0..63.  A table lookup then translates from the hash value
	// to the final answer.
	return bitPosition.pos[((q&-q)*deBruijn26)>>58];
}

// highestBitPosition returns the position of the maximum nonzero bit in q.
// Panics if q = 0.
int highestBitPosition(uint64_t q) {
	if (q == 0) {
		panic("highestBitPosition(0) is undefined");
	}

	// Flood-fill 1's to the right, then return 1 less than the bit count.
	// For example, 0x00080000 becomes 0x000FFFFF before counting the bits.
	q |= q >> 1;
	q |= q >> 2;
	q |= q >> 4;
	q |= q >> 8;
	q |= q >> 16;
	q |= q >> 32;
	return countBits(q) - 1;
}

// countBits returns the number of nonzero bits in q.
int countBits(uint64_t q) {
	// See section 8.6 of "Software Optimization Guide for AMD64 Processors."

	// Dividing q into 32 two-bit fields, replace each field with its bit count.
	q -= (q >> 1) & (ones / 3);
	// To see how this works, see what happens to the sample value 0b00011011.
	// This uses the trick that 0b11111111 / 3 == 0b01010101.
	// It also uses this interesting insight:
	//   00 has 0 bits  = 0b00 - 0
	//   01 has 1 bit   = 0b01 - 0
	//   10 has 1 bit   = 0b10 - 1
	//   11 has 2 bits  = 0b11 - 1
	// In other words, if you take a 2-bit number and subtract its 2's digit
	// from the number itself, you get the correct count of bits in the number.
	//
	//                           q: Aa Bb Cc Dd ... 00 01 10 11
	//
	//                      q >> 1: 0A aB bC cD ... 00 00 11 01
	//                    ones / 3: 01 01 01 01 ... 01 01 01 01
	//                              ---------------------------
	//   X = (q >> 1) & (ones / 3): 0A 0B 0C 0D ... 00 00 01 01
	// In X, we have have the value 0 or 1 in each 2-bit field, and the value
	// corresponds to the 2's column of that field's original value.
	//
	// We subtract the 2's columns from the two bit fields:
	//                           q: Aa Bb Cc Dd ... 00 01 10 11
	//     ((q >> 1) & (ones / 3)): 0A 0B 0C 0D ... 00 00 01 01
	//                              ---------------------------
	// q - ((q >> 1) & (ones / 3)): ss tt uu vv ... 00 01 01 10
	// ...where ss=Aa-A, tt=Bb-B, uu=Cc-C, and vv=Dd-D. By the logic mentioned
	// above, each of ss, tt, uu, and vv is the count of bits in the
	// corresponding field Aa, Bb, Cc, and Dd.  So each 2-bit field of q has been
	// replaced with the number of 1-bits it had.

	// Add the counts in adjacent 2-bit groups, producing 4-bit sums.
	q = (q&(ones/15*3)) + ((q>>2)&(ones/15*3));
	// This uses the trick that 0b11111111 / 15 * 3 == 0b00110011.
	//                           q: 00 00 ... aa bb cc dd 00 01 10 11
	//                   ones/15*3: 00 11 ... 00 11 00 11 00 11 00 11
	//    X = (q>>2) & (ones/15*3): 00 00 ... 00 aa 00 cc 00 00 00 10
	//    Y =      q & (ones/15*3): 00 00 ... 00 bb 00 dd 00 01 00 11
	//                       X + Y: 00 00 ... 0s ss 0t tt 00 01 01 01
	//                              (where aa+bb = sss and cc+dd=ttt)

	// Add the counts in adjacent 4-bit groups, producing 8-bit sums.
	q = (q + (q>>4)) & (ones / 255 * 15);
	// This uses 0b1111111111111111 / 255 * 15 == 0b0000111100001111.
	// We can do a little less work at this step, because the sums (being the
	// count of bits in an eight-bit field) will fit in four bits. So we don't
	// have to clear alternate 4-bit blocks in the two inputs before we add them.
	// We can just clear the alternate blocks in the result.
	//
	//                    X = q>>4: 0000 0aaa 0bbb 0ccc
	//                    Y =    q: 0aaa 0bbb 0ccc 0ddd
	//                       X + Y: xxxx ssss xxxx tttt
	//
	//        MASK = (ones/255*15): 0000 1111 0000 1111
	//              (X + Y) & MASK: 0000 ssss 0000 tttt
	// ...where ssss=aaa+bbb and tttt=ccc+ddd, and xxxx means "doesn't matter".
	// It is important to note that, though the xxxx's represent don't-cares
	// (because we're going to mask them out anyway), we know that xxxx <= 8.
	// That means each xxxx is small enough that it will never generate a
	// carry-in to the 4-bit field to its left, which we DO care about.

	// Add up all the 8-bit counts and return the sum.
	return int((q * (ones / 255)) >> (bpq - 8));
	// This uses 0b111111111111111111111111 / 255 = 0b000000010000000100000001.
	// In the following diagrams, each character represents a byte, not a bit.
	//
	//                           q: a b c d e f g h
	//                    ones/255: 1 1 1 1 1 1 1 1
	//
	// Let's multiplipy those two values in long hand, using base-256 notation.
	// Let q = a b c d e f g, where a, b, c, d, e, f, g, and h each represent a
	// byte. Each contains a value between 0 and 8 -- we know that's true,
	// because each represents the bit count of an 8-bit field of the original q.
	//                              a b c d e f g h
	//                            * 1 1 1 1 1 1 1 1
	//               ------------------------------
	//                              a b c d e f g h
	//                            a b c d e f g h 0
	//                          a b c d e f g h 0 0
	//                        a b c d e f g h 0 0 0
	//                      a b c d e f g h 0 0 0 0
	//                    a b c d e f g h 0 0 0 0 0
	//                  a b c d e f g h 0 0 0 0 0 0
	//             +  a b c d e f g h 0 0 0 0 0 0 0
	//               ------------------------------
	//                              S x x x x x x x
	// ... where S = a + b + c + d + e + f + g + h. The x's are don't-cares.
	// The columns to the left of S get thrown away when the 128-bit product is
	// reduced to a 64-bit result.  S, and each x, is the sum of 8 numbers,
	// each of which is between 0 and 8.  Thus S, and each x, is between 0 and
	// 64, which means that each fits easily in its own 8-bit column sum, with no
	// overflow to its right-hand neighbor.
	//
	// All we need to do is grab S, the high-order byte of the product.
	//               product >> 56: 0 0 0 0 0 0 0 S
}

// loopCountBits returns the number of nonzero bits in a uint64_t.
// Benchmarking shows that this is about 10 times slower than the sophisticated
// countBits implementation above.
int loopCountBits(uint64_t q) {
  int count = 0;
	while (q != 0) {
		count += int(q & 1);
		q = q >> 1;
	}
	return count;
}

struct lookupBitsInByte {
  unsigned char count[256];
};

static lookupBitsInByte initLookupBitsInByte() {
  lookupBitsInByte table;
	for (uint64_t i = 0; i < 256; ++i) {
		table.count[i] = (unsigned char) countBits(i);
	}
  return table;
}

static lookupBitsInByte byteTable = initLookupBitsInByte();

// byteTableCountBits returns the number of nonzero bits in a uint64_t.
// Benchmarking shows that this is about twice as slow as the sophisticated
// countBits implementation above.
int byteTableCountBits(uint64_t q) {
	return int(byteTable.count[(q>> 0) & 0xFF]) +
		     int(byteTable.count[(q>> 8) & 0xFF]) +
		     int(byteTable.count[(q>>16) & 0xFF]) +
		     int(byteTable.count[(q>>24) & 0xFF]) +
		     int(byteTable.count[(q>>32) & 0xFF]) +
		     int(byteTable.count[(q>>40) & 0xFF]) +
		     int(byteTable.count[(q>>48) & 0xFF]) +
		     int(byteTable.count[(q>>56) & 0xFF]);
}

struct lookupBitsInWord {
  unsigned char count[0x10000];
};

static lookupBitsInWord initLookupBitsInWord() {
  lookupBitsInWord table;
	for (uint64_t i = 0; i < 0x10000; ++i) {
		table.count[i] = (unsigned char) countBits(i);
	}
  return table;
}

static lookupBitsInWord wordTable = initLookupBitsInWord();

// wordTableCountBits returns the number of nonzero bits in a uint64_t.
// Benchmarking shows that this is slightly faster than the sophisticated
// countBits implementation above.
int wordTableCountBits(uint64_t q) {
	return int(wordTable.count[(q >>  0) & 0xFFFF]) +
		     int(wordTable.count[(q >> 16) & 0xFFFF]) +
		     int(wordTable.count[(q >> 32) & 0xFFFF]) +
		     int(wordTable.count[(q >> 48) & 0xFFFF]);
}

// Here are benchmark results for the four different countBits approaches:
//     Benchmark_wordTableCountBits    1849 ns/op
//     Benchmark_countBits             2068 ns/op
//     Benchmark_byteTableCountBits    3833 ns/op
//     Benchmark_loopCountBits        19737 ns/op
// These numbers are the average of four runs. An "op" consists of counting the
// bits in each of the four quadwords of 122 different sets (the testSets[]
// defined in set_test.go).

int loopHighestBitPosition(uint64_t q) {
	uint64_t m = 1ULL << maxbit;
	for (int pos = maxbit; m != 0; m >>= 1, --pos) {
		if ((m&q) != 0) {
			return pos;
		}
	}
	panic("loopHighestBitPosition(0) is undefined");
  return -1;
}

// floodFillLowestBitPosition returns the position of the maximum nonzero bit
// in q.  Panics if q = 0.
int floodFillLowestBitPosition(uint64_t q) {
	if (q == 0) {
		panic("floodFillLowestBitPosition(0) is undefined");
	}

	// Flood-fill 1's to the right, then return 64 minus the bit count.
	// For example, 0x00080000 becomes 0x000FFFFF before counting the bits.
	q |= q <<  1;
	q |= q <<  2;
	q |= q <<  4;
	q |= q <<  8;
	q |= q << 16;
	q |= q << 32;
	return bpq - countBits(q);
}

int loopLowestBitPosition(uint64_t q) {
	uint64_t m = 1;
	for (int pos = 0; m != 0; m <<= 1, ++pos) {
		if ((m&q) != 0) {
			return pos;
		}
	}
	panic("loopLowestBitPosition(0) is undefined");
  return maxbit + 1;
}

///////////////////////////////////////////////////////////////////////////////|
