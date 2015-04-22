#
# >-------------< Bandwidth Measurement Package>--------------<
#

cmake_minimum_required(VERSION 3.0)

FILE(GLOB BANDWIDTH_HEADERS *.h)
FILE(GLOB BANDWIDTH_SOURCES *.cpp)

add_library( bandwidth STATIC  ${BANDWIDTH_SOURCES} ${BANDWIDTH_HEADERS} ) 

install( TARGETS bandwidth DESTINATION lib)
install( FILES BANDWIDTH_HEADERS DESTINATION include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

source_group(Headers FILES ${BANDWIDTH_HEADERS})
source_group(Sources FILES ${BANDWIDTH_SOURCES})