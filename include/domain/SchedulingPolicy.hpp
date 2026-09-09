#ifndef SCHEDULING_POLICY_HPP
#define SCHEDULING_POLICY_HPP

#include <array>

#include "domain/Queue.hpp"

class Process;

class SchedulingPolicy {
private:
	static constexpr int NUM_QUEUES = 3;
	static constexpr int MAX_QUEUE_LEVEL = NUM_QUEUES - 1;

	std::array<Queue, NUM_QUEUES> queues;

public:
	SchedulingPolicy();

	void addProcess(Process* process);
	Process* selectNextProcess();
	bool hasUsedFullQuantum(const Process* process) const;
	void demoteProcess(Process* process);
	void boostAllProcesses();
	bool hasReadyProcess() const;
};

#endif // SCHEDULING_POLICY_HPP
