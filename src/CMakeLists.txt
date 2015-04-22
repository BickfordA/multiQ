cmake_minimum_required(VERSION 3.0)

project(MultiQ_Metrics)

add_subdirectory(parser)
add_subdirectory(multiQ)
add_subdirectory(delimitedTextWriter)
add_subdirectory(bandwidth)

add_executable( mq MainDriver.cpp)

target_link_libraries(mq csvParser )
target_link_libraries(mq multiQ)
target_link_libraries(mq dtfWriter)
target_link_libraries(mq bandwidth)

install(TARGETS mq DESTINATION bin)

#try this
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/parser/) 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/multiQ/) 