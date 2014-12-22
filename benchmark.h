#ifndef __BENCHMARK_H__
#define __BENCHMARK_H__
#include <algorithm>
#include <chrono>
#include <vector>

// This benchmarking code is based on the Go benchmarking functionality.
// See /usr/local/go/src/pkg//testing/benchmark.go

typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

struct TimeInterval {
  TimePoint::duration d;

  TimeInterval() : d(0) { }

  template<typename Duration>
  TimeInterval(Duration _d) : d(_d) { }

  int64_t Nanoseconds() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(d).count();
  }

  bool operator< (TimeInterval other) {
    return this->d < other.d;
  }

  // template<typename Duration>
  // bool operator< (Duration other) {
  //   return this->d < other;
  // }

  template<typename Duration>
  TimeInterval &operator+= (Duration other) {
    this->d += other;
    return *this;
  }
};

struct BenchmarkResult {
  int          N; // The number of iterations.
  TimeInterval T; // The total time taken.
};

struct Benchmark;
typedef void (*Function)(Benchmark &);

struct Benchmark {
  std::string     Name;     // the name of the function to be benchmarked
  Function        F;        // the function to be benchmarked
  int             N;        // iter count for benchmarked function
  bool            failed;
  TimeInterval    duration;
  TimeInterval    previousDuration;
  int             previousN;
  BenchmarkResult result;

  bool            timerOn;
  TimePoint       start;    // time benchmark started

  Benchmark(std::string name, Function f)
    : Name(name), F(f), N(0), failed(false), duration(0), timerOn(false) { }

  int64_t nsPerOp() {
    return N <= 0 ? 0 : duration.Nanoseconds() / int64_t(N);
  }

  static TimePoint Now() {
    return std::chrono::high_resolution_clock::now();
  }

  // runN runs a single benchmark for the specified number of iterations.
  void runN(int n) {
    this->N = n;
    this->ResetTimer();
    this->StartTimer();
    this->F(*this);
    this->StopTimer();
    this->previousN = n;
    this->previousDuration = this->duration;
  }

  // StartTimer starts timing a test.  This function is called automatically
  // before a benchmark starts, but it can also used to resume timing after
  // a call to StopTimer.
  void StartTimer() {
    if (!this->timerOn) {
      this->start = std::chrono::high_resolution_clock::now();
      this->timerOn = true;
    }
  }

  // StopTimer stops timing a test.  This can be used to pause the timer
  // while performing complex initialization that you don't
  // want to measure.
  void StopTimer() {
    if (this->timerOn) {
      this->duration += std::chrono::high_resolution_clock::now() - this->start;
      this->timerOn = false;
    }
  }

  // ResetTimer zeros the elapsed benchmark time.
  // It does not affect whether the timer is running.
  void ResetTimer() {
    if (this->timerOn) {
      this->start = Now();
    }
    this->duration = TimeInterval();
  }

  // round_down_10 rounds a number down to the nearest power of 10.
  static int round_down_10(int n) {
    int tens = 0;
    // tens = floor(log_10(n))
    while (n >= 10) {
      n = n / 10;
      ++tens;
    }
    // result = 10^tens
    int result = 1;
    for (int i = 0; i < tens; ++i) {
      result *= 10;
    }
    return result;
  }

  // round_up rounds x up to a number of the form [1eX, 2eX, 5eX].
  static int round_up(int n) {
    int base = round_down_10(n);
    return n <=     base ?      base
         : n <= 2 * base ?  2 * base
         : n <= 5 * base ?  5 * base
         :                 10 * base
         ;
  }


  // launch launches the benchmark function.  It gradually increases the number
  // of benchmark iterations until the benchmark runs for a second in order
  // to get a reasonable measurement.  It prints timing information in this form
  //		testing.BenchmarkHello	100000		19 ns/op
  // launch is run by the fun function as a separate goroutine.
  void run() {
    // Run the benchmark for a single iteration in case it's expensive.
    int n = 1;

    // The benchmarked function should run at least this long.
    TimeInterval benchTime(std::chrono::seconds(1));

    this->runN(n);
    // Run the benchmark for at least the specified amount of time.
    auto d = benchTime;
    while (!this->failed && this->duration < d && n < 1e9) {
      int last = n;

      // Predict iterations/sec.
      if (this->nsPerOp() == 0) {
        n = 1e9;
      } else {
        n = int(d.Nanoseconds() / this->nsPerOp());
      }

      // Run more iterations than we think we'll need for a second (1.5x).
      // Don't grow too fast in case we had timing errors previously.
      // Be sure to run at least one more than last time.
      n = std::max(std::min(n+n/2, 100*last), last+1);

      // Round up to something easy to read.
      n = round_up(n);
      this->runN(n);
    }
    this->result = BenchmarkResult{this->N, this->duration};
    std::cout << this->Name << "\t" << this->N << "\t" << this->nsPerOp()
              << " ns/op" << std::endl;
  }
};

#define BENCHMARK(f) (Benchmark(#f, f).run())

#endif /* __BENCHMARK_H__ */
