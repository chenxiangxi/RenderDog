cmake_minimum_required(VERSION 3.19.2)
project(VulkanRHIUnitTest)

source_group("include" REGULAR_EXPRESSION "include/*")
source_group("src" REGULAR_EXPRESSION "src/*")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include  ${VULKAN_PATH}/Include ${VulkanRHI_INCLUDE} ${Core_INCLUDE})
link_directories(${OUTPUT_LIB_DIR})

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_executable(${PROJECT_NAME} ${CPP_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC gtest VulkanRHI)