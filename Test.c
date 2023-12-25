#include <stdio.h>
#include <stdlib.h>

// Define the structure for a process
typedef struct {
    int process_id;
    int priority;
    int burst_time;
} Process;

// Define the structure for a node in the queue
typedef struct Node {
    Process data;
    struct Node* next;
} Node;

// Define the structure for a queue
typedef struct {
    Node* front;
    Node* rear;
} Queue;

// Initialize the queue by setting front and rear to NULL
void initializeQueue(Queue* queue) {
    queue->front = queue->rear = NULL;
}

// Check if the queue is empty
int isQueueEmpty(Queue* queue) {
    return (queue->front == NULL);
}

// Enqueue a process into the queue
void enqueue(Queue* queue, Process process) {
    // Create a new node and allocate memory for it
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = process;
    newNode->next = NULL;

    // If the queue is empty, set both front and rear to the new node
    if (isQueueEmpty(queue)) {
        queue->front = queue->rear = newNode;
    } else {
        // Otherwise, add the new node to the rear and update the rear pointer
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Dequeue a process from the queue
Process dequeue(Queue* queue) {
    // Check if the queue is empty
    if (isQueueEmpty(queue)) {
        printf("Queue is empty.\n");
        exit(EXIT_FAILURE);
    }

    // Get the front node and process
    Node* temp = queue->front;
    Process process = temp->data;

    // Move the front pointer to the next node and free the memory
    queue->front = temp->next;
    free(temp);

    // If the queue becomes empty, update the rear pointer as well
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    // Return the dequeued process
    return process;
}

// Multilevel Queue Scheduling
void MLQScheduling(Process processes[], int numProcesses) {
    // Create three priority queues
    Queue queues[3];
    int currentTime = 0;

    // Initialize each queue
    for (int i = 0; i < 3; i++) {
        initializeQueue(&queues[i]);
    }

    // Enqueue processes based on their priority
    for (int i = 0; i < numProcesses; i++) {
        enqueue(&queues[processes[i].priority - 1], processes[i]);
    }

    // Execute processes based on priority
    while (1) {
        for (int i = 0; i < 3; i++) {
            if (!isQueueEmpty(&queues[i])) {
                // Dequeue and execute the process
                Process currentProcess = dequeue(&queues[i]);
                printf("Executing Process %d (Priority %d, Burst Time %d) at Time %d\n",
                       currentProcess.process_id, currentProcess.priority, currentProcess.burst_time, currentTime);

                // Update the current time and consider the process as completed
                currentTime += currentProcess.burst_time;
                numProcesses--;

                // If the process has remaining burst time, enqueue it back
                if (currentProcess.burst_time > 0) {
                    enqueue(&queues[i], currentProcess);
                }
            }
        }

        // Check if all processes have been executed
        if (numProcesses == 0) {
            break;
        }
    }
}

// Main function
int main() {
    int numProcesses;

    // Get the number of processes from the user
    printf("Enter the number of processes: ");
    scanf("%d", &numProcesses);

    // Allocate memory for an array of processes
    Process* processes = (Process*)malloc(numProcesses * sizeof(Process));

    // Input details for each process
    for (int i = 0; i < numProcesses; i++) {
        printf("Enter details for Process %d:\n", i + 1);
        processes[i].process_id = i + 1;

        printf("Priority (1, 2, or 3): ");
        scanf("%d", &processes[i].priority);

        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
    }

    // Perform Multilevel Queue CPU Scheduling
    MLQScheduling(processes, numProcesses);

    // Free allocated memory
    free(processes);

    // Return 0 to indicate successful execution
    return 0;
}
