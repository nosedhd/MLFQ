#include <iostream>
#include <cassert>
#include "domain/Process.hpp"

void test_process_lifecycle() {
    Process p1("P001", 0, 10);
    assert(p1.getPid() == "P001");
    assert(p1.getState() == ProcessState::NEW);

    p1.markReady(0);
    assert(p1.getCurrentQueue() == 0);
    assert(p1.getState() == ProcessState::READY);

    for (int cycle = 0; cycle < 3; ++cycle) {
        p1.runOneCycle(cycle);
    }
    assert(p1.getRemainingTime() == 7);
    assert(p1.getQuantumUsed() == 3);
    assert(p1.getStartTime().value() == 0);
    assert(p1.getFirstResponseTime().value() == 0);
    
    p1.demote(3);
    assert(p1.getCurrentQueue() == 1);
    assert(p1.getState() == ProcessState::READY);
    assert(p1.getQuantumUsed() == 0);

    p1.boostToTop();
    assert(p1.getCurrentQueue() == 0);
    assert(p1.getState() == ProcessState::READY);
    assert(p1.getQuantumUsed() == 0);
}

void test_process_finish_time() {
    Process corto("PSHORT", 0, 1);
    corto.markReady(0);
    corto.runOneCycle(0);
    assert(corto.getFinishTime().value() == 1);
    assert(corto.getState() == ProcessState::TERMINATED);
}

void test_process_validation() {
    bool caught = false;
    try {
        Process p2("P002", -5, 10);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
}

int main() {
    test_process_lifecycle();
    test_process_finish_time();
    test_process_validation();
    std::cout << "All Process tests passed!" << std::endl;
    return 0;
}
