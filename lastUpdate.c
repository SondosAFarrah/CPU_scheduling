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
    struct Process* Processes;
    struct Process* copyQueue;
    struct Process* copyQueue2;
    struct Process* copyQueue3;
    struct Process* copyQueue4;
    struct Process* copyQueue5;

    int ProcessesSize;
    int copyQueueSize;
    int copyQueueSize2;
    int copyQueueSize3;
    int copyQueueSize4;
    int copyQueueSize5;


};
double turn [4];
double waiting[4];

void RoundRobin(struct SchedulerQueue* scheduler) {
    int start=0;
    printf("\n Round Robin Scheduler\n");
    // Run the scheduler
    while (scheduler->readyQueueSize > 0) {

        struct Process currentProcess = scheduler->readyQueue[0];
        if(scheduler->currentTime<currentProcess.arrivalTime){
            printf("idel form %d to %d\n", scheduler->currentTime,currentProcess.arrivalTime);
            scheduler->currentTime=currentProcess.arrivalTime;

        }
        if (currentProcess.remainingTime >= scheduler->timeQuantum) {
            currentProcess.remainingTime -= scheduler->timeQuantum;
            start=scheduler->currentTime;
            scheduler->currentTime += scheduler->timeQuantum;
            int end=scheduler->currentTime;
            printf("Process %d executed form %d to %d\n", currentProcess.processId, start,end);
            if(currentProcess.remainingTime==0){
                scheduler->copyQueue[currentProcess.processId-1].completionTime = scheduler->currentTime;
                scheduler->copyQueue[currentProcess.processId-1].turnaroundTime = scheduler->currentTime - currentProcess.arrivalTime;
            }
            else{
                scheduler->readyQueue[scheduler->readyQueueSize++] = currentProcess;
            }
        } else {
            start=scheduler->currentTime;
            scheduler->currentTime += currentProcess.remainingTime;
            int end=scheduler->currentTime;
            printf("Process %d executed form %d to %d\n", currentProcess.processId, start,end);
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

    printf("\nProcess ID\tArrival Time\tBurst Time\tPriority\tCompletion Time\t Waiting Time\tTurnaround Time \n");
    for (int i = 0; i < scheduler->ProcessesSize; i++) {
        printf("%d\t\t  %d\t\t %d\t\t %d\t\t %d\t\t  %d\t\t %d \n", scheduler->Processes[i].processId, scheduler->Processes[i].arrivalTime,
               scheduler->Processes[i].burstTime, scheduler->Processes[i].priority,scheduler->copyQueue[i].completionTime,scheduler->copyQueue[i].waitingTime,scheduler->copyQueue[i].turnaroundTime);
    }


    int sum=0;

    for (int i = 0; i < scheduler->copyQueueSize ; i++) {
        sum+=scheduler->copyQueue[i].turnaroundTime;
    }
    printf("Average Turnaround Time = %0.2f\n",(1.0)*sum/scheduler->copyQueueSize);
    turn[0]=(1.0)*sum/scheduler->copyQueueSize;
    sum=0;

    for (int i = 0; i < scheduler->copyQueueSize ; i++) {

        sum+=scheduler->copyQueue[i].turnaroundTime-scheduler->copyQueue[i].burstTime;
    }
    printf("Average Waiting Time = %0.2f",(1.0)*sum/scheduler->copyQueueSize);
    waiting[0]=(1.0)*sum/scheduler->copyQueueSize;

    printf("\n******************************************\n");
}

void SJF(struct SchedulerQueue* scheduler) {
    scheduler->currentTime = 0; // Reset the current time to 0
    printf("\n SJF Scheduler\n");
    struct Process temp;
    for (int i = 0; i < scheduler->copyQueueSize2 - 1; i++) {
        for (int j = 0; j < scheduler->copyQueueSize2 - i - 1; j++) {
            if (scheduler->copyQueue2[j].burstTime > scheduler->copyQueue2[j + 1].burstTime) {
                // Swap
                temp = scheduler->copyQueue2[j];
                scheduler->copyQueue2[j] = scheduler->copyQueue2[j + 1];
                scheduler->copyQueue2[j + 1] = temp;
            }
        }
    }
    int start = 0; // Start from time 0
    for (int i = 0; i < scheduler->copyQueueSize2; i++) {
        struct Process currentProcess = scheduler->copyQueue2[i];
        scheduler->currentTime = start;
        int end = scheduler->currentTime + currentProcess.burstTime;
        printf("Process %d executed for %d units (start at %d end at %d)\n", currentProcess.processId,currentProcess.burstTime, start, end);
        scheduler->copyQueue2[i].completionTime = end;
        scheduler->copyQueue2[i].turnaroundTime = scheduler->copyQueue2[i].completionTime - scheduler->copyQueue2[i].arrivalTime;
        scheduler->copyQueue2[i].waitingTime = scheduler->copyQueue2[i].turnaroundTime - scheduler->copyQueue2[i].burstTime;
        start = end;
    }

    printf("\nProcess ID\tArrival Time\tBurst Time\tPriority\tCompletion Time\t Waiting Time\tTurnaround Time \n");
    for (int i = 0; i < scheduler->ProcessesSize; i++) {
        printf("%d\t\t  %d\t\t %d\t\t %d\t\t %d\t\t  %d\t\t %d \n", scheduler->Processes[i].processId, scheduler->Processes[i].arrivalTime,
               scheduler->Processes[i].burstTime, scheduler->Processes[i].priority,scheduler->copyQueue2[i].completionTime,scheduler->copyQueue2[i].waitingTime,scheduler->copyQueue2[i].turnaroundTime);
    }
    int sum=0;

    for (int i = 0; i < scheduler->copyQueueSize2; i++) {
        sum+=scheduler->copyQueue2[i].turnaroundTime;
    }
    printf("\nAverage Turnaround Time = %0.2f\n",(1.0)*sum/scheduler->copyQueueSize2);
    turn[1]=(1.0)*sum/scheduler->copyQueueSize2;

    sum=0;
    for (int i = 0; i < scheduler->copyQueueSize2; i++) {
        sum+=scheduler->copyQueue2[i].waitingTime;
    }
    printf("Average Waiting Time = %0.2f",(1.0)*sum/scheduler->copyQueueSize2);
    waiting[1]=(1.0)*sum/scheduler->copyQueueSize2;

    printf("\n******************************************\n");
}

// Priority Preemptive
void Priority(struct SchedulerQueue* scheduler) {
    printf("\nPriority Scheduler\n");
    struct Process temp;
    // Sort the processes based on arrival time (ascending order)
    for (int i = 0; i < scheduler->copyQueueSize3 - 1; i++) {
        for (int j = 0; j < scheduler->copyQueueSize3 - i - 1; j++) {
            if (scheduler->copyQueue3[j].arrivalTime > scheduler->copyQueue3[j + 1].arrivalTime) {
                // Swap
                temp = scheduler->copyQueue3[j];
                scheduler->copyQueue3[j] = scheduler->copyQueue3[j + 1];
                scheduler->copyQueue3[j + 1] = temp;
            }
        }
    }
    scheduler->currentTime = scheduler->copyQueue3[0].arrivalTime;
    int completedProcesses = 0;

    while (completedProcesses < scheduler->copyQueueSize3) {
        int highestPriority = 9999;
        int highestPriorityIndex = -1;
        // Find the process with the highest priority
        for (int i = 0; i < scheduler->copyQueueSize3; i++) {
            if (scheduler->copyQueue3[i].arrivalTime <= scheduler->currentTime &&
                scheduler->copyQueue3[i].priority < highestPriority && scheduler->copyQueue3[i].remainingTime > 0) {
                highestPriority = scheduler->copyQueue3[i].priority;
                highestPriorityIndex = i;
            }
        }
        if (highestPriorityIndex == -1) {
            scheduler->currentTime++;
            continue;
        }
        struct Process *currentProcess = &scheduler->copyQueue3[highestPriorityIndex];
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

    printf("\nProcess ID\tArrival Time\tBurst Time\tPriority\tCompletion Time\t Waiting Time\tTurnaround Time \n");
    for (int i = 0; i < scheduler->ProcessesSize; i++) {
        printf("%d\t\t  %d\t\t %d\t\t %d\t\t %d\t\t  %d\t\t %d \n", scheduler->Processes[i].processId, scheduler->Processes[i].arrivalTime,
               scheduler->Processes[i].burstTime, scheduler->Processes[i].priority,scheduler->copyQueue3[i].completionTime,scheduler->copyQueue3[i].waitingTime,scheduler->copyQueue3[i].turnaroundTime);
    }

    int sum=0;
    // Calculate Average turnaround time for each process
    for (int i = 0; i < scheduler->copyQueueSize3 ; i++) {
        sum+=scheduler->copyQueue3[i].turnaroundTime;
    }

    printf("Average Turnaround Time = %0.2f\n",(1.0)*sum/scheduler->copyQueueSize3);
    turn[2]=(1.0)*sum/scheduler->copyQueueSize3;
    sum=0;
    // Calculate waiting time for each process
    for (int i = 0; i < scheduler->copyQueueSize3 ; i++) {
        sum+=scheduler->copyQueue3[i].turnaroundTime-scheduler->copyQueue3[i].burstTime;
    }
    printf("Average Waiting Time = %0.2f",(1.0)*sum/scheduler->copyQueueSize3);
    waiting[2]=(1.0)*sum/scheduler->copyQueueSize3;

    printf("\n******************************************\n");


}
//Shortest Remaining Time First
void SRTF(struct SchedulerQueue* scheduler) {
    printf("\nSRTF Scheduler\n");
    // Initialize variables
    int completedProcesses = 0;
    int currentTime = 0;
    while (completedProcesses < scheduler->copyQueueSize4) {
        int shortestBurstIndex = -1;
        int shortestBurstTime = INT_MAX;

        // Find the process with the shortest remaining burst time among the arrived processes
        for (int i = 0; i < scheduler->copyQueueSize4; i++) {
            if (scheduler->copyQueue4[i].arrivalTime <= currentTime && scheduler->copyQueue4[i].remainingTime > 0) {
                if (scheduler->copyQueue4[i].remainingTime < shortestBurstTime) {
                    shortestBurstIndex = i;
                    shortestBurstTime = scheduler->copyQueue4[i].remainingTime;
                }
            }
        }
        if (shortestBurstIndex == -1) {
            currentTime++;
            continue;
        }
        // Execute the process for 1 unit
        printf("Process %d executed for 1 unit.\n", scheduler->copyQueue4[shortestBurstIndex].processId);
        scheduler->copyQueue4[shortestBurstIndex].remainingTime--;
        currentTime++;
        // Check if the process has completed
        if (scheduler->copyQueue4[shortestBurstIndex].remainingTime == 0) {
            completedProcesses++;
            scheduler->copyQueue4[shortestBurstIndex].completionTime = currentTime;
            scheduler->copyQueue4[shortestBurstIndex].turnaroundTime = scheduler->copyQueue4[shortestBurstIndex].completionTime - scheduler->copyQueue4[shortestBurstIndex].arrivalTime;
            scheduler->copyQueue4[shortestBurstIndex].waitingTime = scheduler->copyQueue4[shortestBurstIndex].turnaroundTime - scheduler->copyQueue4[shortestBurstIndex].burstTime;
        }
    }
    // Calculate average turnaround time and average waiting time
    double avgTurnaroundTime = 0;
    double avgWaitingTime = 0;

    for (int i = 0; i < scheduler->copyQueueSize4; i++) {
        avgTurnaroundTime += scheduler->copyQueue4[i].turnaroundTime;
        avgWaitingTime += scheduler->copyQueue4[i].waitingTime;
    }
    avgTurnaroundTime /= scheduler->copyQueueSize4;
    avgWaitingTime /= scheduler->copyQueueSize4;

    // Display completion time, turnaround time, and waiting time for each process
    printf("\nProcess ID\tArrival Time\tBurst Time\tPriority\tCompletion Time\t Waiting Time\tTurnaround Time \n");
    for (int i = 0; i < scheduler->ProcessesSize; i++) {
        printf("%d\t\t  %d\t\t %d\t\t %d\t\t %d\t\t  %d\t\t %d \n", scheduler->Processes[i].processId, scheduler->Processes[i].arrivalTime,
               scheduler->Processes[i].burstTime, scheduler->Processes[i].priority,scheduler->copyQueue4[i].completionTime,scheduler->copyQueue4[i].waitingTime,scheduler->copyQueue4[i].turnaroundTime);
    }

    printf("Average Turnaround Time = %.2f\n", avgTurnaroundTime);
    turn[3]=avgTurnaroundTime;

    printf("Average Waiting Time = %.2f\n", avgWaitingTime);
    waiting[3]=avgWaitingTime;


    printf("\n******************************************\n");


}
void FCFS(struct SchedulerQueue* scheduler){
    printf("\nFCFS Algorithm \n");
    int currenttime=0;
    int start=0,end=0;
    struct Process temp;

    for (int i = 0; i < scheduler->copyQueueSize5 - 1; i++) {
        for (int j = 0; j < scheduler->copyQueueSize5 - i - 1; j++) {
            if (scheduler->copyQueue5[j].arrivalTime > scheduler->copyQueue5[j + 1].arrivalTime) {
                // Swap
                temp = scheduler->copyQueue5[j];
                scheduler->copyQueue5[j] = scheduler->copyQueue5[j + 1];
                scheduler->copyQueue5[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i <scheduler->copyQueueSize5; i++) {
        start=currenttime;
        end=currenttime;
        if(scheduler->copyQueue5[i].arrivalTime>currenttime){
            printf("idle from %d to %d \n",currenttime,scheduler->copyQueue5[i].arrivalTime);
            currenttime=scheduler->copyQueue5[i].arrivalTime;
        }
        if(scheduler->copyQueue5[i].arrivalTime<=currenttime) {
            printf("Process %d executed from %d to %d \n", scheduler->copyQueue5[i].processId, currenttime,scheduler->copyQueue5[i].burstTime+currenttime);
            currenttime+=scheduler->copyQueue5[i].burstTime;
            scheduler->copyQueue5[i].completionTime = currenttime;
            scheduler->copyQueue5[i].turnaroundTime = end - scheduler->copyQueue5[i].arrivalTime;
            scheduler->copyQueue5[i].waitingTime = scheduler->copyQueue5[i].arrivalTime - scheduler->copyQueue5[i].burstTime;


            int responseTime = start - scheduler->copyQueue5[i].arrivalTime;
            printf("Process %d Response Time: %d\n", scheduler->copyQueue5[i].processId, responseTime);
        }

    }
    // Display completion time, turnaround time, and waiting time for each process

    printf("\nProcess ID\tArrival Time\tBurst Time\tPriority\tCompletion Time\t Waiting Time\tTurnaround Time \n");
    for (int i = 0; i < scheduler->ProcessesSize; i++) {
        printf("%d\t\t  %d\t\t %d\t\t %d\t\t %d\t\t  %d\t\t %d \n", scheduler->Processes[i].processId, scheduler->Processes[i].arrivalTime,
               scheduler->Processes[i].burstTime, scheduler->Processes[i].priority,scheduler->copyQueue5[i].completionTime,scheduler->copyQueue5[i].waitingTime,scheduler->copyQueue5[i].turnaroundTime);
    }

    int sum=0;
    for (int i = 0; i < scheduler->copyQueueSize5; i++) {
        sum+=scheduler->copyQueue5[i].turnaroundTime;
    }
    printf("Average Turnaround Time = %.2f\n", (1.0)*sum/scheduler->copyQueueSize5);
    turn[4]=(1.0)*sum/scheduler->copyQueueSize5;

    int sum2=0;
    for (int i = 0; i < scheduler->copyQueueSize5; i++) {
        sum2+=scheduler->copyQueue5[i].waitingTime;
    }
    printf("Average Waiting Time = %.2f\n", (1.0)*sum2/scheduler->copyQueueSize5);
    waiting[4]=(1.0)*sum2/scheduler->copyQueueSize5;

}


int main() {
    struct SchedulerQueue scheduler;
    scheduler.currentTime = 0;
    scheduler.readyQueueSize = 0;
    scheduler.copyQueueSize = 0;
    scheduler.copyQueueSize2 = 0;
    scheduler.copyQueueSize3 = 0;
    scheduler.copyQueueSize4 = 0;
    scheduler.copyQueueSize5 = 0;
    scheduler.ProcessesSize=0;


    // Add processes to the scheduler
    int numP;
    printf("Enter number of Processes: ");
    scanf("%d", &numP);
    while(numP<1){
        printf("Number of Processes must be more than 0!!!\n");
        printf("Enter number of Processes: ");
        scanf("%d", &numP);
    }
    printf("Enter Time Quantum For RoundRobin Algorithm: ");
    scanf("%d", &scheduler.timeQuantum);
    while(scheduler.timeQuantum<1){
        printf("Time Quantum must be more than 0!!!\n");
        printf("Enter Time Quantum: ");
        scanf("%d", &scheduler.timeQuantum);
    }
    printf("*********************\n");
    scheduler.Processes = malloc(sizeof(struct Process) * 10);
    scheduler.readyQueue = malloc(sizeof(struct Process) * 10);
    scheduler.copyQueue = malloc(sizeof(struct Process) * 10);
    scheduler.copyQueue2 = malloc(sizeof(struct Process) * 10);
    scheduler.copyQueue3 = malloc(sizeof(struct Process) * 10);
    scheduler.copyQueue4 = malloc(sizeof(struct Process) * 10);
    scheduler.copyQueue5 = malloc(sizeof(struct Process) * 10);




    for (int i = 1; i <= numP; i++) {
        int tA, Bt, pri;
        printf("\nEnter Arrival Time for Process[%d]: ", i);
        scanf("%d", &tA);
        printf("Enter Burst Time for Process [%d]: ", i);
        scanf("%d", &Bt);
        while(Bt<1){
            printf("Burst Time must be more than 0!!!\n");
            printf("Enter Burst Time for Process [%d]: ", i);
            scanf("%d", &Bt);
        }
        printf("Enter Priority for Process [%d]: ", i);
        scanf("%d", &pri);

        scheduler.Processes[scheduler.ProcessesSize++] = (struct Process){i, tA, Bt, Bt, -1, -1, pri};
        scheduler.readyQueue[scheduler.readyQueueSize++] = (struct Process){i, tA, Bt, Bt, -1, -1, pri};
        scheduler.copyQueue[scheduler.copyQueueSize++] = (struct Process){i, tA, Bt, Bt, -1, -1, pri};
        scheduler.copyQueue2[scheduler.copyQueueSize2++] = (struct Process){i, tA, Bt, Bt, -1, -1, pri};
        scheduler.copyQueue3[scheduler.copyQueueSize3++] = (struct Process){i, tA, Bt, Bt, -1, -1, pri};
        scheduler.copyQueue4[scheduler.copyQueueSize4++] = (struct Process){i, tA, Bt, Bt, -1, -1, pri};
        scheduler.copyQueue5[scheduler.copyQueueSize5++] = (struct Process){i, tA, Bt, Bt, -1, -1, pri};

    }

    printf("\n*****************************************************************************\n");


    // Print the process details
    printf("Process ID\tArrival Time\tBurst Time\tPriority\t\n");
    for (int i = 0; i < numP; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\n", scheduler.readyQueue[i].processId, scheduler.readyQueue[i].arrivalTime,
               scheduler.readyQueue[i].burstTime, scheduler.readyQueue[i].priority);
    }
    printf("\n*****************************************************************************\n");


    RoundRobin(&scheduler);
    Priority(&scheduler);
    SJF(&scheduler);
    SRTF(&scheduler);
    FCFS(&scheduler);


    // Find the best Algorithm for Turnaround time :
int BestTurn=turn[0];
int algorithm=-1;
    for (int i = 0; i <= 4; i++) {
        if (turn[i] < BestTurn) {
            BestTurn = turn[i];
            algorithm=i;
        }
    }
    printf("*****************************************************************************************\n");
    if(algorithm==0){
        printf("The Best Algorithm For Turnarround Time is RoundRobin with average turnarround time = %0.2f\n",turn[0]);

    }
    if(algorithm==1){
        printf("The Best Algorithm For Turnarround Time is SJF with average turnarround time = %0.2f\n",turn[1]);

    }
    if(algorithm==2){
        printf("The Best Algorithm For Turnarround Time is Priority Preemptive with average turnarround time = %0.2f\n",turn[2]);

    }
    if(algorithm==3){
        printf("The Best Algorithm For Turnarround Time is SRTF with average turnarround time = %0.2f\n",turn[3]);

    }
    if(algorithm==4){
        printf("The Best Algorithm For Turnarround Time is FCFS with average turnarround time = %0.2f\n",turn[4]);

    }
    // Find the best Algorithm for Waiting time :
    int BestWait=waiting[0];
    int algorithm2=-1;

    for (int i = 0; i <= 4; i++) {
        if (waiting[i] < BestWait) {
            BestWait = waiting[i];
            algorithm2=i;
        }
    }
    if(algorithm2==0){
        printf("The Best Algorithm For Waiting Time is RoundRobin with average turnarround time = %0.2f\n",waiting[0]);

    }
    if(algorithm2==1){
        printf("The Best Algorithm For Waiting Time is SJF with average turnarround time = %0.2f\n",waiting[1]);

    }
    if(algorithm2==2){
        printf("The Best Algorithm For Waiting Time is Priority Preemptive with average turnarround time = %0.2f\n",waiting[2]);

    }
    if(algorithm2==3){
        printf("The Best Algorithm For Waiting Time is SRTF with average turnarround time = %0.2f\n",waiting[3]);

    }
    if(algorithm2==4){
        printf("The Best Algorithm For Waiting Time is FCFS with average turnarround time = %0.2f\n",waiting[4]);

    }
    free(scheduler.Processes);
    free(scheduler.readyQueue);
    free(scheduler.copyQueue);
    free(scheduler.copyQueue2);
    free(scheduler.copyQueue3);
    free(scheduler.copyQueue4);
    free(scheduler.copyQueue5);



    return 0;
}