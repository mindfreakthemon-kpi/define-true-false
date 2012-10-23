project(dtf CXX C)
cmake_minimum_required(VERSION 2.6)

add_subdirectory(lib/gtest-1.6.0)

include_directories(
  ${gtest_SOURCE_DIR}/include
  ${dtf_SOURCE_DIR}/include)

add_executable(test_duck
    src/lexer_test.cc
    src/lexer.cc)

set_target_properties(test_duck
    PROPERTIES
    COMPILE_FLAGS "-std=c++11")

target_link_libraries(test_duck gtest gtest_main)

