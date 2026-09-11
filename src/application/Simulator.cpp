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

// Bucle principal de la simulación. El uso de tiempo discreto imita la naturaleza tick-by-tick del reloj de la CPU de una máquina real.
void Simulator::run() {
	while (!allProcessesTerminated()) {
		tick();
	}
}

// Cada llamada a tick representa un ciclo completo del scheduler del Sistema Operativo.
void Simulator::tick() {
	addArrivingProcesses();

    // Verificación temprana del Priority Boost para mitigar Starvation antes de hacer el scheduling.
	if (currentCycle > 0 && currentCycle % boostInterval == 0) {
		policy.boostAllProcesses();
	}

    // Delegación de la toma de decisiones al componente de política (Strategy).
	Process* process = policy.selectNextProcess();
	if (process != nullptr) {
		process->runOneCycle(currentCycle);

        // Reevaluación del estado del proceso tras consumir CPU. Si demostró ser CPU-bound,
        // se activa el castigo (demotion) en el planificador.
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
