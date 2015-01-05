#ifndef __HEXSEARCH_H__
#define __HEXSEARCH_H__ 1
#include <cassert>
#include <ostream>
#include <vector>
#include <algorithm>
#include "cellset.h"
#include "board.h"
#include "winner.h"

template<bitpos SIZE>
class Threat;

enum pattern_kind { NONE, THREAT, FORK };

template<bitpos SIZE>
class Pattern;

template<bitpos SIZE>
class Pattern {
  typedef std::vector< Pattern<SIZE> > patternVec;
  typedef cellset<SIZE>                cellSet;

  pattern_kind   kind; // Is this a Threat or a Fork?
  bitpos         play; // Defined only for Threats.
  cellSet        body; // All the cells required by this Threat or Fork.
  patternVec     subs; // Any needed subpatterns.

public:

  static Pattern empty_fork() {
    Pattern p;
    p.kind = FORK;
    return p;
  }

  Pattern()
    : kind(NONE), play(-1), body(), subs() { }

  Pattern(int at)
    : kind(THREAT), play(at), body(), subs()
    {
      body.set(at);
    }

  // Pattern(int at, cellSet cells)
  //   : kind(THREAT), play(at), body(cells), subs()
  // {
  //   body.set(at);
  // }

  Pattern(int at, const Pattern &patt)
    : kind(THREAT), play(at), body(patt.body), subs({patt})
  {
    body.set(at);
  }

  // Pattern(int at, cellSet cells, const Pattern &patt)
  //   : kind(THREAT), play(at), body(cells), subs({patt})
  // {
  //   body.set(at);
  // }

  Pattern(std::initializer_list< Pattern<SIZE> > list)
    : kind(FORK), play(-1), body(), subs(list)
  {
    for (auto p : subs) {
      body |= p.body;
    }
  }

  Pattern(const patternVec &vec)
    : kind(FORK), play(-1), body(), subs(vec)
  {
    for (auto p : subs) {
      body |= p.body;
    }
  }

  Pattern(patternVec &&vec)
    : kind(FORK), play(-1), body(), subs(vec)
  {
    for (auto p : subs) {
      body |= p.body;
    }
  }

  Pattern(const Pattern<SIZE> &other)
    : kind(other.kind), play(other.play), body(other.body), subs(other.subs)
  { }

  Pattern(Pattern<SIZE> &&other)
    : kind(other.kind), play(other.play), body(other.body), subs(other.subs)
  { }

  Pattern &operator=(const Pattern &other) {
    this->kind = other.kind;
    this->play = other.play;
    this->body = other.body;
    this->subs = other.subs;
    return *this;
  }

  Pattern &operator=(Pattern &&other) {
    this->kind = other.kind;
    this->play = other.play;
    this->body = other.body;
    std::swap(this->subs, other.subs);
    return *this;
  }

  Pattern &swap(Pattern<SIZE> &other)
  {
    pattern_kind k = other.kind;
    bitpos       p = other.play;
    cellSet      b = other.b;

    other.kind = this->kind;
    other.play = this->play;
    other.body = this->body;

    this->kind = k;
    this->play = p;
    this->body = b;

    std::swap(this->subs, other.subs);
    return *this;
  }

  bool none() { return kind == NONE; }

  int size() { return subs.size(); }

  bitpos threat_loc() { return play; }

  cellSet cells() const { return body; }

  // a measure of this pattern's complexity
  int cost() const {
    int sum = 1 + 100 * subs.size() * subs.size();
    for (auto &sub : subs) {
      sum += sub.cost();
    }
    return sum;
  }

  Pattern<SIZE> &add_subpattern(const Pattern<SIZE> &sub) {
    subs.push_back(sub);
    body |= sub.body;
    return *this;
  }

  bool is_solid() {
    assert(kind == FORK);
    if (this->subs.size() == 0) {
      return false;
    }
    cellset<SIZE> overlap = this->subs[0].cells();
    for (int tine = 1; tine < this->subs.size(); ++tine) {
      overlap &= this->subs[tine].cells();
    }
    return overlap.none();
  }

  int cmp(const Pattern &other) const {
    int a = this->cells().count();
    int b = other.cells().count();
    if (a < b) return -1;
    if (a > b) return  1;
    a = SIZE*SIZE - this->play;
    b = SIZE*SIZE - other.play;
    if (a < b) return -1;
    if (a > b) return  1;
    a = this->cost();
    b = other.cost();
    if (a < b) return -1;
    if (a > b) return  1;
    return 0;
  }

  bool operator<(Pattern &other) const {
    return this->cmp(other) < 0;
  }

  bool operator<= (Pattern &other) const {
    return this->cmp(other) <= 0;
  }

  bool operator> (Pattern &other) const {
    return this->cmp(other) > 0;
  }

  bool operator>= (Pattern &other) const {
    return this->cmp(other) >= 0;
  }

  Pattern<SIZE> minimum_fork() {
    assert(kind == FORK);
    assert(this->is_solid());
    Pattern<SIZE> cheapest_fork = *this;
    int cheapest_cost = cheapest_fork.cost();
    int tine_count = this->subs.size();
    int limit_set = 1 << tine_count;
    for (int subset = 3; subset < limit_set; ++subset) {
      Pattern<SIZE> subset_fork({});
      for (int tine = 0; tine < this->subs.size(); ++tine) {
        if (((1 << tine) & subset) != 0) {
          subset_fork.add_subpattern(this->subs[tine]);
        }
      }
      if (subset_fork.is_solid()) {
        int cost = subset_fork.cost();
        if (cost < cheapest_cost) {
          cheapest_cost = cost;
          cheapest_fork = subset_fork;
        }
      }
    }
    return cheapest_fork;
  }

  std::ostream &print(std::ostream &out) {
    switch (kind) {
    case NONE:
      out << "NONE()";
      break;
    case THREAT:
      out << "THREAT at " << play;
      if (subs.size() > 0) {
        out << " establishes connection via " << subs[0];
      }
      break;
    case FORK:
      out << "FORK(";
      for (int i = 0; i < subs.size(); ++i) {
        if (i > 0) out << ", ";
        out << subs[i];
      }
      out << ')';
      break;
    }
    return out;
  }
};

template<bitpos SIZE>
std::ostream &operator<<(std::ostream &out, Pattern<SIZE> &patt) {
  return patt.print(out);
}

#if 0
template<bitpos SIZE>
class Threat : public Pattern<SIZE> {
public:
  // The threat is that if all the cells in ‹footprint› are vacant, claiming
  // cell ‹play› will create some kind of connection.

  // The immediate move that will vivify the threat.
  bitpos  play;

  // The virtual connection created by ‹play›, if any.
  Pattern<SIZE> *via;

  // invariant: ‹play› is in ‹Pattern::footprint› 

  Threat() : Pattern<SIZE>(), play(-1), via(nullptr) { }

  Threat(bitpos at) : Pattern<SIZE>(), play(at), via(nullptr)
  {
    this->footprint.set(at);
  }

  Threat(bitpos at, Pattern<SIZE> *via_)
    : Pattern<SIZE>(via_ ? via_->cells() : cellset<SIZE>()),
      play(at), via(via_)
  {
    this->footprint.set(at);
  }

  Threat(const Threat& other)
    : Pattern<SIZE>(other.cells()),
      play(other.at),
      via(other.via == nullptr ? nullptr : other.via->clone())
  {
  }

  Threat(Threat&& other)
    : Pattern<SIZE>(other.cells()), play(other.at), via(nullptr)
  {
    Pattern<SIZE> *save = other.via;
    this->via = other.via;
    other.via = nullptr;
  }

  Threat<SIZE> *clone() {
    return new Threat(this->at, this->footprint,
                      this->via == nullptr ? nullptr : this->via->clone());
  }

  Threat<SIZE> &operator=(const Threat<SIZE> &other) {
    this->at = other.at;
    this->footprint = other.footprint;
    this->via = other.via == nullptr ? nullptr : other.via->clone();
    return *this;
  }

  Threat<SIZE> &operator=(Threat<SIZE> &&other) {
    Pattern<SIZE> *save = this->via;
    this->at = other.at;
    this->footprint = other.footprint;
    this->via = other.via == nullptr ? nullptr : other.via->clone();
    other.via = save;
    return *this;
  }

  ~Threat() {
    if (this->via) {
      delete via;
    }
  }


  int cost() {
    if (via != nullptr) {
      return 1 + via->cost();
    } else {
      return 1;
    }
  }
};

template<bitpos SIZE>
class Fork : public Pattern<SIZE> {
public:
  // A fork represents the fact that there are multiple threats that cannot be
  // simultaneously countered. The threats are the tines of the fork.

  typedef std::vector< Threat<SIZE> > threats;

  threats tines; // The tines of the fork.

  // invariant: ‹footprint› is the union of all ‹tines[i].footprint›.

  Fork() : Pattern<SIZE>(), tines() { }

  Fork(threats list) : Pattern<SIZE>(), tines(list) {
    for (auto tine : tines) {
      this->footprint |= tine.footprint;
    }
  }

  Fork(const Fork<SIZE>& other)
    : Pattern<SIZE>(other.footprint), tines(other.tines)
  {
  }

  Fork(Fork<SIZE>&& other)
    : Pattern<SIZE>(other.footprint), tines(other.tines)
  {
  }

  Fork &operator= (Fork<SIZE>&& other) {
    this->footprint = other.footprint;
    tines = other.tines;
  }

  Fork &add_threat(Threat<SIZE> threat) {
    this->tines.push_back(threat);
    this->footprint |= threat.footprint;
    return *this;
  }

  int cost() {
    int sum = 100 * tines.size() * tines.size();
    for (auto t : tines) {
      sum += t.cost();
    }
    return sum;
  }
};
#endif

template<bitpos SIZE>
std::vector<bitpos> shuffled_set_elements(cellset<SIZE> set) {
  std::vector<bitpos> result;
  while (set.any()) {
    bitpos p = set.min();
    set.reset(p);
    result.push_back(p);
  }
  std::random_shuffle(result.begin(), result.end());
  return result;
}

template<bitpos SIZE>
char search(const Board<SIZE> &b, Pattern<SIZE> &via, std::string indent="") {
  const bool DEBUG = false;
  char me = b.player();

  // Player will lose if they don't play in one of these cells.
  cellset<SIZE> must = b.emptyCells();

  // Potential moves to be tested.
  std::vector<bitpos> moves = shuffled_set_elements<SIZE>(b.emptyCells());

  Pattern<SIZE> my_cheapest_win;
  Pattern<SIZE> opponent_threats = Pattern<SIZE>::empty_fork();
  for (bitpos n : moves) {

    if (DEBUG) std::cout << indent << me << " is now trying move at " << n
                         << std::endl;

    if (!must.test(n)) {
      if (DEBUG) std::cout << indent
                           << "skip it; it isn't in the must-move set"
                           << std::endl;
      continue; // Don't bother checking moves that we know will lose.
    }

    auto new_board = Board<SIZE>(b).go(n);
    auto w = new_board.winner(me);

    if (w == me) {
      if (DEBUG) std::cout << indent << me << " moving at " << n
                           << " is an immediate win" << std::endl;
      my_cheapest_win = Pattern<SIZE>(n);
      if (DEBUG) std::cout << indent << "cheapest win is now "
                           << my_cheapest_win << std::endl;
      continue;
      //break; // We won't find a cheaper win than a single-play victory.
    }

    // Further searching is needed.
    Pattern<SIZE> patt;
    if (DEBUG) std::cout << indent << "recurring after move at " << n
                         << std::endl;
    w = search(new_board, patt, indent + "  ");
    if (w == me) {
      patt = Pattern<SIZE>(n, patt);
      if (DEBUG) std::cout << indent << me << " moving at " << n
                           << " wins recursively" << std::endl;
      if ((my_cheapest_win.none()) || my_cheapest_win > patt) {
        my_cheapest_win = patt;
        if (DEBUG) std::cout << indent << "cheapest win is now " 
                             << my_cheapest_win << std::endl;
      } else {
        if (DEBUG) std::cout << indent << "but the win was too expensive" 
                             << std::endl;
      }
    } else {
      // w is the opponent, because search() never returns w == '?'.
      if (DEBUG) std::cout << indent << me << " moving at " << n
                           << " loses recursively" << std::endl;
      if (DEBUG) std::cout << indent << "opponent " << b.opponent()
                           << " wins via " << patt << std::endl;
      must &= patt.cells();
      if (DEBUG) std::cout << indent << "must-move narrows to "
                           << must << std::endl;
      opponent_threats.add_subpattern(patt);
    }
  }

  if (my_cheapest_win.none()) {
      if (DEBUG) std::cout << indent << "final: " << me << " loses"
                           << std::endl;
    via = opponent_threats.minimum_fork();
    if (DEBUG) std::cout << indent << "opponent " << b.opponent()
                         << " wins via " << via << std::endl;
    return b.opponent();
  } else {
    via = my_cheapest_win;
    if (DEBUG) std::cout << indent << "final: " << me 
                         << " wins; cheapest solution is " << via << std::endl;
    return me;
  }
}
#endif /* __HEXSEARCH_H__ */
