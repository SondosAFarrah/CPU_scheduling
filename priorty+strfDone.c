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
    printf("\nPriority Scheduler\n");
    struct Process temp;

    // Sort the processes based on arrival time (ascending order)
    for (int i = 0; i < scheduler->copyQueueSize - 1; i++) {
        for (int j = 0; j < scheduler->copyQueueSize - i - 1; j++) {
            if (scheduler->copyQueue[j].arrivalTime > scheduler->copyQueue[j + 1].arrivalTime) {
                // Swap
                temp = scheduler->copyQueue[j];
                scheduler->copyQueue[j] = scheduler->copyQueue[j + 1];
                scheduler->copyQueue[j + 1] = temp;
            }
        }
    }

    scheduler->currentTime = scheduler->copyQueue[0].arrivalTime;
    int completedProcesses = 0;

    while (completedProcesses < scheduler->copyQueueSize) {
        int highestPriority = 9999;
        int highestPriorityIndex = -1;

        // Find the process with the highest priority
        for (int i = 0; i < scheduler->copyQueueSize; i++) {
            if (scheduler->copyQueue[i].arrivalTime <= scheduler->currentTime && scheduler->copyQueue[i].priority < highestPriority && scheduler->copyQueue[i].remainingTime > 0) {
                highestPriority = scheduler->copyQueue[i].priority;
                highestPriorityIndex = i;
            }
        }

        if (highestPriorityIndex == -1) {
            scheduler->currentTime++;
            continue;
        }

        struct Process* currentProcess = &scheduler->copyQueue[highestPriorityIndex];
        printf("Process %d executed from %d to ", currentProcess->processId, scheduler->currentTime);

        if (currentProcess->remainingTime > 1) {
            scheduler->currentTime++;
            currentProcess->remainingTime--;
        } else {
            scheduler->currentTime += currentProcess->remainingTime;
            currentProcess->remainingTime = 0;
            completedProcesses++;
            currentProcess->completionTime = scheduler->currentTime;
            currentProcess->turnaroundTime = currentProcess->completionTime - currentProcess->arrivalTime;
            currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->burstTime;
        }

        printf("%d\n", scheduler->currentTime);
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

void SRTF(struct SchedulerQueue* scheduler) {
    printf("\nSRTF Scheduler\n");

    // Initialize variables
    int completedProcesses = 0;
    int currentTime = 0;

    while (completedProcesses < scheduler->copyQueueSize) {
        int shortestBurstIndex = -1;
        int shortestBurstTime = INT_MAX;

        // Find the process with the shortest remaining burst time among the arrived processes
        for (int i = 0; i < scheduler->copyQueueSize; i++) {
            if (scheduler->copyQueue[i].arrivalTime <= currentTime && scheduler->copyQueue[i].remainingTime > 0) {
                if (scheduler->copyQueue[i].remainingTime < shortestBurstTime) {
                    shortestBurstIndex = i;
                    shortestBurstTime = scheduler->copyQueue[i].remainingTime;
                }
            }
        }

        if (shortestBurstIndex == -1) {
            currentTime++;
            continue;
        }

        // Execute the process for 1 unit
        printf("Process %d executed for 1 unit.\n", scheduler->copyQueue[shortestBurstIndex].processId);
        scheduler->copyQueue[shortestBurstIndex].remainingTime--;
        currentTime++;

        // Check if the process has completed
        if (scheduler->copyQueue[shortestBurstIndex].remainingTime == 0) {
            completedProcesses++;
            scheduler->copyQueue[shortestBurstIndex].completionTime = currentTime;
            scheduler->copyQueue[shortestBurstIndex].turnaroundTime = scheduler->copyQueue[shortestBurstIndex].completionTime - scheduler->copyQueue[shortestBurstIndex].arrivalTime;
            scheduler->copyQueue[shortestBurstIndex].waitingTime = scheduler->copyQueue[shortestBurstIndex].turnaroundTime - scheduler->copyQueue[shortestBurstIndex].burstTime;
        }
    }

    // Calculate average turnaround time and average waiting time
    double avgTurnaroundTime = 0;
    double avgWaitingTime = 0;

    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        avgTurnaroundTime += scheduler->copyQueue[i].turnaroundTime;
        avgWaitingTime += scheduler->copyQueue[i].waitingTime;
    }

    avgTurnaroundTime /= scheduler->copyQueueSize;
    avgWaitingTime /= scheduler->copyQueueSize;

    // Display completion time, turnaround time, and waiting time for each process
    printf("\nCompletion time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d completed at time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].completionTime);
    }

    printf("\nTurnaround time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d turnaround time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].turnaroundTime);
    }
    printf("Average Turnaround Time = %.2f\n", avgTurnaroundTime);

    printf("\nWaiting time\n");
    for (int i = 0; i < scheduler->copyQueueSize; i++) {
        printf("Process %d waiting time %d\n", scheduler->copyQueue[i].processId, scheduler->copyQueue[i].waitingTime);
    }
    printf("Average Waiting Time = %.2f\n", avgWaitingTime);
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
