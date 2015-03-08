#
# >-------------< MulitQ Parser >--------------<
#

cmake_minimum_required(VERSION 3.0)

FILE(GLOB PARSER_HEADERS *.h)
FILE(GLOB PARSER_SOURCES *.cpp)

add_library( csvParser STATIC  ${PARSER_SOURCES} ${PARSER_HEADERS} ) 

install( TARGETS csvParser DESTINATION lib)
install( FILES PARSER_HEADERS DESTINATION include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#include_directories(${CMAKE_SOURCE_DIR}/parser/) #PARSER
#include_directories(${CMAKE_BINARY_DIR}/parser/) #PARSER

source_group(Headers FILES ${PARSER_HEADERS})
source_group(Sources FILES ${PARSER_SOURCES})