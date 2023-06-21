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
struct SchedulerQueue {
    int currentTime;
    int timeQuantum;
    struct Process* readyQueue;
    int readyQueueSize;
    struct Process* copyQueue;
    int copyQueueSize;
};

void RoundRobin(struct SchedulerQueue* scheduler) {
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
    int sum=0;
    // Print turnaround time for each process
    printf("\n Turnaround time\n");
    for (int i = 0; i < scheduler->copyQueueSize ; i++) {
        printf("Process %d turnaround time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].turnaroundTime);
    
        sum+=scheduler->copyQueue[i].turnaroundTime;
    }
    printf("Average Turnaround Time = %d\n",sum/scheduler->copyQueueSize);
    sum=0;
    // Print waiting time for each process
    printf("\n Waiting time\n");
    for (int i = 0; i < scheduler->copyQueueSize ; i++) {
        printf("Process %d waiting time %d\n", scheduler->copyQueue[i].processId,scheduler->copyQueue[i].turnaroundTime-scheduler->copyQueue[i].burstTime);
    
        sum+=scheduler->copyQueue[i].turnaroundTime-scheduler->copyQueue[i].burstTime;
    }
    printf("Average Waiting Time = %d",sum/scheduler->copyQueueSize);
    // for(int i =0; i<scheduler->copyQueueSize;i++){
    //     printf("in %d p%d\n",i,scheduler->copyQueue[i].processId);
    // }
    printf("\n******************************************\n");
    
}

void SJF(struct SchedulerQueue* scheduler) {
    scheduler->currentTime = 0; // Reset the current time to 0
    printf("\n SJF Scheduler\n");
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
    // for (int i = 0; i < scheduler->copyQueueSize; i++) {
    //     printf("Process %d burst time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].burstTime);
    // }

    int start = 0; // Start from time 0
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        struct Process currentProcess = scheduler->copyQueue[i];
        scheduler->currentTime = start;
        int end = scheduler->currentTime + currentProcess.burstTime;
        printf("Process %d executed for %d units (start at %d end at %d)\n", currentProcess.processId,currentProcess.burstTime, start, end);
        scheduler->copyQueue[i].completionTime = end;
        scheduler->copyQueue[i].turnaroundTime = scheduler->copyQueue[i].completionTime - scheduler->copyQueue[i].arrivalTime;
        scheduler->copyQueue[i].waitingTime = scheduler->copyQueue[i].turnaroundTime - scheduler->copyQueue[i].burstTime;
        start = end;
    }
    
    // Print completion time for each process
    printf("\nCompletion time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d completed at time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].completionTime);
    }
    
    int sum=0;
    // Print turnaround time for each process
    printf("\nTurnaround time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d turnaround time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].turnaroundTime);
        sum+=scheduler->copyQueue[i].turnaroundTime;
    }
    printf("Average Turnaround Time = %d\n",sum/scheduler->copyQueueSize);
    sum=0;
    // Print waiting time for each process
    printf("\nWaiting time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d waiting time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].waitingTime);
    
        sum+=scheduler->copyQueue[i].waitingTime;
    }
    printf("Average Waiting Time = %d",sum/scheduler->copyQueueSize);

    printf("\n******************************************\n");
}

void Priority(struct SchedulerQueue* scheduler) {
    printf("\n Priority Scheduler\n");
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
    // printf("\n Priority Scheduler\n");
    // for (int i = 0; i < scheduler->copyQueueSize; i++) {
    //     printf("Process %d pri %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].priority);
    // }
    scheduler->currentTime=0;
    int start = scheduler->currentTime;
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        struct Process currentProcess = scheduler->copyQueue[i];
        if (currentProcess.arrivalTime > scheduler->currentTime) {
            scheduler->currentTime = currentProcess.arrivalTime;
        }
        scheduler->currentTime += currentProcess.burstTime;
        int end = scheduler->currentTime;
        printf("Process %d start at %d end at %d\n", currentProcess.processId, start, end);
        scheduler->copyQueue[i].completionTime = end;
        scheduler->copyQueue[i].turnaroundTime = scheduler->copyQueue[i].completionTime - scheduler->copyQueue[i].arrivalTime;
        scheduler->copyQueue[i].waitingTime = scheduler->copyQueue[i].turnaroundTime - scheduler->copyQueue[i].burstTime;
        start = end;
    }
    
    // Print completion time for each process
    printf("\nCompletion time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d completed at time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].completionTime);
    }
    
    int sum=0;
    // Print turnaround time for each process
    printf("\nTurnaround time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d turnaround time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].turnaroundTime);
        sum+=scheduler->copyQueue[i].turnaroundTime;
    }
    printf("Average Turnaround Time = %d\n",sum/scheduler->copyQueueSize);
    sum=0;
    // Print waiting time for each process
    printf("\nWaiting time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d waiting time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].waitingTime);
    
        sum+=scheduler->copyQueue[i].waitingTime;
    }
    printf("Average Waiting Time = %d",sum/scheduler->copyQueueSize);
    printf("\n******************************************\n");
}

void SRTF(struct SchedulerQueue* scheduler) {
    printf("\nSRTF Scheduler\n");

    // Initialize variables
    int completedProcesses = 0;
    int smallestBurstIndex = 0;
    int smallestBurstTime = -1;

    while (completedProcesses < scheduler->copyQueueSize) {
        // Find the process with the smallest remaining burst time
        smallestBurstTime = -1;
        smallestBurstIndex = -1;

        for (int i = 0; i < scheduler->copyQueueSize; i++) {
            if (scheduler->copyQueue[i].arrivalTime <= scheduler->currentTime && scheduler->copyQueue[i].remainingTime > 0) {
                if (smallestBurstTime == -1 || scheduler->copyQueue[i].remainingTime < smallestBurstTime) {
                    smallestBurstTime = scheduler->copyQueue[i].remainingTime;
                    smallestBurstIndex = i;
                }
            }
        }

        if (smallestBurstIndex == -1) {
            scheduler->currentTime++;
            continue;
        }

        // Execute the process
        struct Process* currentProcess = &(scheduler->copyQueue[smallestBurstIndex]);
        currentProcess->remainingTime--;
        scheduler->currentTime++;

        if (currentProcess->remainingTime == 0) {
            completedProcesses++;
            currentProcess->completionTime = scheduler->currentTime;
            currentProcess->turnaroundTime = currentProcess->completionTime - currentProcess->arrivalTime;
            currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->burstTime;
        }

        printf("Process %d executed for 1 unit.\n", currentProcess->processId);
    }

    // Print completion time for each process
    printf("\nCompletion time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d completed at time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].completionTime);
    }

    int sum = 0;
    // Print turnaround time for each process
    printf("\nTurnaround time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d turnaround time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].turnaroundTime);
        sum += scheduler->copyQueue[i].turnaroundTime;
    }
    printf("Average Turnaround Time = %d\n", sum / scheduler->copyQueueSize);
    sum = 0;
    // Print waiting time for each process
    printf("\nWaiting time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d waiting time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].waitingTime);
        sum += scheduler->copyQueue[i].waitingTime;
    }
    printf("Average Waiting Time = %d", sum / scheduler->copyQueueSize);

    printf("\n******************************************\n");
}
int main() {
    struct SchedulerQueue scheduler;
    scheduler.currentTime = 0;
    scheduler.readyQueueSize = 0;
    scheduler.copyQueueSize = 0;

    // Add processes to the scheduler
    int numP;
    printf("Enter number of Processes: ");
    scanf("%d", &numP);
    while(numP<1){
        printf("Number of Processes must be more than 0!!!\n");
        printf("Enter number of Processes: ");
        scanf("%d", &numP);
    }
    printf("Enter Time Quantum: ");
    scanf("%d", &scheduler.timeQuantum);
    while(scheduler.timeQuantum<1){
        printf("Time Quantum must be more than 0!!!\n");
        printf("Enter Time Quantum: ");
        scanf("%d", &scheduler.timeQuantum);
    }
    printf("*********************\n");
    scheduler.readyQueue = malloc(sizeof(struct Process) * 10);
    scheduler.copyQueue = malloc(sizeof(struct Process) * 10);

    for (int i = 1; i <= numP; i++) {
        int tA, Bt, pri;
        printf("\nEnter Arrival Time for Process %d: ", i);
        scanf("%d", &tA);
        printf("Enter Burst Time for Process %d: ", i);
        scanf("%d", &Bt);
        while(Bt<1){
            printf("Burst Time must be more than 0!!!\n");
            printf("Enter Burst Time for Process %d: ", i);
            scanf("%d", &Bt);
        }
        printf("Enter Priority for Process %d: ", i);
        scanf("%d", &pri);
        
        scheduler.readyQueue[scheduler.readyQueueSize++] = (struct Process){i, tA, Bt, Bt, -1, -1, pri};
        scheduler.copyQueue[scheduler.copyQueueSize++] = (struct Process){i, tA, Bt, Bt, -1, -1, pri};
    }

    
    RoundRobin(&scheduler);
    Priority(&scheduler);
    SJF(&scheduler);
    SRTF(&scheduler);
    
    free(scheduler.readyQueue);
    free(scheduler.copyQueue);

    return 0;
}
