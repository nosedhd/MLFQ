#include <iostream>
#include <vector>

#include "application/Metrics.hpp"
#include "application/Simulator.hpp"
#include "domain/Process.hpp"
#include "infrastructure/CsvIO.hpp"

int main() {
    try {
        std::vector<Process> processes =
            CsvIO::readProcesses("input.csv");

        std::vector<Process*> processPointers;
        processPointers.reserve(processes.size());
        for (Process& process : processes) {
            processPointers.push_back(&process);
        }

        Simulator simulator(processPointers, 20);
        simulator.run();

        std::vector<ProcessMetrics> metrics =
            Metrics::calculate(processPointers);
        CsvIO::writeResults("results.csv", metrics);
    } catch (const std::exception& exception) {
        std::cerr << "Error: " << exception.what() << std::endl;
        return 1;
    }

    return 0;
}