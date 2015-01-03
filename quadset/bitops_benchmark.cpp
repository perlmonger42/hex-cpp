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

void Benchmark_DeBruijnLowbit(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += lowestBitPosition(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_IntrinsicLowbit(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += __builtin_ctzll(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_FfsllLowbit(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += ffsll(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

void Benchmark_LoopCountBits(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += loopCountBits(random_u64s[j]);
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

void Benchmark_Popcount(Benchmark &b) {
  unsigned long long localSum = 0;
  for (int i = 0; i < b.N; ++i) {
    for (int j = 0; j < sizeof(random_u64s)/sizeof(random_u64s[0]); ++j) {
      localSum += __builtin_popcountll(random_u64s[j]);
    }
  }
  globalSum += localSum;
}

int main() {
  build_random_quads();

  BENCHMARK(Benchmark_DeBruijnLowbit);
  BENCHMARK(Benchmark_IntrinsicLowbit);
  BENCHMARK(Benchmark_FfsllLowbit);

  BENCHMARK(Benchmark_LoopCountBits);
  BENCHMARK(Benchmark_CountBits);
  BENCHMARK(Benchmark_Popcount);
  return 0;
}
