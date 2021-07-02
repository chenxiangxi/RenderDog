cmake_minimum_required(VERSION 3.19.2)
project(Launcher)

find_package(Qt5 REQUIRED QUIET COMPONENTS Core Widgets Gui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

source_group("include" REGULAR_EXPRESSION "include/*")
source_group("src" REGULAR_EXPRESSION "src/*")
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${BVApplication_INCLUDE})
link_directories(${OUTPUT_LIB_DIR})

add_executable(${PROJECT_NAME} ${CPP_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC VulkanRHI Qt5::Core Qt5::Gui Qt5::Widgets BVApplication)