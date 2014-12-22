#include <iostream>
//#include <type_traits>
#include "board.h"
#include "winner.h"

//template class CellSet<4>;
//template class StandardSets<4>;
//template class Board<13>;

int main(int argc, char **argv) {

  //std::cout << "bit<64,4> is pod: " << std::is_pod< bit<64,4> >::value << std::endl;
  //std::cout << "CellSet<4> is pod: " << std::is_pod< CellSet<4> >::value << std::endl;
  std::cout << "All: " << CellSet<4>::universe() << " : " << CellSet<4>::universe().to_ullong() << std::endl;
  // std::cout << "Top: " << StandardSets<4>::top << " : " << StandardSets<4>::top.to_ullong() << std::endl;
  // std::cout << "Bottom: " << StandardSets<4>::bottom << " : " << StandardSets<4>::bottom.to_ullong() << std::endl;
  // std::cout << "Left: " << StandardSets<4>::left << " : " << StandardSets<4>::left.to_ullong() << std::endl;
  // std::cout << "Right: " << StandardSets<4>::right << " : " << StandardSets<4>::right.to_ullong() << std::endl;
  Board<4> b(R"(
      - - - -
       - - X O
        - O O O
         O O O O)");
  std::cout << b;
  b.MappedRender(std::cout, "", {{'*', b.verticalCells().neighbors()}});

  Board<7> b7(R"(
      X - - - - - X
       - - - - - - -
        - - - - - - -
         - - - X - - -
          - - - - - - -
           - - - - - - -
            X - - - - - X)");
  std::cout << b7;
  std::cout << b7.verticalCells() << std::endl;
  auto n = b7.verticalCells().neighbors();
  b7.MappedRender(std::cout, "", {{'*', n}});
  //std::cout << n << std::endl;


  Board<11> b11(R"(
      X - - - - - - - - - X
       - - - - - X - - - - -
        - - - - - - - - - - -
         - - - - - - - - - - -
          - - - - - - - - - - -
           - X - - - X - - - X -
            - - - - - - - - - - -
             - - - - - - - - - - -
              - - - - - - - - - - -
               - - - - - X - - - - -
                X - - - - - - - - - X)");
  std::cout << b11;
  auto n11 = b11.verticalCells().neighbors();
  b11.MappedRender(std::cout, "", {{'*', n11}});
  //std::cout << n11 << std::endl;

  b11 = Board<11>(R"(
      - - - - - X - - - - -
       - - - - - - - - - - -
        - - - - - - - - - - -
         - - - - - - - - - - -
          X - - - - - - - - - -
           - - - - - X - - - - -
            - - - - - - - - - - X
             - - - - - - - - - - -
              - - - - - - - - - - -
               - - - - - - - - - - -
                - - - - - X - - - - -)");
  std::cout << b11;
  // auto s11 = b11.topRow();
  // b11.MappedRender(std::cout, "", {{'*', s11}});
  // s11 = b11.bottomRow();
  // b11.MappedRender(std::cout, "", {{'*', s11}});
  // s11 = b11.leftColumn();
  // b11.MappedRender(std::cout, "", {{'*', s11}});
  // s11 = b11.rightColumn();
  // b11.MappedRender(std::cout, "", {{'*', s11}});
  b11.MappedRender(std::cout, "", {{'*', b11.verticalCells().neighbors()}});
  // //std::cout << n11 << std::endl;

  // Board<1>::MappedRender(std::cout, "", {{'*', Board<1>::leftColumn()}});
  // Board<2>::MappedRender(std::cout, "", {{'*', Board<2>::leftColumn()}});
  // Board<3>::MappedRender(std::cout, "", {{'*', Board<3>::leftColumn()}});
  // Board<4>::MappedRender(std::cout, "", {{'*', Board<4>::leftColumn()}});
  // Board<5>::MappedRender(std::cout, "", {{'*', Board<5>::leftColumn()}});
  // Board<6>::MappedRender(std::cout, "", {{'*', Board<6>::leftColumn()}});
  // Board<7>::MappedRender(std::cout, "", {{'*', Board<7>::leftColumn()}});
  // Board<8>::MappedRender(std::cout, "", {{'*', Board<8>::leftColumn()}});
  // Board<9>::MappedRender(std::cout, "", {{'*', Board<9>::leftColumn()}});
  // Board<10>::MappedRender(std::cout, "", {{'*', Board<10>::leftColumn()}});
  // Board<11>::MappedRender(std::cout, "", {{'*', Board<11>::leftColumn()}});
  // Board<12>::MappedRender(std::cout, "", {{'*', Board<12>::leftColumn()}});
  // Board<13>::MappedRender(std::cout, "", {{'*', Board<13>::leftColumn()}});
  // 
  // std::cout << 1 << ": " << shared_bits<64,1>::left() << std::endl;
  // std::cout << 2 << ": " << shared_bits<64,2>::left() << std::endl;
  // std::cout << 3 << ": " << shared_bits<64,3>::left() << std::endl;
  // std::cout << 4 << ": " << shared_bits<64,4>::left() << std::endl;
  // std::cout << 5 << ": " << shared_bits<64,5>::left() << std::endl;
  // std::cout << 6 << ": " << shared_bits<64,6>::left() << std::endl;
  // std::cout << 7 << ": " << shared_bits<64,7>::left() << std::endl;
  // std::cout << 8 << ": " << shared_bits<64,8>::left() << std::endl;
  // std::cout << 9 << ": " << shared_bits<128,9>::left() << std::endl;
  // std::cout << 10 << ": " << shared_bits<128,10>::left() << std::endl;
  // std::cout << 11 << ": " << shared_bits<128,11>::left() << std::endl;
  // std::cout << 12 << ": " << shared_bits<192,12>::left() << std::endl;
  // std::cout << 13 << ": " << shared_bits<192,13>::left() << std::endl;

  auto b13 = Board<13>(R"(
      X X - - - X - - - - - - -
       X - - - - - - - - - X - -
        - - - - - - - - - - X - -
         - - - - - - - - - - X - -
          X - - - - - - - - - - - -
           - - - - - - X - - - - - -
            - - - - - - - - - - - - X
             - - - - - - - - - - - - -
              - - - - - - - - - - - - -
               - - - - - - - - - - - - -
                - - - - - - - - - - - - -
                 - X X X - - - - - - - - X
                  - - - - - - - X - - - - X)");
  std::cout << b13;
  b13.MappedRender(std::cout, "", {{'*', b13.verticalCells().neighbors()}});
  return 0;
}
