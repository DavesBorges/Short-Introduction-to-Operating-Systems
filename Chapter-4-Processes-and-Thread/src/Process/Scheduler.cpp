#include <fstream>
#include <iostream>
#include <algorithm>
#include "Scheduler.h"


using namespace std;

std::deque<Control_block> running_processes;
int start_pid = 0;

std::vector<Control_block> finished_processes;

std::vector<Waiting_process> sleeping_processes;

std::vector<Waiting_entry> waiting_table;

bool operator==(const Waiting_entry& entry1, const Waiting_entry& entry2) {
    return entry1.pid == entry2.pid;
}

bool operator < (const Waiting_entry& entry1, const Waiting_entry& entry2) {
    return entry1.pid < entry2.pid;
}

void check_sleeping_processes() {
    for (int i = 0; i < sleeping_processes.size(); ++i) {
        Waiting_process& waiting_process = sleeping_processes[i];
        if (waiting_process.wake_time <= high_resolution_clock::now()) {
            running_processes.push_back(waiting_process.control_block);
            sleeping_processes.erase(sleeping_processes.begin() + i);
        }
        else {
            break;
        }
    }
}

void schedule_last_executed_process();

void re_schedule() {
    if (!running_processes.empty()) {
        schedule_last_executed_process();
    }
    check_sleeping_processes();
    
}

void schedule_last_executed_process() {
    Control_block& control_block = running_processes.front();
    switch (control_block.state) {
    case PState::TERMINTATED: {
        int pid = control_block.pid;
        control_block.real_time = high_resolution_clock::now() - control_block.start_time;
        Waiting_entry we;
        we.pid = pid;
        std::vector<Waiting_entry>::iterator it = find(waiting_table.begin(), waiting_table.end(), we);
        if (it != waiting_table.end()) {
            for (int i = 0; i < it->waiting.size(); ++i) {

                Control_block block = it->waiting[i];
                block.pid_waiting_for = -1;
                running_processes.push_back(block);
            }
            waiting_table.erase(it);
        }
    }
                            std::cout << "\nProcess " << control_block.short_name
                                << " (pid " << control_block.pid << ") has finished. "
                                << "Priority Level -> " << priority_to_int(control_block.priority)
                                << " Cpu time -> "
                                << std::chrono::duration_cast<milliseconds>(control_block.cpu_time).count()
                                << '\n' << '\n';
                            finished_processes.push_back(control_block);
                            break;

    case PState::WAITING: {
        if (control_block.pid_waiting_for == -1) {
            Waiting_process process;
            process.control_block = control_block;
            process.wake_time = control_block.wake_time;

            sleeping_processes.push_back(process);
            std::sort(sleeping_processes.begin(), sleeping_processes.end(),
                [](const Waiting_process p1, const Waiting_process p2)
                {
                    if (!p1.waiting_for_process && !p2.waiting_for_process)
                        return p1.wake_time < p2.wake_time;
                    if (!p1.waiting_for_process)
                        return true;
                    if (p1.waiting_for_process)
                        return false;

                    else {
                        return priority_to_int(p1.control_block.priority) > priority_to_int(p2.control_block.priority);
                    }
                });
            break;
        }
        int pid = control_block.pid_waiting_for;
        Waiting_entry we;
        we.pid = pid;
        std::vector<Waiting_entry>::iterator entry = find(waiting_table.begin(), waiting_table.end(), we);

        if (entry != waiting_table.end()) {
            entry->waiting.push_back(control_block);

        }
        else {
            Waiting_entry entry;
            entry.pid = pid;
            entry.waiting.push_back(control_block);
            waiting_table.push_back(entry);
        }

        break;
    }

    default:
        running_processes.push_back(control_block);
    }
    running_processes.pop_front();
}


int schedule_args(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "No program name" << '\n';
        return -1;
    }

    for (int i = 1; i < argc; ++i) {
        std::cout << "Loading " << argv[i] << " of priority level " << atoi(argv[i + 1]) << '\n';
        if (!std::ifstream{ argv[i] }) {
            std::cerr << "Could not load " << argv[i] << '\n';
            return -1;
        }
        if (find_if(running_processes.begin(), running_processes.end(),
            [argv, i](const Control_block& p1) {return p1.name == argv[i]; }) != running_processes.end()) {

            std::cerr << " Process " << argv[i] << " already running" << '\n';
            return -1;
        }
        Control_block control_block;
        control_block.pid = ++start_pid;
        control_block.name = argv[i];

        //get the index of the first character after the last '/' occurence
        int j = control_block.name.size();
        while (j >= 0) {
            if (control_block.name[j] == '/') {
                j++;
                break;
            }
            --j;
        }
        control_block.short_name = control_block.name.substr(j);
        control_block.state = PState::NEW;
        int priority = atoi(argv[++i]);
        control_block.priority = int_to_priority(priority);
        running_processes.push_back(control_block);

    }
    return 0;
}

