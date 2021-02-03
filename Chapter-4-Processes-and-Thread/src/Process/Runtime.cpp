#include <fstream>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <algorithm>
#include "Runtime.h"


struct Waiting_node {
    int m_pid;
    Waiting_node* waiting_for{ nullptr };
};

std::vector<Waiting_node> waiting_graph;

std::string get_log_message(const Command& c) {
    std::ostringstream ostr;
    ostr << "Executing" << '\t' << c.m_operator << '\t'
        << "on" << '\t' << c.operand;
    return ostr.str();
}

void run_kernel_panic();

void execute_tasks() {

    waiting_graph.reserve(running_processes.size() * 2);

    while (!running_processes.empty() || !sleeping_processes.empty() || !waiting_table.empty()) {
        if (!running_processes.empty()) {
            Control_block& control_block = running_processes.front();
            std::cout << "Switching to " << control_block.short_name << '\n';
            if (control_block.state == PState::WAITING) {
                std::cout << "PROCESS " << control_block.short_name << " awaked" << '\n';
            }
            try {
                run_process(control_block);
            }
            catch (const Application_runtime_error& error) {
                run_kernel_panic();
                break;
            }
        }
        re_schedule();

    }
}

bool check_deadlock(int pid_waiting, int pid_waiting_for);
void terminate_deadlock(int pid_waiting, int pid_waiting_for);

void run_kernel_panic() {
    for (auto& cb : running_processes) {
        cb.real_time = high_resolution_clock::now() - cb.start_time;
        cb.state = PState::TERMINTATED;
        finished_processes.push_back(cb);
    }

    for (auto& entry : sleeping_processes) {
        auto& cb = entry.control_block;
        cb.real_time = high_resolution_clock::now() - cb.start_time;
        cb.state = PState::TERMINTATED;
        finished_processes.push_back(cb);
    }

    for (auto& entry : waiting_table) {
        for (auto& cb : entry.waiting) {
            cb.real_time = high_resolution_clock::now() - cb.start_time;
            cb.state = PState::TERMINTATED;
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

    if (control_block.state == PState::NEW) {
        control_block.start_time = high_resolution_clock::now();
    }
    high_resolution_clock::time_point begin = high_resolution_clock::now();

    // Awake the process and increment the executed instructions
    if (control_block.state == PState::WAITING)
        control_block.executed_instructions++;
    control_block.state = PState::RUNNING;


    int max = priority_to_int(control_block.priority) * 3;     // Number of instructions to execute is proportional to the priority level

    for (int i = 0; i < max && ifs.good(); ++i) {
        Command currentCommand;
        ifs >> currentCommand;

        if (ifs.bad() || ifs.fail()) {
            break;
        }

        std::cout << '\t' << get_log_message(currentCommand) << '\n';
        std::this_thread::sleep_for(milliseconds(300));             // this is needed so we can have time to look at the commands being printed
        if (currentCommand.m_operator == "sleep") {
            control_block.state = PState::WAITING;
            control_block.wake_time = high_resolution_clock::now() + seconds(currentCommand.operand);
            break;
        }

        if (currentCommand.m_operator == "ret") {                   // end the process if ret is found
            control_block.state = PState::TERMINTATED;
            control_block.executed_instructions++;
            control_block.cpu_time += high_resolution_clock::now() - begin;
            return;
        }

        if (currentCommand.m_operator == "fork") {
            Control_block child = control_block;
            child.pid = ++start_pid;
            child.readPosition = ifs.tellg();
            running_processes.push_back(child);

            std::cout << " Forked process " << control_block.short_name << " (child pid " << child.pid << '\n';
        }

        if (currentCommand.m_operator == "wait") {
            //  check if the process already finished

            auto it = std::find_if(finished_processes.begin(), finished_processes.end(),
                [&currentCommand](const Control_block& cb) {return cb.pid == currentCommand.operand; });
            if (it != finished_processes.end())
            {
                control_block.executed_instructions++;
                std::cout << "\tprocess " << it->name << " pid -> " << it->pid << " already finished" << '\n';

            }
            else {
                if (check_deadlock(control_block.pid, currentCommand.operand)) {
                    std::cerr << "\t Deadlock found " << " process " << control_block.short_name << '\n';
                    throw Application_runtime_error();
                }

                auto it = std::find_if(waiting_graph.begin(), waiting_graph.end(),
                    [currentCommand](const Waiting_node& node) {return node.m_pid == currentCommand.operand; });

                if (it != waiting_graph.end()) {

                }
                Waiting_node n;
                n.m_pid = control_block.pid;
                Waiting_node waiting_for;

                waiting_for.m_pid = currentCommand.operand;


                waiting_graph.push_back(waiting_for);
                n.waiting_for = &waiting_graph[waiting_graph.size() - 1];
                waiting_graph.push_back(n);
                control_block.state = PState::WAITING;
                control_block.pid_waiting_for = currentCommand.operand;
                std::cout << "\tWaiting for process " << " pid -> " << currentCommand.operand << '\n';
                break;
            }
        }
        control_block.executed_instructions++;
    }
    if (ifs.good() && !ifs.eof()) {
        control_block.readPosition = ifs.tellg();
    }
    else {
        control_block.state = PState::TERMINTATED;
    }
    control_block.cpu_time += high_resolution_clock::now() - begin;
}

bool check_deadlock(int pid_waiting, int pid_waiting_for) {
    bool keep_looking;
    if (pid_waiting == pid_waiting_for)
        return true;

    auto we = waiting_graph.begin();
    int pid_looking  = pid_waiting_for;
    Waiting_node* it = waiting_graph.data();
    for (it; it != nullptr && it != waiting_graph.data() + waiting_graph.size(); ++it) {
        if (it->m_pid == pid_looking) {
            Waiting_node* n = it->waiting_for;
            while (n != nullptr) {
                pid_looking = n->m_pid;
                if (n->m_pid == pid_waiting) {
                    return true;
                }
                n = n->waiting_for;
            }
        }
    }
    return false;
}

void terminate_deadlock(int pid_waiting, int pid_waiting_for) {
}
void run_process_slow(Control_block& control_block) {
    std::ifstream ifs{ control_block.name };
    ifs.seekg(control_block.readPosition);
    high_resolution_clock::time_point begin = high_resolution_clock::now();

    control_block.state = PState::RUNNING;

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
            control_block.state = PState::TERMINTATED;
            return;
        }
    }
    if (ifs.good() && !ifs.eof()) {
        control_block.readPosition = ifs.tellg();
    }
    else {
        control_block.state = PState::TERMINTATED;
    }
}

std::ostream& operator<<(std::ostream& outstream, const Command& c) {
    return outstream << c.m_operator << " " << c.operand;
}

std::istream& operator>>(std::istream& inputstream, Command& c) {
    return inputstream >> c.m_operator >> c.operand;
}
