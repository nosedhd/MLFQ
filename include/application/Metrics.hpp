#ifndef METRICS_HPP
#define METRICS_HPP

#include <string>
#include <vector>

class Process;

struct ProcessMetrics {
	std::string pid;
	int arrivalTime;
	int burstTime;
	int startTime;
	int finishTime;
	int responseTime;
	int turnaroundTime;
	int waitingTime;
};

class Metrics {
public:
	static std::vector<ProcessMetrics> calculate(
	const std::vector<Process*>& processes);
};

#endif // METRICS_HPP
