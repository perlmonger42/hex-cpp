#include <iostream>
#include <stdint.h>
#include <initializer_list>
#include "../quadset/quadset.h"

// SIZE is the edge length of the board.
// BITS is the number of cells on a board of that size.
// QUADS is the number of 64-bit unsigned integers necessary
// to represent a set of cells on a board of that size.
// For example, an 11×11 board has SIZE=11, BITS=121, and QUADS=2.
template<bitpos SIZE, bitpos BITS = SIZE*SIZE, bitpos QUADS = (BITS+63) / 64>
class cellset : public quadset<SIZE*SIZE> {
public:
  typedef cellset<SIZE> cellSet;
  typedef quadset<BITS> quadSet;

  inline constexpr cellset() { quadSet::reset(); }
  inline constexpr cellset(uint64_t val) : quadSet{val} { }
  inline constexpr cellset(quadSet val) : quadSet(val) { }
  // inline constexpr cellset() : quadSet() { }
  // inline constexpr cellset(quadSet const &other) : quadSet(other) { }
  // inline constexpr cellset(cellset const &other) : quadSet(other) { }
  // inline constexpr cellset(unsigned long long val) : quadSet{val} { }
  
  static inline constexpr cellSet make() {
    return cellSet{ quadSet::make() };
  }

  template<typename... Args>
  static inline constexpr cellSet make(bitpos p, Args... args) {
    return cellSet{ quadSet::make(p, args...) };
  }

  static inline cellSet make(std::initializer_list<bitpos> list) {
    return cellSet{ quadSet::make(list) };
  }

  static inline constexpr cellSet range(bitpos m, bitpos n) {
    return cellSet{ quadSet::range(m, n) };
  }

  static inline constexpr cellSet universe() {
    return cellSet{ quadSet::universe() };
  }

  inline constexpr cellSet clean() {
    return cellSet{ quadSet::clean() };
  }

  inline constexpr bitpos size() { return BITS; }

  inline constexpr bool any() const {
    return this->quadSet::any();
  }

  inline constexpr bool none() const {
    return this->quadSet::none();
  }

  inline constexpr bool all() const {
    return this->quadSet::all();
  }

  inline constexpr bitpos count() const {
    return this->quadSet::count();
  }

  inline constexpr bool operator==(cellSet other) const {
    return this->quadSet::operator==(other);
  }

  inline constexpr bool operator!=(cellSet other) const {
    return this->quadSet::operator!=(other);
  }

  inline constexpr bool test(bitpos pos) const {
    return this->quadSet::test(pos);
  }

  inline constexpr uint64_t to_ullong() const {
    return this->quadSet::to_ullong();
  }

  inline constexpr cellSet operator<<(bitpos n) const {
    return cellSet{ this->quadSet::operator<<(n) };
  }

  inline constexpr cellSet fast_lsh(bitpos n) const {
    return cellSet{ this->quadSet::fast_lsh(n) };
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

  inline constexpr cellSet operator^ (cellSet other) const {
    return cellSet{ this->quadSet::operator^(other) };
  }

  inline constexpr cellSet operator- (cellSet other) const {
    return cellSet{ this->quadSet::operator-(other) };
  }

  inline constexpr cellSet operator~ () const {
    return cellSet{ this->quadSet::operator~() };
  }

  inline constexpr cellSet fast_not () const {
    return cellSet{ this->quadSet::fast_not() };
  }

  inline cellSet& operator<<= (bitpos n) {
    this->quadSet::operator<<=(n);
    return *this;
  }

  inline cellSet& fast_lsh_assign (bitpos n) {
    this->quadSet::fast_lsh_assign(n);
    return *this;
  }

  inline cellSet& operator>>= (bitpos n) {
    this->quadSet::operator>>=(n);
    return *this;
  }

  inline cellSet& operator&= (cellSet other) {
    this->quadSet::operator&=(other);
    return *this;
  }

  inline cellSet& operator|= (cellSet other) {
    this->quadSet::operator|=(other);
    return *this;
  }

  inline cellSet& operator^= (cellSet other) {
    this->quadSet::operator^=(other);
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

  inline cellSet& set(bitpos m, bitpos n) {
    this->quadSet::set(m, n);
    return *this;
  }

  inline cellSet& set(std::initializer_list<bitpos> list) {
    this->quadSet::set(list);
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

  inline cellSet& reset(bitpos m, bitpos n) {
    this->quadSet::reset(m, n);
    return *this;
  }

  inline cellSet& reset(std::initializer_list<bitpos> list) {
    this->quadSet::reset(list);
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

  inline cellSet& flip(bitpos m, bitpos n) {
    this->quadSet::flip(m, n);
    return *this;
  }

  inline cellSet& flip(std::initializer_list<bitpos> list) {
    this->quadSet::flip(list);
    return *this;
  }



  static inline constexpr cellSet top() {
    return cellSet{ uint64_t(-1LL) >> (64-SIZE) };
  }

  static inline constexpr cellSet bottom() {
    return top() << (BITS - SIZE);
  }

  static inline constexpr cellSet left() {
    return cellSet{ quadSet::repeat_block(quadSet{1}, SIZE, SIZE) };
  }

  static inline constexpr cellSet right() {
    return cellSet{ quadSet::repeat_block(quadSet{1<<(SIZE-1)}, SIZE, SIZE) };
  }

  constexpr cellSet neighbors() const {
    constexpr auto l = ~left();
    constexpr auto r = ~right();
    auto s = *this;
    return ( (s >> SIZE)
           | (((s >> (SIZE - 1)) | (s << 1)) & l)
           | (((s << (SIZE - 1)) | (s >> 1)) & r)
           | (s << SIZE)
           );
  }

  constexpr cellSet fast_neighbors() const {
    constexpr auto l = left().fast_not();
    constexpr auto r = right().fast_not();
    auto s = *this;
    return ( (s >> SIZE)
           | (((s >> (SIZE - 1)) | s.fast_lsh(1)) & l)
           | ((s.fast_lsh(SIZE - 1) | (s >> 1)) & r)
           | s.fast_lsh(SIZE)
           );
  }

  std::ostream& print(std::ostream &out) {
    return quadSet::print(out);
  }
  std::ostream& emit(std::ostream &out) {
    return quadSet::print(out);
  }
};

//static_assert (std::is_pod< cellset<8> >::value, "cellset must be POD");


template<bitpos SIZE>
std::ostream& operator<< (std::ostream &out, cellset<SIZE> set) {
  return set.emit(out);
}
