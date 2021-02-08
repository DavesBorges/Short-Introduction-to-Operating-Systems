/*
*	Author David de Jesus M. Borges
*	Date:	02/04/2021
*
*	This file contains the entry point for the Kernel Simulator
*
*    The command line command should be in the form:
*    "./Kernel-Simulator program1 priorityprogram1 program2 priorityprogram2"
* 
*/


#include <iostream>
#include "Process/Runtime.h"

void print_message(const kernel_clock::time_point& begin);

int main(int argc, char* argv[])
{

    kernel_clock::time_point start;

    execute_all_processes(argc, argv);     
    print_message(start);

    return 0;

}


void print_message(const kernel_clock::time_point& start) {
 
    std::cout << "Finished in " 
			<< duration_cast<seconds>(kernel_clock::now() - start).count() 
			<< " seconds";
    std::cout << "\n -------------------------------\n";

    for (auto& process : finished_processes) {
        auto real_time = duration_cast<seconds>(process.real_time).count();
        std::cout << "Process name " << process.short_name 
				<< ", pid " << process.pid
            << "\n\tPriority Level " << priority_to_int(process.priority)
            << "\n\tcpu time "
			<< duration_cast<seconds>(process.cpu_time).count()<<" seconds\n\t"
			<< "real time " << real_time << " seconds \n\t"
			<< "instructions executed " << process.executed_instructions
            << "\n\tinstructions/second " 
			<< process.executed_instructions /double(real_time)<<'\n'<< '\n';
    }
}
