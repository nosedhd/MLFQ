#include <iostream>
#include <cassert>
#include <vector>
#include "domain/Process.hpp"
#include "application/Simulator.hpp"

void test_boost_logic() {
    Process p1("P1", 0, 10);
    std::vector<Process*> procs = {&p1};
    
    // Boost cada 3 ciclos
    Simulator sim(procs, 3);
    
    // Ciclo 0: Q0
    sim.tick();
    
    // Ciclo 1: Q0 (termina quantum 2, demota a Q1)
    sim.tick();
    assert(p1.getCurrentQueue() == 1);
    
    // Ciclo 2: Q1
    sim.tick();
    assert(p1.getCurrentQueue() == 1);
    
    // Ciclo 3: Toca Priority Boost! Todos a Q0.
    sim.tick();
    // Como sube a Q0 y se ejecuta, termina en Q0 con 1 quantum usado.
    assert(p1.getCurrentQueue() == 0);
    assert(p1.getQuantumUsed() == 1);
}

int main() {
    test_boost_logic();
    std::cout << "All Boost tests passed!" << std::endl;
    return 0;
}
