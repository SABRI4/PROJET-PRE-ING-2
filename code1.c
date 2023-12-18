#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Driver structure
struct Driver {
    char route_id[MAX_LINE_LENGTH];
    char name[MAX_LINE_LENGTH];
    int unique_routes;
};

// AVL Tree Node
struct Node {
    struct Driver data;
    struct Node *left;
    struct Node *right;
    int height;
};

// Function to get the height of a node
int height(struct Node *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Function to get the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to create a new AVL tree node
struct Node *newNode(struct Driver data) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // New node is initially at height 1
    return node;
}

// Function to insert a driver into an AVL tree
struct Node *insert(struct Node *node, struct Driver data) {
    // Perform standard BST insertion
    if (node == NULL)
        return newNode(data);

    int compareResult = strcmp(data.name, node->data.name);
    if (compareResult < 0)
        node->left = insert(node->left, data);
    else if (compareResult > 0)
        node->right = insert(node->right, data);
    else {
        // Duplicate driver name, increment the unique_routes count
        node->data.unique_routes++;
        return node;
    }

    // Update height of current node
    node->height = 1 + max(height(node->left), height(node->right));

    return node;
}

// Function to print drivers and their unique route counts in sorted order
void inOrder(struct Node *root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%s %s %d\n", root->data.name, root->data.route_id, root->data.unique_routes);
        inOrder(root->right);
    }
}

// Function to print the top 10 drivers with the highest unique route counts
void printTop10(struct Node *root, int *count) {
    if (root != NULL && *count < 10) {
        printTop10(root->right, count);

        if (*count < 10) {
            printf("%s %s %d\n", root->data.name, root->data.route_id, root->data.unique_routes);
            (*count)++;
        }

        printTop10(root->left, count);
    }
}

// Function to free the memory allocated for an AVL tree
void freeAVL(struct Node *root) {
    if (root != NULL) {
        freeAVL(root->left);
        freeAVL(root->right);
        free(root);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <csv_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *csv_file = argv[1];
    FILE *file = fopen(csv_file, "r");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    struct Node *root = NULL;

    while (fgets(line, sizeof(line), file) != NULL) {
        char *route_id = strtok(line, ";");
        strtok(NULL, ";"); // Skip the second field
        char *name = strtok(NULL, ";");

        if (route_id != NULL && name != NULL) {
            struct Driver newDriver;
            strncpy(newDriver.route_id, route_id, sizeof(newDriver.route_id) - 1);
            newDriver.route_id[sizeof(newDriver.route_id) - 1] = '\0';

            strncpy(newDriver.name, name, sizeof(newDriver.name) - 1);
            newDriver.name[sizeof(newDriver.name) - 1] = '\0';

            newDriver.unique_routes = 1; // Initialize the unique_routes count
            root = insert(root, newDriver);
        }
    }

    fclose(file);

    printf("Driver names, route_id, and the count of unique routes:\n");
    inOrder(root);

    printf("\nTop 10 drivers with the highest unique route counts:\n");
    int count = 0;
    printTop10(root, &count);

    freeAVL(root);

    return 0;
}
