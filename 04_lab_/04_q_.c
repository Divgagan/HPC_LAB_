#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


typedef struct Node {
    int data;
    struct Node* next;
} Node;


Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}


void processNode(Node* node) {
    printf("Processing node value = %d by Thread = %d\n",
           node->data, omp_get_thread_num());
}

int main() {

    
    Node* head = createNode(10);
    head->next = createNode(20);
    head->next->next = createNode(30);
    head->next->next->next = createNode(40);

    omp_set_num_threads(4);

    #pragma omp parallel
    {
       
        #pragma omp single
        {
            Node* temp = head;

            while(temp != NULL) {

               
                #pragma omp task firstprivate(temp)
                {
                    processNode(temp);
                }

                temp = temp->next;
            }

            #pragma omp taskwait
        }
    }

 
    return 0;
}



Analysis:
- Single thread traverses linked list → producer.

- Creates tasks for each node.

- Worker threads process nodes → consumers.

- firstprivate(temp) ensures correct node pointer.


Without firstprivate:

- All tasks may access same pointer.

- Wrong node processed.

- Race condition.

Design Pattern Used: 

- Producer–Consumer model.

