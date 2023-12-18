#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Driver structure
struct Driver {
    char route_id[MAX_LINE_LENGTH];
    char name[MAX_LINE_LENGTH];
};

// AVL Tree Node
struct Node {
    struct Driver data;
    struct Node *left;
    struct Node *right;
    int height;
    int unique_routes;
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

// Function to get the balance factor of a node
int getBalance(struct Node *node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Function to rotate a subtree rooted with y to the right
struct Node *rightRotate(struct Node *y) {
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

// Function to rotate a subtree rooted with x to the left
struct Node *leftRotate(struct Node *x) {
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
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
    node->unique_routes = 1; // New driver has taken one route by default
    return node;
}

// Function to insert a driver into an AVL tree
struct Node *insert(struct Node *node, struct Driver data) {
    // Perform standard BST insertion
    if (node == NULL)
        return newNode(data);

    if (strcmp(data.name, node->data.name) < 0)
        node->left = insert(node->left, data);
    else if (strcmp(data.name, node->data.name) > 0)
        node->right = insert(node->right, data);
    else {
        // Duplicate driver name, increment the unique_routes count
        node->unique_routes++;
        return node;
    }

    // Update height of current node
    node->height = 1 + max(height(node->left), height(node->right));

    // Get the balance factor to check whether this node became unbalanced
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && strcmp(data.name, node->left->data.name) < 0)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && strcmp(data.name, node->right->data.name) > 0)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && strcmp(data.name, node->left->data.name) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && strcmp(data.name, node->right->data.name) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Function to print drivers and their unique route counts in sorted order
void inOrder(struct Node *root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%s %d\n", root->data.name, root->unique_routes);
        inOrder(root->right);
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

            root = insert(root, newDriver);
        }
    }

    fclose(file);

    printf("Driver names and the count of unique routes:\n");
    inOrder(root);

    freeAVL(root);

    return 0;
}
