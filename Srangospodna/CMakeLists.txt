project(dtf CXX C)
cmake_minimum_required(VERSION 2.6)

add_subdirectory(lib/gtest-1.6.0)

include_directories(
  ${gtest_SOURCE_DIR}/include
  ${dtf_SOURCE_DIR}/include)

add_executable(dtf
    src/main.cpp
		src/Lexer.cpp)
		
add_executable(dtf_lexer_test
    src/test_lexer.cpp
		src/Lexer.cpp)
				
add_executable(dtf_parser_test
    src/test_parser.cpp
    		src/Lexer.cpp
		src/Parser.cpp)

target_link_libraries(dtf gtest gtest_main)
target_link_libraries(dtf_lexer_test gtest gtest_main)
target_link_libraries(dtf_parser_test gtest gtest_main)

