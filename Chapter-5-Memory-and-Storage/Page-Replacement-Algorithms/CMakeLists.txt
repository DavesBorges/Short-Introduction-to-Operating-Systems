
add_executable(Page-Replacement ./include/Paging_structures.h ./src/Algorithms.h
								./src/Algorithms.cpp ./src/main.cpp
								./src/Cmdline_facilities.h 
								./src/Cmdline_facilities.cpp)

set(BINARY_PATH "${CMAKE_CURRENT_BINARY_DIR}/bin")

set_target_properties( Page-Replacement	PROPERTIES
						RUNTIME_OUTPUT_DIRECTORY ${BINARY_PATH}
						)

target_include_directories(Page-Replacement PUBLIC 
							./include/
							)

