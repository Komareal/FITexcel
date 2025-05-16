cmake_minimum_required(VERSION 3.1...3.29)

project(excel
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -Wall -pedantic -Wextra -Wshadow -Wno-long-long -O3 ")


if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    set(EXPRESSION_PARSER_LIB "${CMAKE_SOURCE_DIR}/lib/x86_64-linux-gnu/libexpression_parser.a")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    set(EXPRESSION_PARSER_LIB "${CMAKE_SOURCE_DIR}/lib/arm-linux-gnu/libexpression_parser.a")
else ()
    message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

add_executable(excel
        main.cpp
        src/Cell/CBuilder.cpp
        src/Cell/CCell.cpp
        src/Cell/CPos.cpp
        src/CSpreadsheet.cpp
        test/test.cpp
        src/Cell/CRefManager.cpp
        src/Cell/AST/CASTValNode.cpp
        src/Cell/AST/CASTRefNode.cpp
        src/Cell/AST/CASTBinaryNode.cpp
        src/Cell/AST/CASTOperations.cpp
        src/Cell/AST/CASTUnaryNode.cpp
        src/Cell/AST/CASTFuncNode.cpp
        src/Cell/AST/CASTRangeNode.cpp
        src/Cell/CPosRangeIterator.cpp
)

target_link_libraries(excel ${EXPRESSION_PARSER_LIB})
