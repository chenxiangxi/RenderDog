cmake_minimum_required(VERSION 3.19.2)
project(ThirdParty)
add_subdirectory(googletest)

set_property(TARGET gtest PROPERTY FOLDER "ThirdParty")
set_property(TARGET gmock PROPERTY FOLDER "ThirdParty")
set_property(TARGET gmock_main PROPERTY FOLDER "ThirdParty")
set_property(TARGET gtest_main PROPERTY FOLDER "ThirdParty")