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
    int waitingTime;
};

struct RoundRobinScheduler {
    int currentTime;
    int timeQuantum;
    struct Process* readyQueue;
    int readyQueueSize;
    struct Process* copyQueue;
    int copyQueueSize;
};

void RoundRobin(struct RoundRobinScheduler* scheduler) {
    printf("\n Round Robin Scheduler\n");
    // Run the scheduler
    while (scheduler->readyQueueSize > 0) {
        struct Process currentProcess = scheduler->readyQueue[0];
        if (currentProcess.remainingTime >= scheduler->timeQuantum) {
            currentProcess.remainingTime -= scheduler->timeQuantum;
            scheduler->currentTime += scheduler->timeQuantum;
            printf("Process %d executed for %d units.\n", currentProcess.processId, scheduler->currentTime);
            scheduler->readyQueue[scheduler->readyQueueSize++] = currentProcess;
            if(currentProcess.remainingTime==0){
               scheduler->copyQueue[currentProcess.processId-1].turnaroundTime = scheduler->currentTime - currentProcess.arrivalTime;
               scheduler->copyQueue[currentProcess.processId-1].completionTime = scheduler->currentTime;
            }
            else{

            }
        } else {
            scheduler->currentTime += currentProcess.remainingTime;
            printf("Process %d executed for %d units.\n", currentProcess.processId, scheduler->currentTime);
            currentProcess.remainingTime = 0;
            scheduler->copyQueue[currentProcess.processId-1].completionTime = scheduler->currentTime;
            scheduler->copyQueue[currentProcess.processId-1].turnaroundTime = scheduler->currentTime - currentProcess.arrivalTime;

        }
        // Shift the ready queue
        for (int i = 0; i < scheduler->readyQueueSize - 1; i++) {
            scheduler->readyQueue[i] = scheduler->readyQueue[i + 1];
        }
        scheduler->readyQueueSize--;


    }


    // Print completion time for each process
    printf("\n Completion time\n");
    for (int i = 0; i < scheduler->copyQueueSize ; i++) {
        printf("Process %d completed at time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].completionTime);
    }
    // Print turnaround time for each process
    printf("\n Turnaround time\n");
    for (int i = 0; i < scheduler->copyQueueSize ; i++) {
        printf("Process %d turnaround time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].turnaroundTime);
    }
    // Print waiting time for each process
    printf("\n Waiting time\n");
    for (int i = 0; i < scheduler->copyQueueSize ; i++) {
        printf("Process %d waiting time %d\n", scheduler->copyQueue[i].processId,scheduler->copyQueue[i].turnaroundTime-scheduler->copyQueue[i].burstTime);
    }
    printf("\n******************************************\n");
    //free(scheduler->readyQueue);
    //free(scheduler->copyQueue);
}
void SJF(struct RoundRobinScheduler* scheduler) {
    scheduler->currentTime = 0; // Reset the current time to 0

    struct Process temp;
    for (int i = 0; i < scheduler->copyQueueSize - 1; i++) {
        for (int j = 0; j < scheduler->copyQueueSize - i - 1; j++) {
            if (scheduler->copyQueue[j].burstTime > scheduler->copyQueue[j + 1].burstTime) {
                // Swap
                temp = scheduler->copyQueue[j];
                scheduler->copyQueue[j] = scheduler->copyQueue[j + 1];
                scheduler->copyQueue[j + 1] = temp;
            }
        }
    }

    // Print sorted queue
    printf("\nSJF Queue\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d burst time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].burstTime);
    }

    int start = 0; // Start from time 0
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        struct Process currentProcess = scheduler->copyQueue[i];
        scheduler->currentTime = start;
        int end = scheduler->currentTime + currentProcess.burstTime;
        printf("Process %d start at %d end at %d\n", currentProcess.processId, start, end);
        currentProcess.completionTime = end;
        currentProcess.turnaroundTime = currentProcess.completionTime - currentProcess.arrivalTime;
        currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;
        start = end;
    }

    // Print completion time for each process
    printf("\nCompletion time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d completed at time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].completionTime);
    }

    // Print turnaround time for each process
    printf("\nTurnaround time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d turnaround time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].turnaroundTime);
    }

    // Print waiting time for each process
    printf("\nWaiting time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d waiting time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].waitingTime);
    }

    printf("\n******************************************\n");
}


void Priority(struct RoundRobinScheduler* scheduler) {
    struct Process temp;
    for (int i = 0; i < scheduler->copyQueueSize - 1; i++) {
        for (int j = 0; j < scheduler->copyQueueSize - i - 1; j++) {
            if (scheduler->copyQueue[j].priority > scheduler->copyQueue[j+1].priority) {
                // Swap
                temp = scheduler->copyQueue[j];
                scheduler->copyQueue[j] = scheduler->copyQueue[j+1];
                scheduler->copyQueue[j+1] = temp;
            }
        }
    }
    // Print sorted queue
    printf("\nSorted queue\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d pri %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].priority);
    }

    int start = scheduler->currentTime;
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        struct Process currentProcess = scheduler->copyQueue[i];
        if (currentProcess.arrivalTime > scheduler->currentTime) {
            scheduler->currentTime = currentProcess.arrivalTime;
        }
        scheduler->currentTime += currentProcess.burstTime;
        int end = scheduler->currentTime;
        printf("Process %d start at %d end at %d\n", currentProcess.processId, start, end);
        start = end;
    }
    printf("\n******************************************\n");
}

int main() {
    struct RoundRobinScheduler scheduler;
    scheduler.currentTime = 0;
    scheduler.readyQueueSize = 0;
    scheduler.copyQueueSize = 0;

    // Add processes to the scheduler
    int numP;
    printf("Enter number of Processes: ");
    scanf("%d", &numP);
    printf("Enter Time Quantum: ");
    scanf("%d", &scheduler.timeQuantum);
    scheduler.readyQueue = malloc(sizeof(struct Process) * 10);
    scheduler.copyQueue = malloc(sizeof(struct Process) * 10);

    for (int i = 1; i <= numP; i++) {
        int tA, Bt, pri;
        printf("Enter Arrival Time for Process %d: ", i);
        scanf("%d", &tA);
        printf("Enter Burst Time for Process %d: ", i);
        scanf("%d", &Bt);
        printf("Enter Priority for Process %d: ", i);
        scanf("%d", &pri);

        scheduler.readyQueue[scheduler.readyQueueSize++] = (struct Process){i, tA, Bt, Bt, -1, -1, pri};
        scheduler.copyQueue[scheduler.copyQueueSize++] = (struct Process){i, tA, Bt, Bt, -1, -1, pri};
    }

    Priority(&scheduler);
    RoundRobin(&scheduler);
    SJF(&scheduler);

    free(scheduler.readyQueue);
    free(scheduler.copyQueue);

    return 0;
}
