# 
# >--------------< multiQ >--------------------<
#

cmake_minimum_required(VERSION 3.0)
 

FILE(GLOB MULTIQ_HEADERS *.h)
FILE(GLOB MULTIQ_SOURCES *.cpp)

add_library( multiQ STATIC ${MULTIQ_HEADERS} ${MULTIQ_SOURCES} ) 

install( TARGETS multiQ DESTINATION lib)
install( FILES MULTIQ_HEADERS DESTINATION include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


include_directories(${CMAKE_SOURCE_DIR}/parser/) #parser
include_directories(${CMAKE_BINARY_DIR}/parser/) #parser


source_group(Headers FILES ${MULTIQ_HEADERS})
source_group(Sources FILES ${MULTIQ_SOURCES})