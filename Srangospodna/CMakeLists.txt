project(dtf CXX C)
cmake_minimum_required(VERSION 2.6)

add_subdirectory(lib/gtest-1.6.0)

include_directories(
  ${gtest_SOURCE_DIR}/include
  ${dtf_SOURCE_DIR}/include)

add_executable(dtf
    src/test.cpp)

set_target_properties(dtf
    PROPERTIES
    COMPILE_FLAGS "-std=c++11")

target_link_libraries(dtf gtest gtest_main)

