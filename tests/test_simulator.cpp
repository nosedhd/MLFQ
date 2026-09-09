#include <cassert>
#include <iostream>
#include <vector>

#include "application/Simulator.hpp"
#include "domain/Process.hpp"

void test_simulator_waits_for_arrivals() {
    Process process("P1", 2, 1);
    Simulator simulator({&process}, 20);

    simulator.run();

    assert(process.getState() == ProcessState::TERMINATED);
    assert(process.getStartTime().value() == 2);
    assert(process.getFinishTime().value() == 3);
    assert(simulator.getCurrentCycle() == 3);
}

void test_simulator_applies_priority_boost() {
    Process process("P1", 0, 3);
    Simulator simulator({&process}, 2);

    simulator.run();

    assert(process.getState() == ProcessState::TERMINATED);
    assert(process.getCurrentQueue() == 0);
    assert(process.getFinishTime().value() == 3);
}

void test_simulator_handles_multiple_processes() {
    Process first("P1", 0, 2);
    Process second("P2", 0, 1);
    std::vector<Process*> processes{&first, &second};
    Simulator simulator(processes);

    simulator.run();

    assert(first.getState() == ProcessState::TERMINATED);
    assert(second.getState() == ProcessState::TERMINATED);
    assert(first.getFinishTime().has_value());
    assert(second.getFinishTime().has_value());
    assert(simulator.getCurrentCycle() == 3);
}

int main() {
    test_simulator_waits_for_arrivals();
    test_simulator_applies_priority_boost();
    test_simulator_handles_multiple_processes();
    std::cout << "All Simulator tests passed!" << std::endl;
    return 0;
}