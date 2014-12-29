#include <iostream>
#include <stdint.h>
#include <initializer_list>
#include "../quadset/quadset.h"


template<bitpos S, bitpos BITS = S*S, bitpos QUADWORDS = (BITS+63) / 64>
class CellSet : public quadset<S*S> {
public:
  typedef CellSet<S> cellSet;
  typedef quadset<BITS> quadSet;
  //static constexpr bitpos size = S; // Edge length. A board is size×size cells.
  //static constexpr bitpos max = S*S-1; // Cell numbers run from 0 to max.
  //static constexpr bitpos count = S*S; // The number of cells on an N×N board.


  constexpr CellSet() { quadSet::reset(); }
  constexpr CellSet(uint64_t val) : quadSet{val} { }
  constexpr CellSet(quadSet val) : quadSet(val) { }
  // constexpr CellSet() : quadSet() { }
  // constexpr CellSet(quadSet const &other) : quadSet(other) { }
  // constexpr CellSet(CellSet const &other) : quadSet(other) { }
  // constexpr CellSet(unsigned long long val) : quadSet{val} { }
  
  static constexpr cellSet make() {
    return cellSet{ quadSet::make() };
  }

  template<typename... Args>
  static constexpr cellSet make(bitpos p, Args... args) {
    return cellSet{ quadSet::make(p, args...) };
  }

  static cellSet make(std::initializer_list<bitpos> list) {
    return cellSet{ quadSet::make(list) };
  }

  static constexpr cellSet range(bitpos m, bitpos n) {
    return cellSet{ quadSet::range(m, n) };
  }

  inline constexpr uint64_t to_ullong() const {
    return this->quadSet::to_ullong();
  }

  inline constexpr cellSet operator<<(bitpos n) const {
    return cellSet{ this->quadSet::operator<<(n) };
  }

  inline constexpr cellSet operator>>(bitpos n) const {
    return cellSet{ this->quadSet::operator>>(n) };
  }

  inline constexpr cellSet operator& (cellSet other) const {
    return cellSet{ this->quadSet::operator&(other) };
  }

  inline constexpr cellSet operator| (cellSet other) const {
    return cellSet{ this->quadSet::operator|(other) };
  }

  inline constexpr cellSet operator~ () const {
    return cellSet{ this->quadSet::operator~() };
  }

  inline constexpr cellSet operator- (cellSet other) const {
    return cellSet{ this->quadSet::operator-(other) };
  }

  inline cellSet& operator|= (cellSet other) {
    this->quadSet::operator|=(other);
    return *this;
  }

  inline cellSet& operator&= (cellSet other) {
    this->quadSet::operator&=(other);
    return *this;
  }

  inline cellSet& operator-= (cellSet other) {
    this->quadSet::operator-=(other);
    return *this;
  }

  inline cellSet& set() {
    this->quadSet::set();
    return *this;
  }

  inline cellSet& set(bitpos n) {
    this->quadSet::set(n);
    return *this;
  }

  inline cellSet& clear() {
    this->quadSet::reset();
    return *this;
  }

  inline cellSet& clear(bitpos n) {
    this->quadSet::reset(n);
    return *this;
  }

  inline cellSet& reset() {
    this->quadSet::reset();
    return *this;
  }

  inline cellSet& reset(bitpos n) {
    this->quadSet::reset(n);
    return *this;
  }

  inline cellSet& flip() {
    this->quadSet::flip();
    return *this;
  }

  inline cellSet& flip(bitpos n) {
    this->quadSet::flip(n);
    return *this;
  }

  static inline constexpr cellSet universe() {
    return cellSet{ quadSet::universe() };
  }

  static inline constexpr cellSet top() {
    return cellSet{ uint64_t(-1LL) >> (64-S) };
  }

  static inline constexpr cellSet bottom() {
    return top() << (BITS - S);
  }

  static inline constexpr cellSet left() {
    return cellSet{ quadSet::repeat_block(quadSet{1}, S, S) };
  }

  static inline constexpr cellSet right() {
    return cellSet{ quadSet::repeat_block(quadSet{1<<(S-1)}, S, S) };
  }

  inline constexpr cellSet neighbors() const {
    constexpr auto l = ~left();
    constexpr auto r = ~right();
    auto s = *this;
    return ( (s >> S)
           | (((s >> (S - 1)) | (s << 1)) & l)
           | (((s << (S - 1)) | (s >> 1)) & r)
           | (s << S)
           );
  }

  std::ostream& print(std::ostream &out) {
    return quadSet::print(out);
  }
  std::ostream& emit(std::ostream &out) {
    return quadSet::print(out);
  }
};

//static_assert (std::is_pod< CellSet<8> >::value, "CellSet must be POD");


template<bitpos S>
std::ostream& operator<< (std::ostream &out, CellSet<S> set) {
  return set.emit(out);
}

