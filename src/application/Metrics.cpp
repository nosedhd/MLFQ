#include "application/Metrics.hpp"

#include "domain/Process.hpp"
#include <stdexcept>

std::vector<ProcessMetrics> Metrics::calculate(
	const std::vector<Process*>& processes) {
	std::vector<ProcessMetrics> results;
	results.reserve(processes.size());

	for (const Process* process : processes) {
		if (process == nullptr) {
			throw std::invalid_argument("La lista contiene un proceso nulo.");
		}

		if (process->getState() != ProcessState::TERMINATED ||
			!process->getStartTime().has_value() ||
			!process->getFirstResponseTime().has_value() ||
			!process->getFinishTime().has_value()) {
			throw std::invalid_argument(
				"Las metricas requieren procesos terminados.");
		}

		int responseTime =
			process->getFirstResponseTime().value() - process->getArrivalTime();
		int turnaroundTime =
			process->getFinishTime().value() - process->getArrivalTime();
		int waitingTime = turnaroundTime - process->getBurstTime();

		results.push_back({
			process->getPid(),
			process->getArrivalTime(),
			process->getBurstTime(),
			process->getStartTime().value(),
			process->getFinishTime().value(),
			responseTime,
			turnaroundTime,
			waitingTime
		});
	}

	return results;
}
