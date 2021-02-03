// ProgramsGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include "../../Chapter-4(Processes-and-Thread)/src/Process/Runtime.h"

bool seed{ true };

inline int rand_int(int min, int max) {
    static std::default_random_engine ran;

    if (seed) {
        std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
        auto duration = begin.time_since_epoch();
        auto epoch = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        ran.seed(epoch.count());
        seed = false;
    }
  
    return std::uniform_int_distribution<>{min, max -1}(ran);
}

std::string operators[7]{ "load", "add", "store", "jsr", "wait", "fork", "sleep" };

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Missing program name" << '\n';
        return -1;
    }

    std::ofstream outFile{ argv[1] };

    if (!outFile) {
        std::cerr << "Could not open " << argv[1] << '\n';
        return -2;
    }

    for (int i = 0; i < 19; ++i) {
        Command c;
        c.m_operator = operators[rand_int(0, 6)];
        c.operand = rand_int(0, 5679);
        std::string tabs = "\t";
        if (c.m_operator.size() < 4) {
            tabs += "\t";
        }
        outFile << c.m_operator << tabs << c.operand << '\n';
    }
    outFile << "ret" << "\t\t" << 0 << '\n';
}
