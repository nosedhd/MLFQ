#include "domain/Process.hpp"
#include <algorithm> // Para std::min

// --- Constructor con lista de inicialización y validación ---
Process::Process(const std::string& pid, int arrivalTime, int burstTime)
    : pid(pid),
      arrivalTime(arrivalTime),
      burstTime(burstTime),
      remainingTime(burstTime),    // Al inicio, el tiempo restante es el total
      currentQueue(0),             // Empieza en la cola 0 (la más prioritaria)
      quantumUsed(0),
      startTime(std::nullopt),     // Explícitamente "sin valor"
      finishTime(std::nullopt),
      firstResponseTime(std::nullopt),
      state(ProcessState::NEW)     // Estado inicial
{
    // Validaciones del constructor
    if (arrivalTime < 0) {
        throw std::invalid_argument("arrivalTime no puede ser negativo.");
    }
    if (burstTime <= 0) {
        throw std::invalid_argument("burstTime debe ser mayor a 0.");
    }
}

// --- Implementación de Getters (const) ---
const std::string& Process::getPid() const { return pid; }
int Process::getArrivalTime() const { return arrivalTime; }
int Process::getBurstTime() const { return burstTime; }
int Process::getRemainingTime() const { return remainingTime; }
int Process::getCurrentQueue() const { return currentQueue; }
int Process::getQuantumUsed() const { return quantumUsed; }
std::optional<int> Process::getStartTime() const { return startTime; }
std::optional<int> Process::getFinishTime() const { return finishTime; }
std::optional<int> Process::getFirstResponseTime() const { return firstResponseTime; }
ProcessState Process::getState() const { return state; }

// --- Marca el proceso como listo para ejecutarse en un nivel de cola ---
void Process::markReady(int queueLevel) {
    currentQueue = queueLevel;
    state = ProcessState::READY;
    // Nota: Aquí NO reseteamos quantumUsed, porque se resetea al subir de prioridad (boost)
}

// --- Simula 1 ciclo de CPU (1 unidad de tiempo) ---
void Process::runOneCycle(int currentCycle) {
    // Si es la primera vez que se ejecuta (startTime no tiene valor), lo asignamos.
    if (!startTime.has_value()) {
        startTime = currentCycle;
    }
    // Si es la primera vez que obtiene CPU (firstResponseTime no tiene valor), lo asignamos.
    if (!firstResponseTime.has_value()) {
        firstResponseTime = currentCycle;
    }

    // Cambiamos el estado a RUNNING
    state = ProcessState::RUNNING;

    // Ejecutamos 1 ciclo: reducimos el tiempo restante y aumentamos el quantum usado
    remainingTime--;
    quantumUsed++;

    // Si el proceso terminó su ráfaga
    if (remainingTime == 0) {
        state = ProcessState::TERMINATED;
        finishTime = currentCycle; // Guardamos cuando terminó
    }
}

// --- Degrada el proceso a una cola de menor prioridad (número más alto) ---
void Process::demote(int maxQueueLevel) {
    // Subimos un nivel (ej: de 0 a 1, de 1 a 2), pero sin pasarnos del máximo.
    currentQueue = std::min(currentQueue + 1, maxQueueLevel);
    // Si no ha terminado, queda en READY esperando su próximo turno.
    // Reseteamos el quantum usado porque empieza desde 0 en su nueva cola.
    quantumUsed = 0;
    if (state != ProcessState::TERMINATED) {
        state = ProcessState::READY;
    }
}

// --- Sube el proceso a la cola de máxima prioridad (boost) ---
void Process::boostToTop() {
    currentQueue = 0;      // Cola más prioritaria
    quantumUsed = 0;       // El quantum se reinicia al subir
    if (state != ProcessState::TERMINATED) {
        state = ProcessState::READY;
    }
}