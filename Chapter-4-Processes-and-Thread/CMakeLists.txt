add_executable( Kernel-Simulator ./src/main.cpp  	./src/Process/Control_block.h
				./src/Process/Control_block.cpp 	./src/Process/Runtime.h
				./src/Process/Runtime.cpp	 	 	./src/Process/Scheduler.cpp
				./src/Process/Scheduler.h			./src/Command/Command.h
				)
set(PROGRAM_PATH "${CMAKE_CURRENT_SOURCE_DIR}/pseudo_programs/")
set(BINARY_PATH "${CMAKE_CURRENT_BINARY_DIR}/bin/")

set_target_properties( Kernel-Simulator PROPERTIES
						VS_DEBUGGER_COMMAND_ARGUMENTS 
						"${PROGRAM_PATH}\"Program A\" \"1\" ${PROGRAM_PATH}\"Program B\" \"2\" ${PROGRAM_PATH}\"Program C\" \"3\""
					RUNTIME_OUTPUT_DIRECTORY ${BINARY_PATH})

target_include_directories(Kernel-Simulator PUBLIC ./src)
