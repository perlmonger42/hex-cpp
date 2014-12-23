TL;DR
=====
The C++ implementation of winner-testing of Hex game boards is about
five times faster than an equivalent golang implementation. This C++
implementation uses handcrafted bitsets based on 64-bit unsigned integers,
rather than using the standard library's `std::bitset`.

Adapting the C++ implementation to use `std::bitset` results in a program
that is 1500 times _slower_. I strongly suspect that most of the slowdown is
caused by the shift operations. Looking at the standard library sources, it
appears that bit shifts are using bit-by-bit iterators. That compares very
poorly with the handcrafted bitsets, which use the integer shift operators.


Benchmark Information
=====================

The values given here are the average of three runs.
The raw data is shown below.

Using quadset:

  | Test                            | Timing        |
  | :------------------------------ | ------------: |
  | Benchmark_Winner                |    1298 ns/op |
  | Benchmark_SimpleVirtualWinner   |    1336 ns/op |


Using std::bitset:

  | Test                            | Timing        |
  | :------------------------------ | ------------: |
  | Benchmark_Winner                | 2204259 ns/op |
  | Benchmark_SimpleVirtualWinner   | 2151541 ns/op |


An equivalent program written in Go:

  | Test                            | Timing        |
  | :------------------------------ | ------------: |
  | Benchmark_Winner                |    6072 ns/op |
  | Benchmark_SimpleVirtualWinner   |    6725 ns/op |


Raw Data
========

```bash
  # Using quadsets, winner_benchmark takes less than 1400 nanoseconds:
  $ git rev-parse master
  a2e2373a29bb0c8f20e1f2919d7d9d2f1dfe82fb

  $ /tmp/hex-build/hex/winner_benchmark
  Benchmark_Winner	1000000	1282 ns/op
  Benchmark_SimpleVirtualWinner	1000000	1357 ns/op

  $ /tmp/hex-build/hex/winner_benchmark
  Benchmark_Winner	1000000	1307 ns/op
  Benchmark_SimpleVirtualWinner	1000000	1315 ns/op

  $ /tmp/hex-build/hex/winner_benchmark
  Benchmark_Winner	1000000	1307 ns/op
  Benchmark_SimpleVirtualWinner	1000000	1338 ns/op
```

------------------------------------------------------------------------

```bash
  # Using std::bitset, winner_benchmark takes more than 2 seconds:
  $ git rev-parse explore/std-bitset
  c53ca2c9f13ae8c8d9d76886505f5b727fa97587

  $ /tmp/hex-build/hex/winner_benchmark
  Benchmark_Winner	1000	2251779 ns/op
  Benchmark_SimpleVirtualWinner	1000	2221055 ns/op

  $ /tmp/hex-build/hex/winner_benchmark
  Benchmark_Winner	1000	2224640 ns/op
  Benchmark_SimpleVirtualWinner	1000	2121609 ns/op

  $ /tmp/hex-build/hex/winner_benchmark
  Benchmark_Winner	1000	2136358 ns/op
  Benchmark_SimpleVirtualWinner	1000	2111961 ns/op
```

------------------------------------------------------------------------

```bash
  # An equivalent program in Go:
  $ go test github.com/perlmonger42/hex --bench=.
  PASS
  Benchmark_Winner	  500000	      6068 ns/op
  Benchmark_SimpleVirtualWinner	  500000	      6718 ns/op
  ok  	github.com/perlmonger42/hex	6.531s

  $ go test github.com/perlmonger42/hex --bench=.
  PASS
  Benchmark_Winner	  500000	      6073 ns/op
  Benchmark_SimpleVirtualWinner	  500000	      6737 ns/op
  ok  	github.com/perlmonger42/hex	6.545s

  $ go test github.com/perlmonger42/hex --bench=.
  PASS
  Benchmark_Winner	  500000	      6075 ns/op
  Benchmark_SimpleVirtualWinner	  500000	      6720 ns/op
  ok  	github.com/perlmonger42/hex	6.534s
```
