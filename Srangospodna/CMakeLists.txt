project(dtf CXX C)
cmake_minimum_required(VERSION 2.6)

add_subdirectory(lib/gtest-1.6.0)

include_directories(
  ${gtest_SOURCE_DIR}/include
  ${dtf_SOURCE_DIR}/include)

add_executable(dtf
    src/main.cpp
		src/Lexer.cpp
		src/Node.cpp
		src/Seman.cpp)
				
add_executable(dtf_test
    src/test.cpp
    		src/Lexer.cpp
		src/Parser.cpp
		src/Node.cpp
		src/Seman.cpp)

target_link_libraries(dtf gtest gtest_main)
target_link_libraries(dtf_test gtest gtest_main)
