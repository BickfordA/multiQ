cmake_minimum_required(VERSION 3.0)

project(MultiQ_Metrics)

add_subdirectory(parser)
add_subdirectory(multiQ)

add_executable( mq MainDriver.cpp)

target_link_libraries(mq csvParser )
target_link_libraries(mq multiQ)

install(TARGETS mq DESTINATION bin)

#try this
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/parser/) 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/multiQ/) 