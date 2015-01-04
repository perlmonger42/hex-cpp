#include <vector>
#include <random>
#include <functional>
#include <string.h>
#include "bitops.h"
#include "quadset.h"
#include "../benchmark.h"

quadset<64> random_quads[1000];
unsigned long long random_u64s[1000];

void build_random_quads() {
  //std::minstd_rand generator;
  //std::uniform_int_distribution<unsigned long long> ull_dist;
  //auto rand = std::bind(ull_dist, generator);
  
  for (int i = 0; i < sizeof(random_quads)/sizeof(random_quads[0]); i++) {
    //unsigned long long r;
    //for (r=rand(); r == 0; r=rand())
    //  ;
    //random_quads[i] = quadset<64>(r);
    //random_u64s[i] = r;
    random_quads[i] = quadset<64>(i+1);
    random_u64s[i] = i+1;
  }
}

unsigned long long globalSum;

void Benchmark_LowestBitPositionFloodFill(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += lowestBitPositionFloodFill(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_LowestBitPositionLoop(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += lowestBitPositionLoop(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_LowestBitPositionDeBruijn(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += lowestBitPositionDeBruijn(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_LowestBitPositionFfsll(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += ffsll(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_LowestBitPositionIntrinsic(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += __builtin_ctzll(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_LowestBitPosition(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += lowestBitPosition(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_HighestBitPositionLoop(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += highestBitPositionLoop(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_HighestBitPositionDeBruijn(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += highestBitPositionDeBruijn(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_HighestBitPositionFloodFillAndCount(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += highestBitPositionFloodFillAndCount(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_HighestBitPositionDivideAndConquer(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += highestBitPositionDivideAndConquer(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_HighestBitPositionExtremeDivideAndConquer(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += highestBitPositionExtremeDivideAndConquer(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_HighestBitPositionDoubleConversion(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += highestBitPositionDoubleConversion(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_HighestBitPositionFlsll(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += flsll(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_HighestBitPositionIntrinsic(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += 63 - __builtin_clzll(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_HighestBitPosition(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += highestBitPosition(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_CountBitsLoop(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += countBitsLoop(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_CountBitsCunning(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += countBitsCunning(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_CountBitsByteTable(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += countBitsByteTable(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_CountBitsWordTable(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += countBitsWordTable(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_Popcount(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += __builtin_popcountll(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_CountBits(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += countBits(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

int main() {
  build_random_quads();

  BENCHMARK(Benchmark_LowestBitPositionFloodFill);
  BENCHMARK(Benchmark_LowestBitPositionLoop);
  BENCHMARK(Benchmark_LowestBitPositionDeBruijn);
  BENCHMARK(Benchmark_LowestBitPositionFfsll);
  BENCHMARK(Benchmark_LowestBitPositionIntrinsic);
  BENCHMARK(Benchmark_LowestBitPosition);
  std::cout << std::endl;

  BENCHMARK(Benchmark_HighestBitPositionLoop);
  BENCHMARK(Benchmark_HighestBitPositionFloodFillAndCount);
  BENCHMARK(Benchmark_HighestBitPositionDeBruijn);
  BENCHMARK(Benchmark_HighestBitPositionDivideAndConquer);
  BENCHMARK(Benchmark_HighestBitPositionExtremeDivideAndConquer);
  BENCHMARK(Benchmark_HighestBitPositionDoubleConversion);
  BENCHMARK(Benchmark_HighestBitPositionFlsll);
  BENCHMARK(Benchmark_HighestBitPositionIntrinsic);
  BENCHMARK(Benchmark_HighestBitPosition);
  std::cout << std::endl;

  BENCHMARK(Benchmark_CountBitsLoop);
  BENCHMARK(Benchmark_CountBitsCunning);
  BENCHMARK(Benchmark_CountBitsByteTable);
  BENCHMARK(Benchmark_CountBitsWordTable);
  BENCHMARK(Benchmark_Popcount);
  BENCHMARK(Benchmark_CountBits);
  return 0;
}
