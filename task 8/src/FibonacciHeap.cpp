//
// Created by mvideo on 30.03.2025.
//
#include "../inc/FibonacciHeap.hpp"
#include <cstring>
#include <stdexcept>

// Node constructor
FibonacciHeap::Node::Node(const char* value, int prio) : priority(prio), sibling(nullptr), child(nullptr), parent(nullptr), degree(0), marked(false) {
    try {
        data = new char[strlen(value) + 1];
        strcpy(data, value);
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("Memory allocation failed for Node data");
    }
}
//Node destructor
FibonacciHeap::Node::~Node(){
    delete[] data;
}

// FibonacciHeap constructor
FibonacciHeap::FibonacciHeap() : minNode(nullptr), size(0){}

// FibonacciHeap copy constructor
FibonacciHeap::FibonacciHeap(const FibonacciHeap& other) : minNode(nullptr), size(0){
    if (other.minNode){
        minNode = deepCopyNodes(other.minNode);
        size = other.size;
    }
}

// FibonacciHeap assignment operator
FibonacciHeap& FibonacciHeap::operator=(const FibonacciHeap& other){
    if (this != &other) {
        if (minNode) {
            deleteAllNodes(minNode);
            minNode = nullptr;
        }
        if (other.minNode) {
            minNode = deepCopyNodes(other.minNode);
            size = other.size;
        } else {
            size = 0;
        }
    }
    return *this;
}

// FibonacciHeap destructor
FibonacciHeap::~FibonacciHeap() noexcept{
    if (minNode) {
        deleteAllNodes(minNode);
    }
}

// Helper method to delete all nodes
void FibonacciHeap::deleteAllNodes(Node* node) {
    if (!node) return;
    Node* current = node;
    Node* temp;
    // For each sibling in the circular list
    do {
        temp = current;
        current = current->sibling;

        // Delete all children recursively
        if (temp->child) {
            deleteAllNodes(temp->child);
        }

        // Delete the current node
        delete temp;
    } while (current != node);
}

// Helper method for deep copying nodes
FibonacciHeap::Node* FibonacciHeap::deepCopyNodes(const Node* src) {
    if (!src) return nullptr;

    // First pass: count nodes
    int nodeCount = 0;
    const Node* current = src;
    do {
        nodeCount++;
        current = current->sibling;
    } while (current != src);

    // Create array for nodes
    Node** nodeArray = new Node*[nodeCount];

    // Second pass: create nodes
    current = src;
    for (int i = 0; i < nodeCount; i++) {
        nodeArray[i] = copyNode(current);
        current = current->sibling;
    }

    // Third pass: setup relationships
    Node* newMinNode = nullptr;
    for (int i = 0; i < nodeCount; i++) {
        Node* newNode = nodeArray[i];
        const Node* originalNode = src;

        // Find corresponding original node
        for (int j = 0; j < i; j++) {
            originalNode = originalNode->sibling;
        }

        // Setup sibling pointers (circular)
        newNode->sibling = nodeArray[(i + 1) % nodeCount];

        // Copy children
        if (originalNode->child) {
            newNode->child = deepCopyNodes(originalNode->child);
            // Update parent pointers
            if (newNode->child) {
                Node* child = newNode->child;
                do {
                    child->parent = newNode;
                    child = child->sibling;
                } while (child != newNode->child);
            }
        }

        // Track min node
        if (!newMinNode || newNode->priority < newMinNode->priority) {
            newMinNode = newNode;
        }
    }

    delete[] nodeArray;
    return newMinNode;
}

// Helper method to copy a single node
FibonacciHeap::Node* FibonacciHeap::copyNode(const Node* src) {
    if (!src) return nullptr;

    try {
        Node* newNode = new Node(src->data, src->priority);
        newNode->degree = src->degree;
        newNode->marked = src->marked;
        newNode->child = nullptr;    // Дочерние узлы копируются отдельно
        newNode->parent = nullptr;   // Родитель будет установлен позже
        newNode->sibling = nullptr;  // Связи устанавливаются в deepCopyNodes
        return newNode;
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("Memory allocation failed during node copy");
    }
}

// Check if the heap is empty
bool FibonacciHeap::isEmpty() const {
    return minNode == nullptr;
}

// Insert a new element into the heap
void FibonacciHeap::insert(const char* data, int priority) {
    try {
        Node* newNode = new Node(data, priority);

        if (!minNode) {
            // The heap is empty
            minNode = newNode;
            newNode->sibling = newNode; // Circular reference to itself
        } else {
            // Add to root list
            addToRootList(newNode);

            // Update minimum if needed
            if (newNode->priority < minNode->priority) {
                minNode = newNode;
            }
        }

        size++;
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("Memory allocation failed during insert");
    }
}

// Helper method to add a node to the root list
void FibonacciHeap::addToRootList(Node* node) {
    if (!minNode) {
        minNode = node;
        node->sibling = node; // circular
    } else {
        // Insert node between minNode and minNode->sibling
        node->sibling = minNode->sibling;
        minNode->sibling = node;
    }
    node->parent = nullptr;
}

// Merge this heap with another heap
void FibonacciHeap::merge(FibonacciHeap& other) {
    if (this == &other) return;
    if (other.isEmpty()) return;

    if (isEmpty()) {
        minNode = other.minNode;
        size = other.size;
    } else {
        // Connect the two circular root lists
        Node* thisNext = minNode->sibling;
        Node* otherNext = other.minNode->sibling;
        minNode->sibling = otherNext;
        other.minNode->sibling = thisNext;

        // Update minimum if needed
        if (other.minNode->priority < minNode->priority) {
            minNode = other.minNode;
        }

        size += other.size;
    }

    // Clear the other heap
    other.minNode = nullptr;
    other.size = 0;
}

// Get the minimum element's data (peek)
const char* FibonacciHeap::peek() const {
    if (isEmpty()) {
        throw std::runtime_error("Cannot peek: heap is empty");
    }
    return minNode->data;
}

// Remove the minimum element
void FibonacciHeap::remove() {
    if (isEmpty()) {
        throw std::runtime_error("Cannot remove: heap is empty");
    }

    Node* oldMin = minNode;

    // Add each child of the minimum node to the root list
    if (oldMin->child) {
        Node* child = oldMin->child;
        Node* nextChild;

        // Disconnect each child's parent pointer and add to root list
        do {
            nextChild = child->sibling;
            child->parent = nullptr;
            addToRootList(child);
            child = nextChild;
        } while (child != oldMin->child);
    }

    // Remove minimum node from root list
    if (oldMin->sibling == oldMin) {
        // This was the only node
        minNode = nullptr;
    } else {
        // Find the node that points to oldMin
        Node* prevSibling = oldMin;
        while (prevSibling->sibling != oldMin) {
            prevSibling = prevSibling->sibling;
        }

        // Update sibling pointer to skip oldMin
        prevSibling->sibling = oldMin->sibling;
        minNode = oldMin->sibling;

        // Consolidate the heap
        consolidate();
    }

    delete oldMin;
    size--;
}

// Consolidate the heap after a removal
void FibonacciHeap::consolidate() {
    if (isEmpty()) return;
    const int maxDegree = 32;
    Node* degreeArray[maxDegree] = {nullptr};
    Node* current = minNode;
    Node* start = minNode;
    Node* next;
    int numRoots = 0;
    Node** rootList = nullptr;
    do {
        numRoots++;
        current = current->sibling;
    } while (current != start);
    try {
        rootList = new Node*[numRoots];
        current = minNode;
        for (int i = 0; i < numRoots; i++) {
            rootList[i] = current;
            current = current->sibling;
        }
        for (int i = 0; i < numRoots; i++) {
            current = rootList[i];
            int degree = current->degree;
            while (degreeArray[degree]) {
                Node* other = degreeArray[degree];
                if (current->priority > other->priority) {
                    Node* temp = current;
                    current = other;
                    other = temp;
                }
                link(other, current);
                degreeArray[degree] = nullptr;
                degree++;
            }
            degreeArray[degree] = current;
        }
        minNode = nullptr;
        for (int i = 0; i < maxDegree; i++) {
            if (degreeArray[i]) {
                if (!minNode) {
                    minNode = degreeArray[i];
                    minNode->sibling = minNode;
                } else {
                    addToRootList(degreeArray[i]);
                    if (degreeArray[i]->priority < minNode->priority) {
                        minNode = degreeArray[i];
                    }
                }
            }
        }
        delete[] rootList;
    } catch (const std::bad_alloc& e) {
        if (rootList) delete[] rootList;
        throw std::runtime_error("Memory allocation failed during consolidation");
    }
}

// Link two trees in the consolidate phase
void FibonacciHeap::link(Node* y, Node* x) {
    // Remove y from root list
    Node* prev = y->sibling;
    while (prev->sibling != y) {
        prev = prev->sibling;
    }
    prev->sibling = y->sibling;

    // Make y a child of x
    if (!x->child) {
        x->child = y;
        y->sibling = y; // Circular list of one
    } else {
        y->sibling = x->child->sibling;
        x->child->sibling = y;
    }

    y->parent = x;
    x->degree++;
    y->marked = false;
}

// Cut a node from its parent
void FibonacciHeap::cut(Node* x, Node* y) {
    // Remove x from y's child list
    if (x->sibling == x) {
        // x is the only child
        y->child = nullptr;
    } else {
        Node* prev = x;
        while (prev->sibling != x) {
            prev = prev->sibling;
        }
        prev->sibling = x->sibling;

        if (y->child == x) {
            y->child = x->sibling;
        }
    }

    y->degree--;

    // Add x to the root list
    addToRootList(x);
    x->parent = nullptr;
    x->marked = false;
}

// Perform cascading cuts after a decrease-key operation
void FibonacciHeap::cascadingCut(Node* y) {
    Node* z = y->parent;
    if (z) {
        if (!y->marked) {
            y->marked = true;
        } else {
            cut(y, z);
            cascadingCut(z);
        }
    }
}

// Decrease the key of a node
void FibonacciHeap::decreaseKey(Node* x, int newPriority) {
    if (newPriority > x->priority) {
        throw std::invalid_argument("New priority is greater than current priority");
    }
    x->priority = newPriority;
    Node* y = x->parent;
    if (y && x->priority < y->priority) {
        cut(x, y);
        cascadingCut(y);
    }
    if (x->priority < minNode->priority) {
        minNode = x;
    }
}

// Delete a node from the heap
void FibonacciHeap::deleteNode(Node* x) {
    decreaseKey(x, INT_MIN);
    remove();
}