#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "infrastructure/CsvIO.hpp"

void test_csv_reading() {
    const std::string inputPath = "test_processes.csv";
    {
        std::ofstream input(inputPath);
        input << "PID,Arrival,Burst\n"
              << "P1,0,8\n"
              << "P2, 2, 4\n";
    }

    std::vector<Process> processes = CsvIO::readProcesses(inputPath);

    assert(processes.size() == 2);
    assert(processes[0].getPid() == "P1");
    assert(processes[0].getArrivalTime() == 0);
    assert(processes[0].getBurstTime() == 8);
    assert(processes[1].getPid() == "P2");
    assert(processes[1].getArrivalTime() == 2);
    assert(processes[1].getBurstTime() == 4);

    std::remove(inputPath.c_str());
}

void test_csv_writing() {
    const std::string outputPath = "test_results.csv";
    std::vector<ProcessMetrics> metrics{
        {"P1", 0, 8, 0, 12, 0, 12, 4}
    };

    CsvIO::writeResults(outputPath, metrics);

    std::ifstream output(outputPath);
    std::string content(
        (std::istreambuf_iterator<char>(output)),
        std::istreambuf_iterator<char>());

    assert(content ==
           "PID,Arrival,Burst,Start,Finish,Response,Turnaround,Waiting\n"
           "P1,0,8,0,12,0,12,4\n");

    std::remove(outputPath.c_str());
}

int main() {
    test_csv_reading();
    test_csv_writing();
    std::cout << "All CsvIO tests passed!" << std::endl;
    return 0;
}