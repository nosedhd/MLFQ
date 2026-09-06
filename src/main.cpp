#include <iostream>
#include "domain/Process.hpp"

int main() {
    // Intentamos crear un proceso válido
    try {
        Process corto("PSHORT", 0, 1);
        corto.markReady(0);
        corto.runOneCycle(0);
        std::cout << "Finish: " << corto.getFinishTime().value() << std::endl; // ¿debería ser 1?

        Process p1("P001", 0, 10); // Llega en tiempo 0, necesita 10 ciclos
        
        std::cout << "Proceso creado: " << p1.getPid() << std::endl;
        std::cout << "Estado inicial: " << static_cast<int>(p1.getState()) << " (NEW)" << std::endl;

        // Simulamos que llega a la cola 0 y se ejecuta
        p1.markReady(0);
        
        // Ejecutamos 3 ciclos (simulamos currentCycle = 0, 1, 2)
        for (int cycle = 0; cycle < 3; ++cycle) {
            p1.runOneCycle(cycle);
            std::cout << "Ciclo " << cycle << ": Remaining=" << p1.getRemainingTime() 
                      << ", Quantum=" << p1.getQuantumUsed() << std::endl;
        }

        // Verificamos los optional (¿tienen valor?)
        if (p1.getStartTime().has_value()) {
            std::cout << "Inicio en ciclo: " << p1.getStartTime().value() << std::endl;
        }
        if (p1.getFirstResponseTime().has_value()) {
            std::cout << "Primera respuesta en: " << p1.getFirstResponseTime().value() << std::endl;
        }

        // Probamos degradación
        p1.demote(3); // Máximo nivel 3
        std::cout << "Degradado a cola: " << p1.getCurrentQueue() << std::endl;

        // Probamos Boost
        p1.boostToTop();
        std::cout << "Boost a cola: " << p1.getCurrentQueue() << std::endl;

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error al crear el proceso: " << e.what() << std::endl;
    }

    // Probamos que el constructor lanza excepción con datos malos
    try {
        Process p2("P002", -5, 10); // ArrivalTime negativo -> explota
    } catch (const std::invalid_argument& e) {
        std::cerr << "Capturado error esperado: " << e.what() << std::endl;
    }

    return 0;
}