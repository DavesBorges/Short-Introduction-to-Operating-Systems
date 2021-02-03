#pragma once

#include <string>
#include <chrono>

typedef std::chrono::high_resolution_clock high_resolution_clock;
typedef std::chrono::seconds seconds;
typedef std::chrono::milliseconds milliseconds;


enum class Priority {
	LOW, MEDIUM, HIGH
};

enum class PState {
	NEW, READY, RUNNING, WAITING, TERMINTATED
};

struct Control_block {
	std::string name;												// name of the file of the process
	std::string short_name;
	unsigned int pid;
	Priority priority{ Priority::MEDIUM };
	PState state{ PState::NEW };
	int readPosition{ 0 };											// position of the cursor in the stream 
	std::chrono::high_resolution_clock::duration cpu_time{ 0 };		// time spent executing commands
	std::chrono::high_resolution_clock::time_point wake_time;		// time to wake after sleep is executed
	std::chrono::high_resolution_clock::time_point start_time;
	std::chrono::high_resolution_clock::duration real_time;			// time since the start_time
	int executed_instructions{ 0 };
	int pid_waiting_for{ -1 };										// pid of the process passed as operand to the wait command						
};

bool operator == (const Control_block& b1, const Control_block& b2);
bool operator <(const Control_block& b1, const Control_block& b2);

Priority int_to_priority(int i);

int priority_to_int(const Priority& p);

bool operator<(const Priority& p1, const Priority& p2);