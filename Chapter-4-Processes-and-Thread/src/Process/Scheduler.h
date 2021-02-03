#ifndef SCHEDULER_BODY_GUARD
#define SCHEDULER_BODY_GUARD

#include <vector>
#include <deque>
#include "Control_block.h"


constexpr int SCHEDULE_STATUS_OK = 0;


struct Waiting_process {
    Control_block control_block;                       
    bool waiting_for_process{ false };              // waiting for process(true) or sleeping (false)
    high_resolution_clock::time_point wake_time;    // time of wake
};

extern std::vector<Waiting_process> sleeping_processes;

/// <summary>
///  Represents a single entry of the wating table. when the process whose pid == Waiting_entry::pid every process of waiting gets awaked
/// </summary>
/// 
struct Waiting_entry {
    int pid;                                     // pid of process that is being waited       
    std::vector<Control_block> waiting;          // processes waiting for pid
};

extern std::vector<Waiting_entry> waiting_table;

extern std::deque<Control_block> running_processes;     // queue of processes that are running or ready to run
extern int start_pid;
extern std::vector<Control_block> finished_processes;

/// <summary>
/// check if is time to wake the sleeping processes 
/// </summary>
void check_sleeping_processes();                  

void re_schedule();                          




int schedule_args(int argc, char* argv[]);

bool operator < (const Waiting_entry& entry1, const Waiting_entry& entry2);

bool operator==(const Waiting_entry& entry1, const Waiting_entry& entry2);



#endif