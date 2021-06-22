cmake_minimum_required(VERSION 3.19.2)
project(Core)

source_group("include" REGULAR_EXPRESSION "include/*")
source_group("src" REGULAR_EXPRESSION "src/*")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(${PROJECT_NAME} SHARED ${CPP_FILES} ${HEADER_FILES})