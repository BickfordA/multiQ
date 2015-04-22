#
# >-------------< Delimited Text File Writer >--------------<
#

cmake_minimum_required(VERSION 3.0)

FILE(GLOB DTF_HEADERS *.h)
FILE(GLOB DFT_SOURCES *.cpp)

add_library( dtfWriter STATIC  ${DFT_SOURCES} ${DTF_HEADERS} ) 

install( TARGETS dtfWriter DESTINATION lib)
install( FILES DTF_HEADERS DESTINATION include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

source_group(Headers FILES ${DTF_HEADERS})
source_group(Sources FILES ${DFT_SOURCES})