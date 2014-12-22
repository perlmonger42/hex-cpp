# Add executable called "hexdemo" that is built from the source file
# "demo.cpp". The extensions are automatically found.
add_executable (hexdemo demo.cpp)

include_directories(
  "../panic"
  "../quadset"
  "../hex"
)

set(GCC_COMPILE_FLAGS "-std=gnu++1y -fno-implicit-templates -Wc++1y-extensions")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

target_link_libraries (hexdemo LINK_PUBLIC panic quadset)
add_dependencies (hexdemo quadset panic)
