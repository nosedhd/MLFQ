#include "application/Simulator.hpp"

#include "domain/Process.hpp"
#include <stdexcept>

Simulator::Simulator(const std::vector<Process*>& processes,
					 int boostInterval)
	: currentCycle(0),
	  boostInterval(boostInterval),
	  processes(processes) {
	if (boostInterval <= 0) {
		throw std::invalid_argument("El intervalo de boost debe ser mayor que cero.");
	}
}

void Simulator::run() {
	while (!allProcessesTerminated()) {
		tick();
	}
}

void Simulator::tick() {
	addArrivingProcesses();

	if (currentCycle > 0 && currentCycle % boostInterval == 0) {
		policy.boostAllProcesses();
	}

	Process* process = policy.selectNextProcess();
	if (process != nullptr) {
		process->runOneCycle(currentCycle);

		if (process->getState() != ProcessState::TERMINATED) {
			if (policy.hasUsedFullQuantum(process)) {
				policy.demoteProcess(process);
			} else {
				policy.addProcess(process);
			}
		}
	}

	++currentCycle;
}

int Simulator::getCurrentCycle() const {
	return currentCycle;
}

void Simulator::addArrivingProcesses() {
	for (Process* process : processes) {
		if (process->getState() == ProcessState::NEW &&
			process->getArrivalTime() == currentCycle) {
			process->markReady(0);
			policy.addProcess(process);
		}
	}
}

bool Simulator::allProcessesTerminated() const {
	for (const Process* process : processes) {
		if (process->getState() != ProcessState::TERMINATED) {
			return false;
		}
	}

	return true;
}
