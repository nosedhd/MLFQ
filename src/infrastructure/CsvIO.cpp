#include "infrastructure/CsvIO.hpp"

#include "domain/Process.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {

std::string trim(const std::string& value) {
	const std::size_t first = value.find_first_not_of(" \t\r\n");
	if (first == std::string::npos) {
		return "";
	}

	const std::size_t last = value.find_last_not_of(" \t\r\n");
	return value.substr(first, last - first + 1);
}

bool isHeader(const std::string& line) {
	std::stringstream stream(line);
	std::string firstColumn;
	std::getline(stream, firstColumn, ',');
	return trim(firstColumn) == "PID";
}

Process parseProcess(const std::string& line, int lineNumber) {
	std::stringstream stream(line);
	std::string pid;
	std::string arrivalText;
	std::string burstText;
	std::string extraColumn;

	if (!std::getline(stream, pid, ',') ||
		!std::getline(stream, arrivalText, ',') ||
		!std::getline(stream, burstText, ',') ||
		std::getline(stream, extraColumn, ',')) {
		throw std::invalid_argument(
			"CSV invalido en la linea " + std::to_string(lineNumber) +
			". Se esperaba: PID,Arrival,Burst.");
	}

	pid = trim(pid);
	arrivalText = trim(arrivalText);
	burstText = trim(burstText);

	if (pid.empty() || arrivalText.empty() || burstText.empty()) {
		throw std::invalid_argument(
			"CSV invalido en la linea " + std::to_string(lineNumber) +
			". Ningun campo puede estar vacio.");
	}

	try {
		std::size_t arrivalCharacters = 0;
		std::size_t burstCharacters = 0;
		int arrivalTime = std::stoi(arrivalText, &arrivalCharacters);
		int burstTime = std::stoi(burstText, &burstCharacters);

		if (arrivalCharacters != arrivalText.size() ||
			burstCharacters != burstText.size()) {
			throw std::invalid_argument("valor numerico incompleto");
		}

		return Process(pid, arrivalTime, burstTime);
	} catch (const std::exception& exception) {
		throw std::invalid_argument(
			"CSV invalido en la linea " + std::to_string(lineNumber) +
			": " + exception.what());
	}
}

} // namespace

std::vector<Process> CsvIO::readProcesses(const std::string& filePath) {
	std::ifstream input(filePath);
	if (!input.is_open()) {
		throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
	}

	std::vector<Process> processes;
	std::string line;
	int lineNumber = 0;

	while (std::getline(input, line)) {
		++lineNumber;
		if (trim(line).empty()) {
			continue;
		}

		if (processes.empty() && isHeader(line)) {
			continue;
		}

		processes.push_back(parseProcess(line, lineNumber));
	}

	return processes;
}

void CsvIO::writeResults(const std::string& filePath,
						 const std::vector<ProcessMetrics>& metrics) {
	std::ofstream output(filePath);
	if (!output.is_open()) {
		throw std::runtime_error("No se pudo crear el archivo: " + filePath);
	}

	output << "PID,Arrival,Burst,Start,Finish,Response,Turnaround,Waiting\n";
	for (const ProcessMetrics& result : metrics) {
		output << result.pid << ','
			   << result.arrivalTime << ','
			   << result.burstTime << ','
			   << result.startTime << ','
			   << result.finishTime << ','
			   << result.responseTime << ','
			   << result.turnaroundTime << ','
			   << result.waitingTime << '\n';
	}

	if (!output) {
		throw std::runtime_error("Error al escribir el archivo: " + filePath);
	}
}
