cmake_minimum_required(VERSION 3.5.0)
project(cgv_compiler VERSION 0.1.0 LANGUAGES C CXX)

add_executable(cgv_compiler cgv_compiler.cpp file_reader.cpp lexer.cpp lexer_keywords.cpp ll_parser.cpp error_helper.cpp semantic.cpp)
add_executable(cgv_lex_test cgv_lex_test.cpp file_reader.cpp lexer.cpp lexer_keywords.cpp error_helper.cpp)