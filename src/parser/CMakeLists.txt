#
# >-------------< MulitQ Parser >--------------<
#

cmake_minimum_required(VERSION 3.0)

FILE(GLOB PARSER_HEADERS *.h)
FILE(GLOB PARSER_SOURCES *.cpp)

add_library( parser STATIC ${PARSER_HEADERS} ${PARSER_SOURCES} ) 

install( TARGETS parser DESTINATION lib)
install( FILES PARSER_HEADERS DESTINATION include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

source_group(Headers FILES ${MP_SCANNER_HEADERS})
source_group(Sources FILES ${MP_SCANNER_SOURCES})