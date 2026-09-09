#include "domain/Queue.hpp"
#include "domain/Process.hpp"
#include <stdexcept>

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