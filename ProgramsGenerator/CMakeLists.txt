add_executable (ProgramsGenerator ./src/main.cpp)

set(BINARY_PATH "${CMAKE_CURRENT_BINARY_DIR}/bin")

set_target_properties( ProgramsGenerator	PROPERTIES
						VS_DEBUGGER_COMMAND_ARGUMENTS
						"Program X"
						RUNTIME_OUTPUT_DIRECTORY ${BINARY_PATH}
						)

