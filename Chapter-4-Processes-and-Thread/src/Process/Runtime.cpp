
/*
*	Author David de Jesus M. Borges
*	Date:	02/04/2021
*
*	This file contains the definition of fucntions responsible for:
*       - Switching context
*       - Executing Process
*       - Running Kernel Panic in case of error
*       - Call the Scheduler::re_schedule() function
*/

#include <fstream>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <algorithm>
#include "Runtime.h"
#include "Command/Command.h"



std::string get_log_message(const Command& c) {
    std::ostringstream ostr;
    ostr << "Executing" << '\t' << c.m_operator << '\t'
        << "on" << '\t' << c.operand;
    return ostr.str();
}

void run_process(Control_block& cb);
void run_kernel_panic();

bool should_run() 
{
    return !running_processes.empty() ||
        !sleeping_processes.empty() || !waiting_table.empty();
}

void execute_all_processes(int argc, char* argv[]) {
    // First schedule the programs passed as parameters and then
    // execute all tasks until process is fiinished or error occurs
    //The command line arguments should be in the form:
    //"./Kernel-Simulator program1 priorityprogram1 program2 priorityprogram2"

    int result = Scheduler::schedule_args(argc, argv);
    kernel_clock::time_point start = kernel_clock::now();

    if (result != Scheduler::schedule_status_ok) {
        std::cerr << " Format "
            << "<process1> <priority_1> <process2> <priority2>"
            << " where priority might be 1 2 or 3" << '\n';

        run_kernel_panic();
        return;
    }

    while (should_run()) {
        if (!running_processes.empty()) {
            Control_block& control_block = running_processes.front();
            std::cout << "Switching to " << control_block.short_name << '\n';
            if (control_block.state == PState::sleeping) {
                std::cout << "PROCESS " << control_block.short_name 
                            << " awaked" << '\n';
            }

            if (control_block.state == PState::waiting) {
                std::cout << "Process " << control_block.short_name
                    << " can now continue ";
            }
            try {
                run_process(control_block);
            }
            catch (...) {
                run_kernel_panic();
                break;
            }
        }
        Scheduler::re_schedule();

    }
}

bool check_deadlock(int pid_waiting, int pid_waiting_for);

bool process_finished(int pid);

void run_kernel_panic() {
    for (auto& cb : running_processes) {
        cb.real_time = kernel_clock::now() - cb.start_time;
        cb.state = PState::terminated;
        finished_processes.push_back(cb);
    }

    for (auto& entry : sleeping_processes) {
        auto& cb = entry.control_block;
        cb.real_time = kernel_clock::now() - cb.start_time;
        cb.state = PState::terminated;
        finished_processes.push_back(cb);
    }

    for (auto& entry : waiting_table) {
        for (auto& cb : entry.waiting) {
            cb.real_time = kernel_clock::now() - cb.start_time;
            cb.state = PState::terminated;
            finished_processes.push_back(cb);
        }
    }
    sleeping_processes.clear();
    running_processes.clear();
    waiting_table.clear();
}
void run_process(Control_block& control_block) {

    // reopen the ifstream and place the cursor at the correct position
    std::ifstream ifs{ control_block.name };
    ifs.seekg(control_block.readPosition);

    if (control_block.state == PState::created) {
        control_block.start_time = kernel_clock::now();
    }
    kernel_clock::time_point begin = kernel_clock::now();

    // Awake the process and increment the executed instructions
    PState cb_state = control_block.state;
    if (cb_state == PState::waiting || cb_state == PState::sleeping)
        control_block.executed_instructions++;
    control_block.state = PState::running;

    // Number of instructions to execute is proportional to the priority level
    int max = priority_to_int(control_block.priority) * 3;     

    for (int i = 0; i < max && ifs.good(); ++i) {
        Command current_command;
        ifs >> current_command;

        if (ifs.bad() || ifs.fail()) {
            break;
        }

        std::cout << '\t' << get_log_message(current_command) << '\n';
        
        // this is needed so we can have time to look at the printed commands 
        std::this_thread::sleep_for(milliseconds(300));     

        if (current_command.m_operator == "sleep") {
            control_block.state = PState::sleeping;
            kernel_clock::time_point now = kernel_clock::now();
            control_block.wake_time = now + seconds(current_command.operand);
            break;
        }

       
        if (current_command.m_operator == "ret") {   
            // end the process if ret is found
            control_block.state = PState::terminated;
            control_block.executed_instructions++;
            control_block.cpu_time += kernel_clock::now() - begin;
            return;
        }

        if (current_command.m_operator == "fork") {
            Control_block child = control_block;
            child.pid = ++start_pid;
            child.readPosition = ifs.tellg();
            running_processes.push_back(child);

            std::cout << " Forked process " << control_block.short_name 
                << " (child pid " << child.pid << '\n';
        }

        if (current_command.m_operator == "wait") {
            //  check if the process already finished

            if (process_finished(current_command.operand)) {
                control_block.executed_instructions++;
                std::cout << "\tprocess pid -> " << current_command.operand 
                    << " already finished" << '\n';
            }
            else {
                if (check_deadlock(control_block.pid,current_command.operand)){
                    std::cerr << "\t Deadlock found " 
                        << " process " << control_block.short_name << '\n';
                    throw Application_runtime_error();
                }

                control_block.state = PState::waiting;
                control_block.pid_waiting_for = current_command.operand;
                std::cout << "\tWaiting for process " 
                          << " pid -> " << current_command.operand << '\n';
                break;
            }
        }
        control_block.executed_instructions++;
    }
    if (ifs.good() && !ifs.eof()) {
        control_block.readPosition = ifs.tellg();
    }
    else {
        control_block.state = PState::terminated;
    }
    control_block.cpu_time += kernel_clock::now() - begin;
}

bool check_deadlock(int pid_waiting, int pid_waiting_for) {

    if (pid_waiting == pid_waiting_for)
        return true;

    return false;
}

bool process_finished(int pid) {
    for (auto& process : finished_processes) {
        if (pid == process.pid)
            return true;
    }
    return false;
}


void run_process_slow(Control_block& control_block) {
    std::ifstream ifs{ control_block.name };
    ifs.seekg(control_block.readPosition);
    kernel_clock::time_point begin = kernel_clock::now();

    control_block.state = PState::running;

    for (int i = 0; i < 3 && ifs.good(); ++i) {
        Command currentCommand;

        ifs >> currentCommand;
        if (ifs.bad() || ifs.fail()) {
            break;
        }

        std::cout << '\t' << get_log_message(currentCommand) << '\n';
        std::this_thread::sleep_for(milliseconds(250));
        if (currentCommand.m_operator == "sleep") {

            // block the all execution of the "system" until process awake
            std::this_thread::sleep_for(seconds(currentCommand.operand));
        }
        if (currentCommand.m_operator == "ret") {
            control_block.state = PState::terminated;
            return;
        }
    }
    if (ifs.good() && !ifs.eof()) {
        control_block.readPosition = ifs.tellg();
    }
    else {
        control_block.state = PState::terminated;
    }
}
