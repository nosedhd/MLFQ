#ifndef CSV_IO_HPP
#define CSV_IO_HPP

#include <string>
#include <vector>

#include "application/Metrics.hpp"
#include "domain/Process.hpp"

class CsvIO {
public:
	static std::vector<Process> readProcesses(const std::string& filePath);

	static void writeResults(const std::string& filePath,
							 const std::vector<ProcessMetrics>& metrics);
};

#endif // CSV_IO_HPP
