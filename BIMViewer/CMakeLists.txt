cmake_minimum_required(VERSION 3.19.2)
project(BIMViewer)

add_subdirectory(Launcher)
set_property(TARGET Launcher PROPERTY FOLDER "BIMViewer")

add_subdirectory(BVApplication)
set_property(TARGET BVApplication PROPERTY FOLDER "BIMViewer")