/*
*	Author David de Jesus M. Borges
*	Date:	02/04/2021
*
*	Declaration of (Process) Control_block (PCB) as well as operators overloads
*/

#pragma once

#include <string>
#include <chrono>

typedef std::chrono::high_resolution_clock kernel_clock;
typedef std::chrono::seconds seconds;
typedef std::chrono::milliseconds milliseconds;
using namespace std::chrono;


enum class Priority {
	low, medium, high
};


enum class PState {

	created, ready, running, waiting, terminated, sleeping
};

struct Control_block {
	std::string name;					// name of the file of the process
	std::string short_name;
	unsigned int pid;
	Priority priority{ Priority::medium };
	PState state{ PState::created };
	int readPosition{ 0 };				// position of the cursor in the stream
	kernel_clock::duration cpu_time{ 0 };	// time spent executing commands
	kernel_clock::time_point wake_time;		// time to wake after sleep
	kernel_clock::time_point start_time;
	kernel_clock::duration real_time;		// time since the start_time
	int executed_instructions{ 0 };

	// pid of the process being waited for -1 means not waiting
	int pid_waiting_for{ -1 };			
};

bool operator == (const Control_block& b1, const Control_block& b2);
bool operator <(const Control_block& b1, const Control_block& b2);

Priority int_to_priority(int i);		

int priority_to_int(const Priority& p);

bool operator<(const Priority& p1, const Priority& p2);
