#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <queue>

class Process;

class Queue {
private:
	std::queue<Process*> procesos;
	int nivel;
	int quantum_maximo;

public:
	Queue(int nivel);

    int getNivel() const;
    int getQuantumMaximo() const;

	void enqueue(Process* p);
	Process* dequeue();
	Process* peek();
	bool isEmpty() const;
};

#endif // QUEUE_HPP
