#include <stdio.h>
#include <stdlib.h>

int main() {
    struct RoundRobinScheduler {
        int currentTime;
        int timeQuantum;
        struct Process* readyQueue;
        int readyQueueSize;
        struct Process* copyQueue;
        int copyQueueSize;
    };

    struct Process {
        int processId;
        int arrivalTime;
        int burstTime;
        int remainingTime;
        int completionTime;
        int turnaroundTime;
    };

    struct RoundRobinScheduler scheduler;
    scheduler.currentTime = 0;
    scheduler.timeQuantum = 4;
    scheduler.readyQueueSize = 0;
    scheduler.readyQueue = malloc(sizeof(struct Process) * 10);
    scheduler.copyQueueSize=0;
    scheduler.copyQueue= malloc(sizeof(struct Process) * 10);

    // Add processes to the scheduler
    scheduler.readyQueue[scheduler.readyQueueSize++] = (struct Process){1, 0, 24, 24, -1, -1};
    scheduler.readyQueue[scheduler.readyQueueSize++] = (struct Process){2, 2, 3, 3, -1, -1};
    scheduler.readyQueue[scheduler.readyQueueSize++] = (struct Process){3, 4, 3, 3, -1, -1};
    int queueSize = scheduler.readyQueueSize;
    scheduler.copyQueue[scheduler.copyQueueSize++] = (struct Process){1, 0, 24, 24, -1, -1};
    scheduler.copyQueue[scheduler.copyQueueSize++] = (struct Process){2, 2, 3, 3, -1, -1};
    scheduler.copyQueue[scheduler.copyQueueSize++] = (struct Process){3, 4, 3, 3, -1, -1};
    
    
    // Run the scheduler
    int Comtime=0, j=0;
    while (scheduler.readyQueueSize > 0) {
        struct Process currentProcess = scheduler.readyQueue[j];
        if (currentProcess.remainingTime >= scheduler.timeQuantum) {
            currentProcess.remainingTime -= scheduler.timeQuantum;
            scheduler.currentTime += scheduler.timeQuantum;
            printf("Process %d executed for %d units.\n", currentProcess.processId, scheduler.currentTime);
            scheduler.readyQueue[scheduler.readyQueueSize++] = currentProcess;
            if(currentProcess.remainingTime==0){
               scheduler.copyQueue[currentProcess.processId-1].turnaroundTime = scheduler.currentTime - currentProcess.arrivalTime;
               scheduler.copyQueue[currentProcess.processId-1].completionTime = scheduler.currentTime;
            }
            else{
                
            }
        } else {
            scheduler.currentTime += currentProcess.remainingTime;
            printf("Process %d executed for %d units.\n", currentProcess.processId, scheduler.currentTime);
            currentProcess.remainingTime = 0;
            scheduler.copyQueue[currentProcess.processId-1].completionTime = scheduler.currentTime;
            scheduler.copyQueue[currentProcess.processId-1].turnaroundTime = scheduler.currentTime - currentProcess.arrivalTime;
            
        }
        // Shift the ready queue
        for (int i = 0; i < scheduler.readyQueueSize - 1; i++) {
            scheduler.readyQueue[i] = scheduler.readyQueue[i + 1];
        }
        scheduler.readyQueueSize--;

        
    }
    

    // Print completion time for each process
    printf("\n Completion time\n");
    for (int i = 0; i < scheduler.copyQueueSize ; i++) {
        printf("Process %d completed at time %d\n", scheduler.copyQueue[i].processId, scheduler.copyQueue[i].completionTime);
    }
    // Print turnaround time for each process
    printf("\n Turnaround time\n");
    for (int i = 0; i < scheduler.copyQueueSize ; i++) {
        printf("Process %d turnaround time %d\n", scheduler.copyQueue[i].processId, scheduler.copyQueue[i].turnaroundTime);
    }
    // Print waiting time for each process
    printf("\n Waiting time\n");
    for (int i = 0; i < scheduler.copyQueueSize ; i++) {
        printf("Process %d waiting time %d\n", scheduler.copyQueue[i].processId,scheduler.copyQueue[i].turnaroundTime-scheduler.copyQueue[i].burstTime);
    }
    free(scheduler.readyQueue);
    free(scheduler.copyQueue);
    return 0;
}