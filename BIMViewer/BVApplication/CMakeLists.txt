cmake_minimum_required(VERSION 3.19.2)
project(BVApplication)

add_definitions(-DLIBRARY_EXPORTS)

find_package(Qt5 REQUIRED QUIET COMPONENTS Core Widgets Gui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

source_group("include" REGULAR_EXPRESSION "include/*")
source_group("src" REGULAR_EXPRESSION "src/*")

set(BVApplication_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "BVApplication include directory")
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

include_directories(${Core_INCLUDE} ${VulkanRHI_INCLUDE} ${BVApplication_INCLUDE} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
link_directories(${OUTPUT_LIB_DIR})


add_library(${PROJECT_NAME} SHARED ${CPP_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} Core VulkanRHI Qt5::Core Qt5::Gui Qt5::Widgets)