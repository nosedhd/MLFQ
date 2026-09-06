#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <optional>
#include <stdexcept> // Para las excepciones

// Enumeración fuerte (scoped enum)
enum class ProcessState {
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
};

class Process {
private:
    // --- Fijos (se setean en el constructor y nunca cambian) ---
    std::string pid;
    int arrivalTime;
    int burstTime;

    // --- Mutables (cambian durante la simulación) ---
    int remainingTime;
    int currentQueue;
    int quantumUsed;

    // --- "Sin valor todavía" (usamos std::optional) ---
    std::optional<int> startTime;
    std::optional<int> finishTime;
    std::optional<int> firstResponseTime;

    // --- Estado actual ---
    ProcessState state;

public:
    // Constructor con validación (lanza excepción si los datos son inválidos)
    Process(const std::string& pid, int arrivalTime, int burstTime);

    // --- Getters (TODOS son const, no modifican el objeto) ---
    const std::string& getPid() const;
    int getArrivalTime() const;
    int getBurstTime() const;
    int getRemainingTime() const;
    int getCurrentQueue() const;
    int getQuantumUsed() const;
    std::optional<int> getStartTime() const;
    std::optional<int> getFinishTime() const;
    std::optional<int> getFirstResponseTime() const;
    ProcessState getState() const;

    // --- Métodos de simulación (modifican el estado interno) ---
    void markReady(int queueLevel);
    void runOneCycle(int currentCycle);
    void demote(int maxQueueLevel);
    void boostToTop();
};

#endif // PROCESS_HPP