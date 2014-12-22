cmake_minimum_required (VERSION 2.8.11)

add_library (quadset bitops.cpp bitops.h qset1.h qset2.h quadset.h)
add_dependencies (quadset panic)

set(GCC_COMPILE_FLAGS "-std=gnu++1y -fno-implicit-templates -Wc++1y-extensions")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

#target_include_directories (quadset PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(quadset panic)
include_directories("../panic")

if (test)
  enable_testing()
  include_directories("${gtest_SOURCE_DIR}/include" "${gtest_SOURCE_DIR}")
  set(EXTRA_LIBS ${EXTRA_LIBS} gtest gtest_main panic quadset)

  add_executable (quadset_test quadset_test.cpp quadset_test.inc qset1.h qset2.h quadset.h)
  add_dependencies(quadset_test ${EXTRA_LIBS})
  target_link_libraries(quadset_test ${EXTRA_LIBS})
  add_test(NAME qtest COMMAND quadset_test)
endif()
