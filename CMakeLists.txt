cmake_minimum_required(VERSION 3.19.2)
project(RenderDog)

set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(OUTPUT_LIB_DIR ${OUTPUT_DIR}/lib CACHE PATH "Output lib directory")
set(OUTPUT_BIN_DIR ${OUTPUT_DIR}/bin CACHE PATH "Output bin directory")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_LIB_DIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(ThirdParty)

add_subdirectory(Core)
add_subdirectory(VulkanRHI)
add_subdirectory(VulkanRHIUnitTest)

add_subdirectory(BIMViewer)