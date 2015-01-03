/* vim: set ft=cpp tw=75: */
#include <string.h>
#include "bitops.h"
#include "quadset.h"
#include "samplesets.h"
#include "gtest/gtest.h"

TEST(SET, CountBits) {
  auto samples = getTestSets<64>();
  for (int i = 0; i < samples->quadsets.size(); ++i) {
    auto a = samples->quadsets.at(i);
    unsigned long long u64 = a.b0;
    auto loopCount = loopCountBits(u64);
    EXPECT_EQ(loopCount, countBits(u64));
    EXPECT_EQ(loopCount, __builtin_popcountll(u64));
  }
}

TEST(SET, LowestSetBit) {
  auto samples = getTestSets<64>();
  for (int i = 0; i < samples->quadsets.size(); ++i) {
    auto a = samples->quadsets.at(i);
    unsigned long long u64 = a.b0;
    if (u64 != 0) {
      auto loopLowest = loopLowestBitPosition(u64);
      EXPECT_EQ(loopLowest, lowestBitPosition(u64));
      EXPECT_EQ(loopLowest, floodFillLowestBitPosition(u64));
      EXPECT_EQ(loopLowest, ffsll(u64)-1);
      EXPECT_EQ(loopLowest, __builtin_ctzll(u64));
    }
  }
}
