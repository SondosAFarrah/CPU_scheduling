#include <stdio.h>
#include <stdlib.h>

struct Process {
    int processId;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int turnaroundTime;
    int priority;
};

struct RoundRobinScheduler {
    int currentTime;
    int timeQuantum;
    struct Process* readyQueue;
    int readyQueueSize;
    struct Process* copyQueue;
    int copyQueueSize;
};

void addProcess(struct RoundRobinScheduler* scheduler, int processId, int arrivalTime, int burstTime) {
    scheduler->readyQueue[scheduler->readyQueueSize++] = (struct Process) {processId, arrivalTime, burstTime, burstTime, -1, -1};
    scheduler->copyQueue[scheduler->copyQueueSize++] = (struct Process) {processId, arrivalTime, burstTime, burstTime, -1, -1};
}

void executeRoundRobin(struct RoundRobinScheduler* scheduler) {
    int j = 0;
    while (scheduler->readyQueueSize > 0) {
        struct Process currentProcess = scheduler->readyQueue[j];
        if (currentProcess.remainingTime >= scheduler->timeQuantum) {
            currentProcess.remainingTime -= scheduler->timeQuantum;
            scheduler->currentTime += scheduler->timeQuantum;
            printf("Process %d executed for %d units.\n", currentProcess.processId, scheduler->currentTime);
            scheduler->readyQueue[scheduler->readyQueueSize++] = currentProcess;
            if (currentProcess.remainingTime == 0) {
                scheduler->copyQueue[currentProcess.processId - 1].turnaroundTime = scheduler->currentTime - currentProcess.arrivalTime;
                scheduler->copyQueue[currentProcess.processId - 1].completionTime = scheduler->currentTime;
            }
        } else {
            scheduler->currentTime += currentProcess.remainingTime;
            printf("Process %d executed for %d units.\n", currentProcess.processId, scheduler->currentTime);
            currentProcess.remainingTime = 0;
            TurnArround_CompletionTime(scheduler, currentProcess);

        }
        for (int i = 0; i < scheduler->readyQueueSize - 1; i++) {
            scheduler->readyQueue[i] = scheduler->readyQueue[i + 1];
        }
        scheduler->readyQueueSize--;
    }
}
void TurnArround_CompletionTime(struct RoundRobinScheduler scheduler, struct Process currentProcess){
    scheduler.copyQueue[currentProcess.processId - 1].completionTime = scheduler.currentTime;
    scheduler.copyQueue[currentProcess.processId - 1].turnaroundTime = scheduler.currentTime - currentProcess.arrivalTime;
}

void printCompletionTime(struct RoundRobinScheduler scheduler) {
    printf("\n Completion time\n");
    for (int i = 0; i < scheduler.copyQueueSize; i++) {
        printf("Process %d completed at time %d\n", scheduler.copyQueue[i].processId, scheduler.copyQueue[i].completionTime);
    }
}

void printTurnaroundTime(struct RoundRobinScheduler scheduler) {
    printf("\n Turnaround time\n");
    for (int i = 0; i < scheduler.copyQueueSize; i++) {
        printf("Process %d turnaround time %d\n", scheduler.copyQueue[i].processId, scheduler.copyQueue[i].turnaroundTime);
    }
}

void printWaitingTime(struct RoundRobinScheduler scheduler) {
    printf("\n Waiting time\n");
    for (int i = 0; i < scheduler.copyQueueSize; i++) {
        printf("Process %d waiting time %d\n", scheduler.copyQueue[i].processId, scheduler.copyQueue[i].turnaroundTime - scheduler.copyQueue[i].burstTime);
    }
}

int main() {
    struct RoundRobinScheduler scheduler;
    scheduler.currentTime = 0;
    scheduler.timeQuantum = 4;
    scheduler.readyQueueSize = 0;
    scheduler.readyQueue = malloc(sizeof(struct Process) * 10);
    scheduler.copyQueueSize = 0;
    scheduler.copyQueue = malloc(sizeof(struct Process) * 10);

    // Add processes to the scheduler
    addProcess(&scheduler, 1, 0, 24);
    addProcess(&scheduler, 2, 2, 3);
    addProcess(&scheduler, 3, 4, 3);

    // Run the scheduler
    executeRoundRobin(&scheduler);

    // Print results
    printCompletionTime(scheduler);
    printTurnaroundTime(scheduler);
    printWaitingTime(scheduler);

    // Free memory
    free(scheduler.readyQueue);
    free(scheduler.copyQueue);

    return 0;
}

