# CMakeLists files in this project can refer to the root source directory of
# the project as ${HEX_SOURCE_DIR} and to the root binary directory of the
# project as ${HEX_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (HEX)

if (test)
  enable_testing()
endif()

add_subdirectory (gtest-1.7.0)
add_subdirectory (panic)
add_subdirectory (quadset)
add_subdirectory (hex)
add_subdirectory (prog)
