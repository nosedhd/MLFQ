#include <cassert>
#include <iostream>

#include "domain/Process.hpp"
#include "domain/Queue.hpp"

void test_queue_properties() {
    Queue highPriority(0);
    Queue mediumPriority(1);
    Queue lowPriority(2);

    assert(highPriority.getNivel() == 0);
    assert(highPriority.getQuantumMaximo() == 2);
    assert(mediumPriority.getQuantumMaximo() == 4);
    assert(lowPriority.getQuantumMaximo() == 8);
}

void test_queue_fifo_operations() {
    Process first("P1", 0, 3);
    Process second("P2", 1, 2);
    Queue queue(0);

    assert(queue.isEmpty());
    assert(queue.peek() == nullptr);
    assert(queue.dequeue() == nullptr);

    queue.enqueue(&first);
    queue.enqueue(&second);

    assert(!queue.isEmpty());
    assert(queue.peek() == &first);
    assert(!queue.isEmpty());

    assert(queue.dequeue() == &first);
    assert(queue.peek() == &second);
    assert(queue.dequeue() == &second);
    assert(queue.isEmpty());
}

int main() {
    test_queue_properties();
    test_queue_fifo_operations();
    std::cout << "All Queue tests passed!" << std::endl;
    return 0;
}