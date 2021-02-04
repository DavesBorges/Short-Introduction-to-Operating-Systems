/*
*	Author David de Jesus M. Borges
*	Date:	02/04/2021
*
*	This file contains the declarations of the 
*	Kernel Runtime interface including error type Application_runtime_error
*/

#pragma once

#include "Scheduler.h"

class Application_runtime_error {};

// execute all tasks until they are finished or an error occurs
void execute_all_processes(int argc, char* argv[]);						

