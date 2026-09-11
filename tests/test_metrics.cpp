#include <iostream>
#include <cassert>
#include <vector>
#include "domain/Process.hpp"
#include "application/Metrics.hpp"
#include "application/Simulator.hpp"

void test_metrics_calculation() {
    Process p1("P1", 0, 3);
    Process p2("P2", 1, 2);
    
    std::vector<Process*> procs = {&p1, &p2};
    Simulator sim(procs, 10);
    sim.run(); // Ejecuta hasta terminar
    
    std::vector<ProcessMetrics> metrics = Metrics::calculate(procs);
    assert(metrics.size() == 2);
    
    // p1 metrics: Corre en t=0 y t=1 (Q0), luego demota a Q1. 
    // Corre de nuevo en t=4 (termina).
    assert(metrics[0].pid == "P1");
    assert(metrics[0].responseTime == 0); // 0 - 0 = 0
    assert(metrics[0].turnaroundTime == 5); // 5 - 0 = 5
    assert(metrics[0].waitingTime == 2); // 5 - 3 = 2
    
    // p2 metrics: Llega t=1. Espera a que P1 suelte CPU (t=2).
    // Corre en t=2 y t=3 (termina).
    assert(metrics[1].pid == "P2");
    assert(metrics[1].responseTime == 1); // 2 - 1 = 1
    assert(metrics[1].turnaroundTime == 3); // 4 - 1 = 3
    assert(metrics[1].waitingTime == 1); // 3 - 2 = 1
}

int main() {
    test_metrics_calculation();
    std::cout << "All Metrics tests passed!" << std::endl;
    return 0;
}