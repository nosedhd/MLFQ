#include "domain/Queue.hpp"
#include "domain/Process.hpp"
#include <stdexcept>

// La asignación de quantums inversamente proporcionales a la prioridad es clave en MLFQ:
// Q0 (alta prioridad) tiene quantum corto (2) para tareas interactivas que necesitan respuesta rápida.
// Q2 (baja prioridad) tiene quantum largo (8) para tareas CPU-bound que requieren procesar mucho volumen.
Queue::Queue(int nivel)
    : nivel(nivel),
      quantum_maximo(0) {
    if (nivel == 0) {
        quantum_maximo = 2;
    } else if (nivel == 1) {
        quantum_maximo = 4;
    } else if (nivel == 2) {
        quantum_maximo = 8;
    } else {
        throw std::invalid_argument("Nivel de cola invalido");
    }
}

int Queue::getNivel() const {
    return nivel;
}

int Queue::getQuantumMaximo() const {
    return quantum_maximo;
}

// Usamos std::queue con punteros (Process*) en lugar de valores (Process) para evitar
// la sobrecarga de copiar el PCB entero (lo cual sería ineficiente y desincronizaría los estados).
// Las operaciones de encolar y desencolar se mantienen en O(1).
void Queue::enqueue(Process* p) {
    procesos.push(p);
}

Process* Queue::dequeue() {
    if (procesos.empty()) {
        return nullptr;
    }

    Process* proceso = procesos.front();
    procesos.pop();

    return proceso;
}

Process* Queue::peek() {
    if (procesos.empty()) {
        return nullptr;
    }

    return procesos.front();
}

bool Queue::isEmpty() const {
    return procesos.empty();
}