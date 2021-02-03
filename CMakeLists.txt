cmake_minimum_required( VERSION 3.13.0)

project( Short-Intorudction-to-Operating-Systems)

set(CMAKE_CXX_STANDARD 11)

set(CHAPTER4_PATH "Chapter-4-Processes-and-Thread")


add_subdirectory(${CHAPTER4_PATH})

set_property( DIRECTORY  PROPERTY   VS_STARTUP_PROJECT Kernel-Simulator)
add_subdirectory(ProgramsGenerator)




