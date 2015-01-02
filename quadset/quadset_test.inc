/* vim: set ft=cpp tw=75: */

// TEST has two parameters: the test case name and the test name.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.

TEST(SET, StaticValues) {
  SET s;

  // sizeof (make sure quadset<BITS> uses the correct qset specialization)
  EXPECT_EQ(((SIZE+63)/64) * 8, sizeof(s));

  // size()
  EXPECT_EQ(s.size(), SIZE);

  // universe()
  std::stringstream stream;
  stream << "{0.." << s.size()-1 << '}';
  auto want = stream.str();
  EXPECT_EQ(want, SET::universe().to_string());
}

TEST(SET, to_string) {
  SET s;
  s.reset();
  EXPECT_EQ("{}", s.to_string());

  s.set(0);
  EXPECT_EQ("{0}", s.to_string());

  s.set(2);
  EXPECT_EQ("{0, 2}", s.to_string());

  s.set(3);
  EXPECT_EQ("{0, 2, 3}", s.to_string());

  s.set(5);
  EXPECT_EQ("{0, 2, 3, 5}", s.to_string());

  s.set(7);
  EXPECT_EQ("{0, 2, 3, 5, 7}", s.to_string());

  s.set(9);
  EXPECT_EQ("{0, 2, 3, 5, 7, 9}", s.to_string());

  s.set(6);
  EXPECT_EQ("{0, 2, 3, 5..7, 9}", s.to_string());

  s.set(8);
  EXPECT_EQ("{0, 2, 3, 5..9}", s.to_string());

  s.set(3, 8);
  EXPECT_EQ("{0, 2..9}", s.to_string());

  s.set(1);
  EXPECT_EQ("{0..9}", s.to_string());

  s.reset();
  s.set(s.size()-10, s.size()-5);
  s.set(s.size()-1);
  std::stringstream stream;
  stream << '{' << s.size()-10 << ".." << s.size()-6 << ", "
         << s.size()-1 << '}';
  auto want = stream.str();
  EXPECT_EQ(want, s.to_string());
};

TEST(SET, setAndTest) {
  auto s = SET::make();
  EXPECT_EQ("{}", s.to_string());

  s = SET::make();
  s.set(10);
  EXPECT_EQ("{10}", s.to_string());

  s = SET::make();
  s.set(4);
  s.set(14);
  s.set(15);
  EXPECT_EQ("{4, 14, 15}", s.to_string());

  s = SET::make();
  s.set(1);
  s.set(2);
  s.set(3);
  s.set(11);
  s.set(12);
  s.set(13);
  EXPECT_EQ("{1..3, 11..13}", s.to_string());

  s = SET::make();
  s.set(s.size()-1);
  std::stringstream stream;
  stream << "{" << s.size()-1 << '}';
  auto want = stream.str();
  EXPECT_EQ(want, s.to_string());

  s.set();
  EXPECT_EQ(s, SET::universe());

  s = SET::make();
  s.set(27, SIZE);
  // stream.str("");
  // stream.clear();
  std::stringstream stream2;
  stream2 << "{27.." << s.size()-1 << '}';
  want = stream2.str();
  EXPECT_EQ(want, s.to_string());

  s = SET::make();
  s.set(12, 20);
  EXPECT_EQ("{12..19}", s.to_string());

  s = SET::make();
  s.set({});
  EXPECT_EQ("{}", s.to_string());

  s = SET::make();
  s.set({10, 9, 8, 1, 5});
  EXPECT_EQ("{1, 5, 8..10}", s.to_string());

  
  // Repeat N times:
  //   Set M to a value in 0..s.size()/4.
  //   Add M random elements to an empty set.
  //   Add the same M random elements to a bitset.
  //   Check that the quadset and the bitset have the same elements.
  std::minstd_rand generator;
  std::uniform_int_distribution<uint32_t> distM(0, s.size()/4);
  std::uniform_int_distribution<uint32_t> distS(0, s.size()-1);
  auto randM = std::bind(distM, generator);
  auto randS = std::bind(distS, generator);
  const int N = 10000;
  for (int i = 0; i < N; ++i) {
    int M = randM();
    auto quad = SET::make();
    auto bits = std::bitset<quad.size()>();
    for (int j = 0; j < M; ++j) {
      int k = randS();
      bits.set(k);
      quad.set(k);
    }
    expect_eq_set<SIZE>("set", bits, quad);
    for (int j = 0; j < quad.size(); ++j) {
      EXPECT_EQ(bits.test(j), quad.test(j));
    }
  }
};

TEST(SET, setBuilders) {
  // static initializer
  SET s = SET{15};
  EXPECT_EQ("{0..3}", s.to_string());

  // from_ullong()
  s = SET::from_ullong(0x8FA5);
  EXPECT_EQ("{0, 2, 5, 7..11, 15}", s.to_string());

  s = SET::from_ullong(0x40);
  EXPECT_EQ("{6}", s.to_string());

  // make()
  EXPECT_EQ("{}", SET::make().to_string());
  EXPECT_EQ("{0}", SET::make(0).to_string());
  EXPECT_EQ("{0, 2}", SET::make(0, 2).to_string());
  EXPECT_EQ("{0, 2, 3}", SET::make(0, 2, 3).to_string());
  EXPECT_EQ("{0, 2, 3, 5}", SET::make(0, 2, 3, 5).to_string());
  EXPECT_EQ("{0, 2, 3, 5, 7}", SET::make(0, 2, 3, 5, 7).to_string());
  EXPECT_EQ("{0, 2, 3, 5, 7, 9}", SET::make(0, 2, 3, 5, 7, 9).to_string());
  EXPECT_EQ("{0, 2, 3, 5..7, 9}", SET::make(0, 2, 3, 5, 7, 9, 6).to_string());
  EXPECT_EQ("{0, 2, 3, 5..9}", SET::make(0, 2, 3, 5, 7, 9, 6, 8).to_string());
  EXPECT_EQ("{0, 2..9}", SET::make(0, 2, 3, 4, 5, 6, 7, 8, 9).to_string());
  EXPECT_EQ("{0..6}", SET::make(0, 1, 2, 3, 4, 5, 6).to_string());

  std::stringstream stream;
  stream << "{0, " << SIZE-1 << '}';
  auto want = stream.str();
  EXPECT_EQ(want, SET::make(0, SIZE-1).to_string());
  s = SET::make();
  EXPECT_EQ("{}", s.to_string());

  s = SET::make(15);
  EXPECT_EQ("{15}", s.to_string());

  s = SET::make(2, 4, 8, 10);
  EXPECT_EQ("{2, 4, 8, 10}", s.to_string());

  s = SET::make(2, 4, 8, 10);
  EXPECT_EQ("{2, 4, 8, 10}", s.to_string());

  s = SET::make({});
  EXPECT_EQ("{}", s.to_string());

  s = SET::make({1, 3, 5, 7, 9});
  EXPECT_EQ("{1, 3, 5, 7, 9}", s.to_string());

  // range()
  s = SET::range(12, 12);
  EXPECT_EQ("{}", s.to_string());

  s = SET::range(5, 6);
  EXPECT_EQ("{5}", s.to_string());

  s = SET::range(10, s.size()-8);
  std::stringstream stream2;
  stream2 << "{10.." << SIZE-9 << '}';
  want = stream2.str();
  EXPECT_EQ(want, s.to_string());

  s = SET::range(5, 10);
  EXPECT_EQ("{5..9}", s.to_string());

  s = SET::range(2, 41);
  EXPECT_EQ("{2..40}", s.to_string());

  std::stringstream stream3;
  stream3 << "{7.." << s.size()-5 << '}';
  want = stream3.str();
  EXPECT_EQ(want, SET::range(7, s.size()-4).to_string());

  
  std::minstd_rand generator;
  std::uniform_int_distribution<uint32_t> distribution(0, s.size()-1);
  auto rand = std::bind(distribution, generator);
  const int N = 100;
  // Repeat N times:
  //   Set rLo, rHi to a random range in 0..s.size().
  //   Set bits[i]=1 for each in i rLo..rHi-1.
  //   Check that SET::range(rLo, rHi).test(i) matches bits[i], for all i.
  for (int i = 0; i < N; ++i) {
    std::bitset<SIZE> bits;
    bitpos rLo = rand();
    std::uniform_int_distribution<uint32_t> distribution2(rLo, SIZE);
    bitpos rHi = distribution2(generator);
    for (int j = rLo; j < rHi; ++j) {
      bits.set(j);
    }
    expect_eq_set<SIZE>("range", bits, SET::range(rLo, rHi));
  }
}

TEST(SET, reset) {
  auto s = SET::range(0, 10);
  s.reset(5);
  EXPECT_EQ("{0..4, 6..9}", s.to_string());

  s = SET::make(10,20,30);
  s.reset(10);
  EXPECT_EQ("{20, 30}", s.to_string());
  s.reset(15);
  EXPECT_EQ("{20, 30}", s.to_string());
  s.reset(30);
  EXPECT_EQ("{20}", s.to_string());
  s.reset(45);
  EXPECT_EQ("{20}", s.to_string());
  s.reset(20);
  EXPECT_EQ("{}", s.to_string());


  s = SET::range(10, 20);
  s.reset(13, 18);
  EXPECT_EQ("{10..12, 18, 19}", s.to_string());
  s.reset(11, 11);
  EXPECT_EQ("{10..12, 18, 19}", s.to_string());

  s.reset({});
  EXPECT_EQ("{10..12, 18, 19}", s.to_string());
  s.reset({11, 18, SIZE-1});
  EXPECT_EQ("{10, 12, 19}", s.to_string());

  
  // Repeat N times:
  //   Set M to a value in 0..s.size()/4.
  //   Remove M random elements from a full set.
  //   Remove the same M random elements from a full bitset.
  //   Check that the quadset and the bitset have the same elements.
  std::minstd_rand generator;
  std::uniform_int_distribution<uint32_t> distM(0, s.size()/4);
  std::uniform_int_distribution<uint32_t> distS(0, s.size()-1);
  auto randM = std::bind(distM, generator);
  auto randS = std::bind(distS, generator);
  const int N = 10000;
  for (int i = 0; i < N; ++i) {
    int M = randM();
    auto quad = SET::make();
    auto bits = std::bitset<quad.size()>();
    bits.set();
    quad.set();
    for (int j = 0; j < M; ++j) {
      int k = randS();
      bits.reset(k);
      quad.reset(k);
    }
    expect_eq_set<SIZE>("reset", bits, quad);
  }
};

TEST(SET, flip) {
  auto s = SET::range(0, 10);
  s.flip(5);
  EXPECT_EQ("{0..4, 6..9}", s.to_string());

  s = SET::make(10,20,30);
  s.flip(10);
  EXPECT_EQ("{20, 30}", s.to_string());
  s.flip(15);
  EXPECT_EQ("{15, 20, 30}", s.to_string());
  s.flip(30);
  EXPECT_EQ("{15, 20}", s.to_string());
  s.flip(45);
  EXPECT_EQ("{15, 20, 45}", s.to_string());
  s.flip(20);
  EXPECT_EQ("{15, 45}", s.to_string());


  s = SET::range(10, 20);
  s.flip(13, 18);
  EXPECT_EQ("{10..12, 18, 19}", s.to_string());
  s.flip(11, 11);
  EXPECT_EQ("{10..12, 18, 19}", s.to_string());
  s.flip(8, 14);
  EXPECT_EQ("{8, 9, 13, 18, 19}", s.to_string());

  s.flip({});
  EXPECT_EQ("{8, 9, 13, 18, 19}", s.to_string());
  s.flip({10, 18, SIZE-1});
  std::stringstream stream;
  stream << "{8..10, 13, 19, " << s.size()-1 << '}';
  auto want = stream.str();
  EXPECT_EQ(want, s.to_string());

  
  // Repeat N times:
  //   Set M to a value in 0..s.size()/4.
  //   Flip M random elements from a full set.
  //   Flip the same M random elements from a full bitset.
  //   Check that the quadset and the bitset have the same elements.
  std::minstd_rand generator;
  std::uniform_int_distribution<uint32_t> distM(0, s.size()/4);
  std::uniform_int_distribution<uint32_t> distS(0, s.size()-1);
  auto randM = std::bind(distM, generator);
  auto randS = std::bind(distS, generator);
  const int N = 10000;
  for (int i = 0; i < N; ++i) {
    int M = randM();
    auto quad = SET::make();
    auto bits = std::bitset<quad.size()>();
    bits.set();
    quad.set();
    for (int j = 0; j < M; ++j) {
      int k = randS();
      bits.flip(k);
      quad.flip(k);
    }
    expect_eq_set<SIZE>("flip", bits, quad);
  }
};

TEST(SET, any) {
  EXPECT_EQ(false, SET::make().any());
  EXPECT_EQ(true, SET::make(0).any());
  EXPECT_EQ(true, SET::make(1).any());
  EXPECT_EQ(true, SET::make(19).any());
  EXPECT_EQ(true, SET::make(SIZE-1).any());

  
  // Repeat N times:
  //   Set M to an integer in the range 0..3.
  //   Add M random elements to the set.
  //   Check that SET::any() == (M != 0)
  std::minstd_rand generator;
  std::uniform_int_distribution<uint32_t> distM(0, 3);
  std::uniform_int_distribution<uint32_t> distS(0, SIZE-1);
  auto randM = std::bind(distM, generator);
  auto randS = std::bind(distS, generator);
  const int N = 100;
  for (int i = 0; i < N; ++i) {
    int M = randM();
    auto s = SET::make();
    for (int j = 0; j < M; ++j) {
      s.set(randS());
    }
    EXPECT_EQ(M != 0, s.any());
  }
};

TEST(SET, none) {
  EXPECT_EQ(true,  SET::make().none());
  EXPECT_EQ(false, SET::make(0).none());
  EXPECT_EQ(false, SET::make(1).none());
  EXPECT_EQ(false, SET::make(19).none());
  EXPECT_EQ(false, SET::make(SIZE-1).none());

  // Repeat N times:
  //   Set M to an integer in the range 0..3.
  //   Add M random elements to the set.
  //   Check that SET::none() == (M != 0)
  std::minstd_rand generator;
  std::uniform_int_distribution<uint32_t> distM(0, 3);
  std::uniform_int_distribution<uint32_t> distS(0, SIZE-1);
  auto randM = std::bind(distM, generator);
  auto randS = std::bind(distS, generator);
  const int N = 100;
  for (int i = 0; i < N; ++i) {
    int M = randM();
    auto s = SET::make();
    for (int j = 0; j < M; ++j) {
      s.set(randS());
    }
    EXPECT_EQ(M == 0, s.none());
  }
};

TEST(SET, all) {
  EXPECT_EQ(false, SET::make().all());
  EXPECT_EQ(false, SET::make(0).all());
  EXPECT_EQ(false, SET::make(1).all());
  EXPECT_EQ(false, SET::make(19).all());
  EXPECT_EQ(true,  SET::range(0, SIZE).all());

  // Repeat N times:
  //   Create a set with all elements included.
  //   Set M to an integer in the range 0..3.
  //   Remove M random elements from the set.
  //   Check that SET::all() == (M == 0)
  std::minstd_rand generator;
  std::uniform_int_distribution<uint32_t> distM(0, 3);
  std::uniform_int_distribution<uint32_t> distS(0, SIZE-1);
  auto randM = std::bind(distM, generator);
  auto randS = std::bind(distS, generator);
  const int N = 1000;
  for (int i = 0; i < N; ++i) {
    int M = randM();
    auto s = SET::range(0, SIZE);
    for (int j = 0; j < M; ++j) {
      s.reset(randS());
    }
    EXPECT_EQ(M == 0, s.all());
  }

};

TEST(SET, count) {
  EXPECT_EQ(0, SET::make().count());
  EXPECT_EQ(1, SET::make(0).count());
  EXPECT_EQ(1, SET::make(1).count());
  EXPECT_EQ(2, SET::make(19, 20).count());
  EXPECT_EQ(SIZE, SET::range(0, SIZE).count());

  // Repeat N times:
  //   Set M to an integer in the range 0 .. 5/4 * SIZE.
  //   Create an empty quadset and an empty bitset.
  //   Generate M random elements and add them to both sets.
  //   Check that the set counts agree.
  std::minstd_rand generator;
  std::uniform_int_distribution<uint32_t> distM(0, SIZE * 5 / 4);
  std::uniform_int_distribution<uint32_t> distS(0, SIZE-1);
  auto randM = std::bind(distM, generator);
  auto randS = std::bind(distS, generator);
  const int N = 1000;
  for (int i = 0; i < N; ++i) {
    int M = randM();
    auto quad = SET::make();
    auto bits = std::bitset<SIZE>();
    for (int j = 0; j < M; ++j) {
      int k = randS();
      quad.set(k);
      bits.set(k);
    }
    EXPECT_EQ(bits.count(), quad.count());
  }

  auto samples = getTestSets<SIZE>();
  for (int i = 0; i < samples->quadsets.size(); ++i) {
    auto a = samples->quadsets.at(i);
    EXPECT_EQ(loopCountBits(a), a.count());
  }
};

TEST(SET, EqNe) {
  EXPECT_EQ(true, SET::make()==SET::make());
  EXPECT_EQ(true, SET::make(7)==SET::make(7));
  EXPECT_EQ(false, SET::make()==SET::make(1));
  EXPECT_EQ(false, SET::make(20)==SET::make(20, 21));

  auto samples = getTestSets<SIZE>();
  for (int i = 0; i < samples->quadsets.size(); ++i) {
    auto &a = samples->quadsets.at(i);
    for (int j = 0; j < samples->quadsets.size(); ++j) {
      auto &b = samples->quadsets.at(j);
      EXPECT_EQ(i==j, a==b);
      EXPECT_EQ(i!=j, a!=b);
    }
  }
};

TEST(SET, ToUllong) {
  EXPECT_EQ(0UL, SET::make().to_ullong());
  EXPECT_EQ(128UL, SET::make(7).to_ullong());
  EXPECT_EQ(2UL, SET::make(1).to_ullong());
  EXPECT_EQ(3UL << 20, SET::make(20, 21).to_ullong());
};

TEST(SET, Shifts) {
  SET s;
  s = SET::make(1, 3, 4, 10);
  EXPECT_EQ("{1, 3, 4, 10}", (s<<0).to_string());
  EXPECT_EQ("{2, 4, 5, 11}", (s<<1).to_string());
  EXPECT_EQ("{4, 6, 7, 13}", (s<<3).to_string());
  EXPECT_EQ("{34, 36, 37, 43}", (s<<33).to_string());
  EXPECT_EQ("{}", (s<<SIZE).to_string());
  EXPECT_EQ("{}", (s<<(SIZE+10)).to_string());

  s <<= 0;
  EXPECT_EQ("{1, 3, 4, 10}", s.to_string());
  s <<= 1;
  EXPECT_EQ("{2, 4, 5, 11}", s.to_string());
  s <<= 2;
  EXPECT_EQ("{4, 6, 7, 13}", s.to_string());
  s <<= 30;
  EXPECT_EQ("{34, 36, 37, 43}", s.to_string());
  s <<= SIZE;
  EXPECT_EQ("{}", s.to_string());


  s = SET::make(34, 36, 37, 43);
  EXPECT_EQ("{34, 36, 37, 43}", (s>>0).to_string());
  EXPECT_EQ("{33, 35, 36, 42}", (s>>1).to_string());
  EXPECT_EQ("{31, 33, 34, 40}", (s>>3).to_string());
  EXPECT_EQ("{14, 16, 17, 23}", (s>>20).to_string());
  EXPECT_EQ("{}", (s>>SIZE).to_string());
  EXPECT_EQ("{}", (s>>(SIZE+10)).to_string());

  s >>= 0;
  EXPECT_EQ("{34, 36, 37, 43}", s.to_string());
  s >>= 1;                                   
  EXPECT_EQ("{33, 35, 36, 42}", s.to_string());
  s >>= 4;                                   
  EXPECT_EQ("{29, 31, 32, 38}", s.to_string());
  s >>= 15;                                  
  EXPECT_EQ("{14, 16, 17, 23}", s.to_string());
  s >>= SIZE;                                  
  EXPECT_EQ("{}", s.to_string());


  std::minstd_rand generator;
  std::uniform_int_distribution<uint32_t> distS(0, SIZE);
  auto randS = std::bind(distS, generator);

  for (auto set : getTestSets<SIZE>()->quadsets) {
    bitpos distance = 0;
    while (distance < SIZE) {
      distance += randS();

      testShift("LeftShift", set << distance, set, distance, ACCURATE);
      auto leftcopy = set;
      leftcopy <<= distance;
      testShift("LeftShiftAssign", leftcopy, set, distance, ACCURATE);

      testShift("RightShift", set >> distance, set, -distance, ACCURATE);
      auto rightcopy = set;
      rightcopy >>= distance;
      testShift("RightShiftAssign", rightcopy, set, -distance, ACCURATE);

      testShift("FastLeftShift", set.fast_lsh(distance), set, distance, FAST);
      leftcopy = set;
      leftcopy.fast_lsh_assign(distance);
      testShift("FastLeftShiftAssign", leftcopy, set, distance, FAST);

      //distance += 20;
    }
  }
};

TEST(SET, BoolOps) {
  SET s = SET::make(0, 1);
  SET t = SET::make(1, 2);
  SET u = SET::range(50, SIZE);

  EXPECT_EQ("{1}", (s & t).to_string());
  EXPECT_EQ("{0..2}", (s | t).to_string());
  EXPECT_EQ("{0, 2}", (s ^ t).to_string());
  EXPECT_EQ("{0}", (s - t).to_string());
  EXPECT_EQ("{0..49}", (~u).to_string());

  SET v = s;
  v &= t;
  EXPECT_EQ("{1}", v.to_string());
  v = s;
  v |= t;
  EXPECT_EQ("{0..2}", v.to_string());
  v = s;
  v ^= t;
  EXPECT_EQ("{0, 2}", v.to_string());
  v = s;
  v -= t;
  EXPECT_EQ("{0}", v.to_string());
  
  // Test that ({MAX}<<1)>>1 == {}
  v.reset();
  v.set(SIZE-1);
  EXPECT_EQ("{}", ((v<<1) >> 1).to_string());
  v.reset();
  v.set(SIZE-1);
  EXPECT_EQ("{}", (v.fast_lsh(1).clean() >> 1).to_string());

  v.reset();
  v.set(SIZE-1);
  v <<= 1;
  v >>= 1;
  EXPECT_EQ("{}", v.to_string());
  v.reset();
  v.set(SIZE-1);
  v.fast_lsh_assign(1);
  v = v.clean();
  v >>= 1;
  EXPECT_EQ("{}", v.to_string());

  // Test that ~s leaves unused bits in the zero state.
  s = ~SET::make(0, SIZE-1);
  t = SET::range(1, SIZE-1);
  EXPECT_EQ(s, t);
  s = SET::make(0, SIZE-1).fast_not().clean();
  EXPECT_EQ(s, t);

  // Test that (~set)>>1 doesn't introduce any new elements.
  EXPECT_EQ(~SET::range(0, SIZE) >> 1, SET::make());
  EXPECT_EQ(SET::range(0, SIZE).fast_not().clean() >> 1, SET::make());
  
  auto samples = getTestSets<SIZE>();
  for (int i = 0; i < samples->quadsets.size(); ++i) {
    auto q1 = samples->quadsets.at(i);
    auto b1 = samples->bitsets.at(i);
    auto quad = q1.fast_not();
    auto bits = ~b1;
    expect_op_result<SIZE>("fast_not", bits, q1, q1, quad, FAST);

    quad = ~q1;
    expect_op_result<SIZE>("~", bits, q1, q1, quad);

    for (int j = 0; j < samples->quadsets.size(); ++j) {
      auto q2 = samples->quadsets.at(j);
      auto b2 = samples->bitsets.at(j);

      quad = q1 & q2;
      bits = b1 & b2;
      expect_op_result<SIZE>("&", bits, q1, q2, quad);

      quad = q1 | q2;
      bits = b1 | b2;
      expect_op_result<SIZE>("|", bits, q1, q2, quad);

      quad = q1 ^ q2;
      bits = b1 ^ b2;
      expect_op_result<SIZE>("^", bits, q1, q2, quad);

      quad = q1 - q2;
      bits = b1 & ~b2;
      expect_op_result<SIZE>("-", bits, q1, q2, quad);

      quad = q1;
      bits = b1;
      quad &= q2;
      bits &= b2;
      expect_op_result<SIZE>("&", bits, q1, q2, quad);

      quad = q1;
      bits = b1;
      quad |= q2;
      bits |= b2;
      expect_op_result<SIZE>("|=", bits, q1, q2, quad);

      quad = q1;
      bits = b1;
      quad ^= q2;
      bits ^= b2;
      expect_op_result<SIZE>("^=", bits, q1, q2, quad);

      quad = q1;
      bits = b1;
      quad -= q2;
      bits &= ~b2;
      expect_op_result<SIZE>("-=", bits, q1, q2, quad);
    }
  }
};
