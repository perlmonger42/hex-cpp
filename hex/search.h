#include "cellset.h"
#include <vector>
#include <tuple>

template<bitpos SIZE>
class threat;

template<bitpos SIZE>
class pattern {
public:
  cellset<SIZE> via;  // All the cells required by this threat or threat_fork.

  pattern() : via() { }
  pattern(cellset<SIZE> cells) : via(cells) { }
};

template<bitpos SIZE>
class threat : public pattern<SIZE> {
public:
  // The threat is that if all the cells in ‹via› are vacant, claiming cell
  // ‹play› will create some kind of connection.
  bitpos  play; // The immediate move that will vivify the threat.

  // invariant: ‹play› is in ‹pattern::via› 

  threat() : pattern<SIZE>(), play(0) { }
  threat(cellset<SIZE> cells, bitpos at) : pattern<SIZE>(cells), play(at) { }
};

template<bitpos SIZE>
class threat_fork : public pattern<SIZE> {
public:
  typedef std::vector< threat<SIZE> > threatVec;

  // A threat_fork represents the fact that there are multiple threats that cannot be
  // simultaneously countered. The threats are the tines of the threat_fork.
  threatVec tines; // The tines of the threat_fork.

  // invariant: ‹via› is the union of all ‹tines[i].via›.

  threat_fork() : pattern<SIZE>(), tines() { }
  threat_fork(threatVec threats) : pattern<SIZE>(), tines(threats) {
    for (auto tine : tines) {
      this->via |= tine.via;
    }
  }
};

template<bitpos SIZE>
std::tuple<char /* winner */, pattern<SIZE>* /* via */> search(Board<SIZE> b) {
  char me = b.player();
  cellset<SIZE> must;  // `me` will lose if s/he doesn't play in one of these
  cellset<SIZE> moves = b.emptyCells(); // potential moves to be tested



  threat<SIZE> *t = new threat<SIZE>(Board<SIZE>::cellSet::universe(), 0);

  return std::make_tuple< char,pattern<SIZE>* >('X', t);
}
