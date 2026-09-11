#include <iostream>
#include <cassert>
#include <vector>
#include "domain/Process.hpp"
#include "domain/SchedulingPolicy.hpp"
#include "application/Simulator.hpp"

void test_demotion_logic() {
    Process p1("P1", 0, 10);
    std::vector<Process*> procs = {&p1};
    
    // Boost de 100 para que no interfiera
    Simulator sim(procs, 100);
    
    // Ciclo 0: Q0
    sim.tick();
    assert(p1.getCurrentQueue() == 0);
    assert(p1.getQuantumUsed() == 1);
    
    // Ciclo 1: Completa quantum Q0 (2). Debe ser degradado a Q1.
    sim.tick();
    assert(p1.getCurrentQueue() == 1);
    assert(p1.getQuantumUsed() == 0); // quantum reseteado al cambiar de cola
    
    // Ciclo 2: Corre en Q1
    sim.tick();
    assert(p1.getCurrentQueue() == 1);
    assert(p1.getQuantumUsed() == 1);
}

int main() {
    test_demotion_logic();
    std::cout << "All Demotion tests passed!" << std::endl;
    return 0;
}
