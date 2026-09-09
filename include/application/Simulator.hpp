#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <vector>

#include "domain/SchedulingPolicy.hpp"

class Process;

class Simulator {
private:
	int currentCycle;
	int boostInterval;
	SchedulingPolicy policy;
	std::vector<Process*> processes;

	void addArrivingProcesses();
	bool allProcessesTerminated() const;

public:
	explicit Simulator(const std::vector<Process*>& processes,
					   int boostInterval = 20);

	void run();
	void tick();

	int getCurrentCycle() const;
};

#endif // SIMULATOR_HPP
