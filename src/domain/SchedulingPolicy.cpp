#include "domain/SchedulingPolicy.hpp"

#include "domain/Process.hpp"
#include <stdexcept>
#include <vector>

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

Process* SchedulingPolicy::selectNextProcess() {
	// TODO: recorrer las colas desde la de mayor prioridad hasta la menor.
	for (Queue& queue : queues) {
		if (!queue.isEmpty()) {
			return queue.dequeue();
		}
	}

	return nullptr;
}

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

void SchedulingPolicy::demoteProcess(Process* process) {
	if (process == nullptr || process->getState() == ProcessState::TERMINATED) {
		return;
	}

	process->demote(MAX_QUEUE_LEVEL);
	addProcess(process);
}

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
