Thom Boyer's C++ Hex Board Game Exploratorium
=============================================

https://github.com/perlmonger42/hex-cpp

Overview
--------

This project contains libraries and programs for studying the game
of Hex.
For an introduction to the game, see the
[wikipedia article](http://en.wikipedia.org/wiki/Hex_%28board_game%29).

Included in this project are these components:

- Three different bitset implementations, for representing subsets of {0..63},
  {0..127}, and {0..191} respectively. These are called quadsets, because they
  use 1, 2, or 3 quadword (uint64\_t) data members.
- The `cellset` class, which wraps a quadset with Hex-specific methods to
  represent sets of Hex cells.
- The `Board` class, which represents a game configuration.
  
  
You might wonder why the need for bitset implementations, given that the C++
standard library provides `std::bitset<N>`. The reason these implementations
exist is that hardcoding the maximum set size makes for a much faster
implementation which also more inlineable. I had thought that it might not make
much difference, but benchmarks show that it does.

Consider the particular case of `winner_benchmark`, a benchmark program
included in this repository.  It checks for a winner on 25 different board
positions. It is able to do this in about 1375 nanoseconds. A variant of the
program that uses std::bitset instead of the handcrafted bitset implementations
requires more than two seconds to do the same job (that's 2,000,000+
nanoseconds). You can see the comparison yourself by checking out the branch
`explore/std-bitset`. The shift operators provided by std::bitset are probably
the cause of most of that dramatic speed difference.
  

Requirements for Building
-------------------------

This project is my own playground, so I have't put much effort into making
it portable.

Requirements:
* [cmake](http://www.cmake.org/) 2.8.11 or newer.
* A C++14 compiler.
* [Google Test](https://code.google.com/p/googletest/) 1.7.0.

The build system uses [cmake](http://www.cmake.org/), and could theoretically
work on any OS. There's a good chance it will work on Linux, though I haven't
tested it there. Getting it to work on Windows is likely to be more difficult.


Getting the Source
------------------

You can get a copy of the code by cloning the github repository:

   git clone https://github.com/perlmonger42/hex-cpp.git


Setting up the Build
--------------------

The project uses Google Test. You will need to get a copy of the gtest sources
([gtest-1.7.0.zip](https://googletest.googlecode.com/files/gtest-1.7.0.zip)) and
place a copy of (or a symlink to) its gtest-1.7.0 folder into the root of this
project. The copy or symlink must have the name `gtest-1.7.0`, or else you must
edit `./CMakeLists.txt` to reflect the name change.

The `build.sh` script kicks off a cmake out-of-source build in the folder
`/tmp/hex-build`. This will certainly _not_ work on Windows, but the script
should be easily adaptable.

Good luck!
