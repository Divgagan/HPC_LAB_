#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//  BST ke nodes ka recursive sum parallel nested tasks se nikalna tha
//  aur variable scoping aur synchronization (taskwait vs barrier) 
//  ka behaviour observe karna tha.

typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, int value) {
    if (root == NULL) return createNode(value);

    if (value < root->data)
        root->left = insert(root->left, value);
    else
        root->right = insert(root->right, value);

    return root;
}

int parallelBSTSum(Node* root) {
    if (root == NULL)
        return 0;

    int leftSum = 0, rightSum = 0;

    #pragma omp task shared(leftSum) if(root->left != NULL)
    {
        leftSum = parallelBSTSum(root->left);
    }

    #pragma omp task shared(rightSum) if(root->right != NULL)
    {
        rightSum = parallelBSTSum(root->right);
    }

    #pragma omp taskwait

    return root->data + leftSum + rightSum;
}

int main() {
    Node* root = NULL;

    int values[10] = {5,3,7,2,4,6,8,1,9,10};

    for(int i=0;i<10;i++)
        root = insert(root, values[i]);

    int total = 0;

    #pragma omp parallel
    {
        #pragma omp single
        {
            total = parallelBSTSum(root);
        }
    }

    printf("BST Sum = %d\n", total);

    return 0;
}

// Analysis of the code:
// 1. The code defines a binary search tree (BST) with a structure `Node` and functions to create nodes and insert values.
// 2. The `parallelBSTSum` function computes the sum of all nodes in the
BST using OpenMP tasks for parallelism.
// 3. For each node, two tasks are created to compute the sum of the left and right subtrees in parallel, with a condition to avoid creating tasks for NULL children.
// 4. The `#pragma omp taskwait` directive ensures that the function waits for both tasks to complete before summing their results and returning the total sum for the current node.
// 5. The `main` function constructs a BST with predefined values, computes the total sum of the nodes in parallel, and prints the result.