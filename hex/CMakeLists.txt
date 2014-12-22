cmake_minimum_required (VERSION 2.8.11)

option(test "Build all tests." OFF)

set(GCC_COMPILE_FLAGS "-O3 -std=gnu++1y -fno-implicit-templates -Wc++1y-extensions")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

include_directories(
  "../panic"
  "../quadset"
)

if (test)
  enable_testing()
  include_directories("${gtest_SOURCE_DIR}/include" "${gtest_SOURCE_DIR}")
  set(EXTRA_LIBS ${EXTRA_LIBS} gtest gtest_main panic quadset)

  add_executable (board_test board_test.cpp board.h)
  add_dependencies(board_test panic quadset)
  target_link_libraries(board_test ${EXTRA_LIBS})
  add_test(NAME board COMMAND board_test)

  add_executable (winner_test winner_test.cpp)
  add_dependencies(winner_test panic quadset)
  target_link_libraries(winner_test ${EXTRA_LIBS})
  add_test(NAME winner COMMAND winner_test)

  add_executable (winner_benchmark winner_benchmark.cpp)
  add_dependencies(winner_benchmark panic quadset)
  target_link_libraries(winner_benchmark ${EXTRA_LIBS})
endif()
