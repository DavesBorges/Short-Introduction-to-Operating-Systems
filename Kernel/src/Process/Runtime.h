#pragma once

#include "Scheduler.h"

class Application_runtime_error {};

// execute all tasks
void execute_tasks();						

// execute one single process
void run_process(Control_block& c);	

// execute one single process poor performance version
void run_process_slow(Control_block& c);	


struct Command {
    std::string m_operator;
    int operand;
};

std::ostream& operator<<(std::ostream& outstream, const Command& c);
std::istream& operator>>(std::istream& inputstream, Command& c);
