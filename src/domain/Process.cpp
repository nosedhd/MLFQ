#include "domain/Process.hpp"
#include <algorithm>

// Aplicamos el principio Fail-Fast: validamos los datos del PCB al momento de su creación para evitar que el motor de simulación trabaje con procesos malos o tiempos imposibles.
Process::Process(const std::string& pid, int arrivalTime, int burstTime)
    : pid(pid),
      arrivalTime(arrivalTime),
      burstTime(burstTime),
      remainingTime(burstTime),
      currentQueue(0),
      quantumUsed(0),
      startTime(std::nullopt),     // std::optional representa fielmente la ausencia de un evento en el tiempo
      finishTime(std::nullopt),
      firstResponseTime(std::nullopt),
      state(ProcessState::NEW)
{
    if (arrivalTime < 0) {
        throw std::invalid_argument("arrivalTime no puede ser negativo.");
    }
    if (burstTime <= 0) {
        throw std::invalid_argument("burstTime debe ser mayor a 0.");
    }
}

// Retornar referencias const garantiza el principio de inmutabilidad otras capas pueden leer el estado del PCB sin molestarlo.
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

// Transición en la máquina de estados hacia READY.
// El quantum usado no se resetea aquí para preservar la historia de ejecución del proceso
// a menos que sea explícitamente demovido o reciba un boost.
void Process::markReady(int queueLevel) {
    currentQueue = queueLevel;
    state = ProcessState::READY;
}

// Simulación del "Timer Interrupt" del hardware. En lugar de resolver el tiempo matemáticamente,
// la simulación discreta permite interrupciones exactas y cálculo preciso de métricas dinámicas.
void Process::runOneCycle(int currentCycle) {
    if (!startTime.has_value()) {
        startTime = currentCycle;
    }
    if (!firstResponseTime.has_value()) {
        firstResponseTime = currentCycle;
    }

    state = ProcessState::RUNNING;
    remainingTime--;
    quantumUsed++;

    // Transición final de la máquina de estados. 
    // Sumamos 1 porque el ciclo de CPU finaliza al concluir esta unidad de tiempo discreto.
    if (remainingTime == 0) {
        state = ProcessState::TERMINATED;
        finishTime = currentCycle + 1;
    }
}

// Penalización MLFQ para procesos CPU-bound. Si un proceso agota su quantum, asume que es
// intensivo en CPU y se baja su prioridad para no perjudicar la interactividad del sistema.
void Process::demote(int maxQueueLevel) {
    currentQueue = std::min(currentQueue + 1, maxQueueLevel);
    quantumUsed = 0;
    
    if (state != ProcessState::TERMINATED) {
        state = ProcessState::READY;
    }
}

// Mecanismo Anti-Starvation. Los procesos que han caído a colas inferiores debido a demociones
// son subidos a la máxima prioridad periódicamente para garantizar que no sufran inanición.
void Process::boostToTop() {
    currentQueue = 0;
    quantumUsed = 0;
    
    if (state != ProcessState::TERMINATED) {
        state = ProcessState::READY;
    }
}