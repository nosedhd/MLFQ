
#include <cassert>
#include <iostream>
#include <vector>

#include "application/Metrics.hpp"
#include "domain/Process.hpp"

void test_metrics_calculation() {
    Process process("P1", 2, 5);

    process.markReady(0);
    process.runOneCycle(4);
    process.runOneCycle(5);
    process.runOneCycle(6);
    process.runOneCycle(7);
    process.runOneCycle(8);

    std::vector<Process*> processes{&process};
    std::vector<ProcessMetrics> results = Metrics::calculate(processes);

    assert(results.size() == 1);
    assert(results[0].pid == "P1");
    assert(results[0].arrivalTime == 2);
    assert(results[0].burstTime == 5);
    assert(results[0].startTime == 4);
    assert(results[0].finishTime == 9);
    assert(results[0].responseTime == 2);
    assert(results[0].turnaroundTime == 7);
    assert(results[0].waitingTime == 2);
}

void test_metrics_rejects_unfinished_process() {
    Process process("P1", 0, 2);

    bool caught = false;
    try {
        Metrics::calculate({&process});
    } catch (const std::invalid_argument&) {
        caught = true;
    }

    assert(caught);
}

int main() {
    test_metrics_calculation();
    test_metrics_rejects_unfinished_process();
    std::cout << "All Metrics tests passed!" << std::endl;
    return 0;
}