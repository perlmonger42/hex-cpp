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
    auto loopCount = countBitsLoop(u64);
    EXPECT_EQ(loopCount, countBits(u64));
    EXPECT_EQ(loopCount, countBitsByteTable(u64));
    EXPECT_EQ(loopCount, countBitsWordTable(u64));
    EXPECT_EQ(loopCount, countBitsCunning(u64));
    EXPECT_EQ(loopCount, __builtin_popcountll(u64));
  }
}

TEST(SET, LowestSetBit) {
  auto samples = getTestSets<64>();
  for (int i = 0; i < samples->quadsets.size(); ++i) {
    auto a = samples->quadsets.at(i);
    unsigned long long u64 = a.b0;
    if (u64 != 0) {
      auto loopLowest = lowestBitPositionLoop(u64);
      EXPECT_EQ(loopLowest, lowestBitPosition(u64));
      EXPECT_EQ(loopLowest, lowestBitPositionDeBruijn(u64));
      EXPECT_EQ(loopLowest, lowestBitPositionFloodFill(u64));
      EXPECT_EQ(loopLowest, ffsll(u64)-1);
      EXPECT_EQ(loopLowest, __builtin_ctzll(u64));
    }
  }
}

TEST(SET, HighestSetBit) {
  auto samples = getTestSets<64>();
  for (int i = 0; i < samples->quadsets.size(); ++i) {
    auto a = samples->quadsets.at(i);
    unsigned long long u64 = a.b0;
    if (u64 != 0) {
      auto loopHighest = highestBitPositionLoop(u64);
      EXPECT_EQ(loopHighest, highestBitPosition(u64));
      EXPECT_EQ(loopHighest, highestBitPositionFloodFillAndCount(u64));
      EXPECT_EQ(loopHighest, highestBitPositionDivideAndConquer(u64));
      EXPECT_EQ(loopHighest, highestBitPositionExtremeDivideAndConquer(u64));
      EXPECT_EQ(loopHighest, highestBitPositionDeBruijn(u64));
      EXPECT_EQ(loopHighest, highestBitPositionDoubleConversion(u64));
      EXPECT_EQ(loopHighest, flsll(u64)-1);
      EXPECT_EQ(loopHighest, 63 - __builtin_clzll(u64));
    }
  }
}
