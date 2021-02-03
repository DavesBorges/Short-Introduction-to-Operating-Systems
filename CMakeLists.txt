cmake_minimum_required( VERSION 3.13.0)

project( Short-Intorudction-to-Operating-Systems)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(Kernel)

add_subdirectory(ProgramsGenerator)

set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "Kernel")


