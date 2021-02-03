#include <iostream>
#include "Process/Runtime.h"

void print_message(const high_resolution_clock::time_point& begin);

int main(int argc, char* argv[])
{
    int result = schedule_args(argc, argv);
    high_resolution_clock::time_point start = high_resolution_clock::now();

    if (result != SCHEDULE_STATUS_OK) {
        std::cerr << " Format " << "<process1> <priority_1> <process2> <priority2>" 
            << " where priority might be 1 2 or 3" << '\n';
        return result;
    }

    execute_tasks();
    
    print_message(start);

    return 0;

}


void print_message(const high_resolution_clock::time_point& start) {
 
    std::cout << "Finished in " << std::chrono::duration_cast<seconds>(high_resolution_clock::now() - start).count() << " seconds";
    std::cout << "\n -------------------------------\n";

    for (auto& process : finished_processes) {
        int real_time = std::chrono::duration_cast<seconds>(process.real_time).count();
        std::cout << "Process name " << process.short_name << ", pid " << process.pid
            << "\n\tPriority Level " << priority_to_int(process.priority)
            << "\n\tcpu time " << std::chrono::duration_cast<seconds>(process.cpu_time).count()
            << " seconds\n\treal time " << real_time
            << " seconds \n\tinstructions executed " << process.executed_instructions
            << "\n\tinstructions/second " << process.executed_instructions / double(real_time) << '\n' << '\n';
    }
}