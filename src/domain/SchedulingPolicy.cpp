#include "domain/SchedulingPolicy.hpp"

#include "domain/Process.hpp"
#include <stdexcept>
#include <vector>

// Patrón Strategy: Esta clase encapsula estrictamente las reglas de selección de MLFQ.
// Si a futuro se requiere cambiar a un planificador FCFS, el Simulator permanecería intacto.
SchedulingPolicy::SchedulingPolicy()
	: queues{Queue(0), Queue(1), Queue(2)} {
}

void SchedulingPolicy::addProcess(Process* process) {
	if (process == nullptr) {
		throw std::invalid_argument("No se puede agregar un proceso nulo.");
	}

	int queueLevel = process->getCurrentQueue();
	if (queueLevel < 0 || queueLevel > MAX_QUEUE_LEVEL) {
		throw std::out_of_range("Nivel de cola invalido.");
	}

	queues[queueLevel].enqueue(process);
}

// Principio de estricta prioridad de MLFQ: Siempre se favorece a los procesos en colas superiores.
// Solo si Q0 está vacía, se revisa Q1, previniendo que procesos batch retrasen a los interactivos.
Process* SchedulingPolicy::selectNextProcess() {
	for (Queue& queue : queues) {
		if (!queue.isEmpty()) {
			return queue.dequeue();
		}
	}

	return nullptr;
}

// Detecta el comportamiento del proceso basado en el consumo de su tajada de tiempo (Quantum).
bool SchedulingPolicy::hasUsedFullQuantum(const Process* process) const {
	if (process == nullptr) {
		return false;
	}

	int queueLevel = process->getCurrentQueue();
	if (queueLevel < 0 || queueLevel > MAX_QUEUE_LEVEL) {
		return false;
	}

	return process->getQuantumUsed() >= queues[queueLevel].getQuantumMaximo();
}

// Implementa la penalidad estructural de MLFQ:
// Un proceso que excede su quantum revela ser CPU-bound. Se le relega a una cola de menor prioridad.
void SchedulingPolicy::demoteProcess(Process* process) {
	if (process == nullptr || process->getState() == ProcessState::TERMINATED) {
		return;
	}

	process->demote(MAX_QUEUE_LEVEL);
	addProcess(process);
}

// Implementación del Priority Boost periódico:
// Extrae de raíz todos los procesos de todas las colas inferiores y los reubica en la cola de máxima prioridad.
// Esto purga cualquier historial negativo, curando la inanición (starvation) en el sistema.
void SchedulingPolicy::boostAllProcesses() {
	std::vector<Process*> readyProcesses;

	for (Queue& queue : queues) {
		while (!queue.isEmpty()) {
			readyProcesses.push_back(queue.dequeue());
		}
	}

	for (Process* process : readyProcesses) {
		process->boostToTop();
		queues[0].enqueue(process);
	}
}

bool SchedulingPolicy::hasReadyProcess() const {
	// TODO: puede sustituirse por una comprobación más eficiente si hace falta.
	for (const Queue& queue : queues) {
		if (!queue.isEmpty()) {
			return true;
		}
	}

	return false;
}
