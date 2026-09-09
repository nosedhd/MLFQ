#include <cassert>
#include <iostream>

#include "domain/Process.hpp"
#include "domain/SchedulingPolicy.hpp"

void test_policy_starts_empty() {
    SchedulingPolicy policy;

    assert(!policy.hasReadyProcess());
    assert(policy.selectNextProcess() == nullptr);
}

void test_policy_selects_highest_priority() {
    Process lowPriority("P_LOW", 0, 3);
    Process mediumPriority("P_MEDIUM", 0, 3);
    Process highPriority("P_HIGH", 0, 3);
    SchedulingPolicy policy;

    lowPriority.markReady(2);
    mediumPriority.markReady(1);
    highPriority.markReady(0);

    policy.addProcess(&lowPriority);
    policy.addProcess(&mediumPriority);
    policy.addProcess(&highPriority);

    assert(policy.selectNextProcess() == &highPriority);
    assert(policy.selectNextProcess() == &mediumPriority);
    assert(policy.selectNextProcess() == &lowPriority);
    assert(!policy.hasReadyProcess());
}

void test_policy_preserves_fifo_order() {
    Process first("P1", 0, 3);
    Process second("P2", 0, 3);
    SchedulingPolicy policy;

    first.markReady(1);
    second.markReady(1);

    policy.addProcess(&first);
    policy.addProcess(&second);

    assert(policy.selectNextProcess() == &first);
    assert(policy.selectNextProcess() == &second);
}

void test_policy_demotes_and_requeues_process() {
    Process process("P1", 0, 6);
    SchedulingPolicy policy;

    process.markReady(0);
    policy.addProcess(&process);
    assert(policy.selectNextProcess() == &process);

    policy.demoteProcess(&process);

    assert(process.getCurrentQueue() == 1);
    assert(process.getState() == ProcessState::READY);
    assert(policy.selectNextProcess() == &process);
}

void test_policy_does_not_demote_beyond_last_queue() {
    Process process("P1", 0, 6);
    SchedulingPolicy policy;

    process.markReady(2);
    policy.demoteProcess(&process);

    assert(process.getCurrentQueue() == 2);
    assert(policy.selectNextProcess() == &process);
}

int main() {
    test_policy_starts_empty();
    test_policy_selects_highest_priority();
    test_policy_preserves_fifo_order();
    test_policy_demotes_and_requeues_process();
    test_policy_does_not_demote_beyond_last_queue();
    std::cout << "All SchedulingPolicy tests passed!" << std::endl;
    return 0;
}