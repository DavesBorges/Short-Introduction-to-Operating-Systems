/*
*	Author David de Jesus M. Borges
*	Date:	02/04/2021
*
*	This file contains the declarations of functions responsible for:
*       - Exec
*       - Executing Process
*       - Running Kernel Panic in case of error
*       - Call the Scheduler::re_schedule() function
*/


#ifndef SCHEDULER_BODY_GUARD
#define SCHEDULER_BODY_GUARD

#include <vector>
#include <deque>
#include "Control_block.h" 



namespace Scheduler {
    constexpr int schedule_status_ok = 0;


    struct Sleeping_process {
        Control_block control_block;
        kernel_clock::time_point wake_time;    // time of wake
    };

    

    /// <summary>
    ///  Represents a single entry of the wating table.
    /// When the process whose pid == Waiting_entry::pid every process 
    /// of waiting gets awaked
    /// </summary>
    struct Waiting_entry {
        int pid;                                // pid of process awaited
        std::vector<Control_block> waiting;     // processes waiting for pid
    };

    

    /// <summary>
    /// check if is time to wake the sleeping processes 
    /// </summary>
    void check_sleeping_processes();

    void re_schedule();




    int schedule_args(int argc, char* argv[]);

    bool operator < (const Waiting_entry& entry1, const Waiting_entry& entry2);

    bool operator==(const Waiting_entry& entry1, const Waiting_entry& entry2);

}

extern std::vector<Scheduler::Waiting_entry> waiting_table;
extern std::vector<Scheduler::Sleeping_process> sleeping_processes; 
extern std::deque<Control_block> running_processes;   // Running or ready
extern int start_pid;
extern std::vector<Control_block> finished_processes; // Terminated

#endif