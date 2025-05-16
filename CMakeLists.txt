cmake_minimum_required(VERSION 3.1...3.29)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -Wall -pedantic -Wextra -Wshadow -Wno-long-long -g -fsanitize=address")


project(
        excel
        VERSION 1.0
        LANGUAGES CXX)
add_executable(excel main.cpp
        Cell/CBuilder.cpp
        Cell/CCell.cpp
        Cell/CPos.cpp
        CSpreadsheet.cpp
        test.cpp
        Cell/CRefManager.cpp
        Cell/AST/CASTValNode.cpp
        Cell/AST/CASTRefNode.cpp
        Cell/AST/CASTBinaryNode.cpp
        Cell/AST/CASTOperations.cpp
        Cell/AST/CASTUnaryNode.cpp
        Cell/AST/CASTFuncNode.cpp
        Cell/AST/CASTRangeNode.cpp
        Cell/AST/CASTRangeNode.h
        Cell/CPosRangeIterator.cpp
        Cell/CPosRangeIterator.h
)
target_link_libraries(excel ${CMAKE_SOURCE_DIR}/x86_64-linux-gnu/libexpression_parser.a)
