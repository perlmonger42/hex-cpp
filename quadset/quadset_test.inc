/* vim: set ft=cpp tw=75: */

// TEST has two parameters: the test case name and the test name.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.

TEST(SET, statics) {
  SET s;

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
    auto qset = SET::make();
    auto bset = std::bitset<qset.size()>();
    for (int j = 0; j < M; ++j) {
      int k = randS();
      bset.set(k);
      qset.set(k);
    }
    expect_eq_set<SIZE>("set", bset, qset);
    for (int j = 0; j < qset.size(); ++j) {
      EXPECT_EQ(bset.test(j), qset.test(j));
    }
  }
};

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
    auto qset = SET::make();
    auto bset = std::bitset<qset.size()>();
    bset.set();
    qset.set();
    for (int j = 0; j < M; ++j) {
      int k = randS();
      bset.reset(k);
      qset.reset(k);
    }
    expect_eq_set<SIZE>("reset", bset, qset);
  }
};

TEST(SET, make) {
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
};

TEST(SET, range) {
  auto s = SET::range(5, 10);
  EXPECT_EQ("{5..9}", s.to_string());

  s = SET::range(2, 41);
  EXPECT_EQ("{2..40}", s.to_string());

  std::stringstream stream;
  stream << "{7.." << s.size()-5 << '}';
  auto want = stream.str();
  EXPECT_EQ(want, SET::range(7, s.size()-4).to_string());

  
  std::minstd_rand generator;
  std::uniform_int_distribution<uint32_t> distribution(0, s.size()-1);
  auto rand = std::bind(distribution, generator);
  const int N = 100;
  // Repeat N times:
  //   Set rLo, rHi to a random range in 0..s.size().
  //   Set bset[i]=1 for each in i rLo..rHi-1.
  //   Check that SET::range(rLo, rHi).test(i) matches bset[i], for all i.
  for (int i = 0; i < N; ++i) {
    std::bitset<SIZE> bset;
    bitpos rLo = rand();
    std::uniform_int_distribution<uint32_t> distribution2(rLo, SIZE);
    bitpos rHi = distribution2(generator);
    for (int j = rLo; j < rHi; ++j) {
      bset.set(j);
    }
    expect_eq_set<SIZE>("range", bset, SET::range(rLo, rHi));
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
    auto qset = SET::make();
    auto bset = std::bitset<SIZE>();
    for (int j = 0; j < M; ++j) {
      int k = randS();
      qset.set(k);
      bset.set(k);
    }
    EXPECT_EQ(bset.count(), qset.count());
  }

  auto samples = getTestSets<SIZE>();
  for (int i = 0; i < samples->size(); ++i) {
    auto a = samples->at(i);
    EXPECT_EQ(loopCountBits(a), a.count());
  }
};

TEST(SET, EqNe) {
  EXPECT_EQ(true, SET::make()==SET::make());
  EXPECT_EQ(true, SET::make(7)==SET::make(7));
  EXPECT_EQ(false, SET::make()==SET::make(1));
  EXPECT_EQ(false, SET::make(20)==SET::make(20, 21));

  std::vector< qset<SIZE> > *samples = getTestSets<SIZE>();
  for (int i = 0; i < samples->size(); ++i) {
    qset<SIZE> &a = samples->at(i);
    for (int j = 0; j < samples->size(); ++j) {
      qset<SIZE> &b = samples->at(j);
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

  for (auto set : *getTestSets<SIZE>()) {
    bitpos distance = 0;
    while (distance < SIZE) {
      distance += randS();

      testShift("LeftShift", set << distance, set, distance);
      auto leftcopy = set;
      leftcopy <<= distance;
      testShift("LeftShiftAssign", leftcopy, set, distance);

      testShift("RightShift", set >> distance, set, -distance);
      auto rightcopy = set;
      rightcopy >>= distance;
      testShift("RightShiftAssign", rightcopy, set, -distance);

      //distance += 20;
    }
  }
};
