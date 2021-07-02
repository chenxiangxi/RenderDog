cmake_minimum_required(VERSION 3.19.2)
project(Core)
add_definitions(-DLIBRARY_EXPORTS)

source_group("include" REGULAR_EXPRESSION "include/*")
source_group("src" REGULAR_EXPRESSION "src/*")
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
set(Core_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "Core include directory")

include_directories(${Core_INCLUDE})

add_library(${PROJECT_NAME} SHARED ${CPP_FILES} ${HEADER_FILES})