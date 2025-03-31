#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cassert>
#include <math.h>


class FibonacciHeap {
    struct Node {
        Node* parent;
        Node* child;
        Node* left;
        Node* right;
        char* data;
        int key;
        int degree;
        bool marked;

        //Node constructor
        Node(int key, const char* value) : parent(nullptr), child(nullptr), left(this), right(this),
                                           key(key), degree(0), marked(false) {
            try {
                data = new char[strlen(value) + 1];
                strcpy(data, value);
            } catch (std::bad_alloc &e) {
                std::cerr << "Memory allocation failed: " << e.what() << std::endl;
                throw;
            }
        }

        //Node destructor
        ~Node(){
            delete[] data;
        }
    };
private:
    Node* minNode;  // Node with minimal key
    int size;       // Count of Nodes in the heap

public:
    // FibonacciHeap constructor
    FibonacciHeap() : size(0), minNode(nullptr) {}

    // Check if the heap is empty
    bool isEmpty() const {
        return minNode == nullptr;
    }

    // FibonacciHeap destructor
    ~FibonacciHeap() {
        while (!isEmpty()) {
            clean(minNode);
            //delete minNode;
        }
    }

    void clean(Node* node) {
        if (node == nullptr) return;

        Node* current = node;
        Node* next;

        do {
            next = current->right;

            // Recursively clean children
            if (current->child) {
                clean(current->child);
            }

            // Remove current node from the doubly linked list BEFORE deleting it
            Node* leftNode = current->left;
            Node* rightNode = current->right;

            if (leftNode) leftNode->right = rightNode;
            if (rightNode) rightNode->left = leftNode;

            // If node is minNode, set minNode to null if is empty
            if(current == minNode){
                minNode = (current->right == current) ? nullptr : current->right;
            }

            // Delete current node
            delete current;
            size--;

            // Move to the next node
            current = next;
        } while (current != node && minNode != nullptr);
    }

    //FibonacciHeap copy constructor
    FibonacciHeap (const FibonacciHeap& other){
        minNode = cloneHeap(other.minNode);
        size = other.size;
    }

    //Clone Heap
    Node* cloneHeap(const Node* other){
        if (other == nullptr) {
            return nullptr;
        }
        try{
            Node* copy = new Node(other->key, other->data);
            copy->degree = other->degree;
            copy->marked = other->marked;

            if (other->child != nullptr) {
                copy->child = cloneHeap(other->child);
                copy->child->parent = copy;
            }

            Node *curr = other->right;
            Node *prev_copy = copy;
            while (curr != other) {
                Node *new_Node = new Node(curr->key, curr->data);
                new_Node->degree = curr->degree;
                new_Node->marked = curr->marked;

                if (curr->child != nullptr) {
                    new_Node->child = cloneHeap(curr->child);
                    new_Node->child->parent = new_Node;
                }

                prev_copy->right = new_Node;
                new_Node->left = prev_copy;
                prev_copy = new_Node;
                curr = curr->right;
            }

            prev_copy->right = copy;
            copy->left = prev_copy;

            return copy;
        } catch (const std::bad_alloc &e) {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            throw;
        }
    }
    void insert(const char* data, int priority) {
        try {
            Node* newNode = new Node(priority, data);
            if (minNode == nullptr) {
                minNode = newNode;
                minNode->right = minNode;
                minNode->left = minNode;
            } else {
                // Add new Node to the root list
                newNode->right = minNode->right;
                newNode->left = minNode;
                minNode->right->left = newNode;
                minNode->right = newNode;
                if (newNode->key < minNode->key) {
                    minNode = newNode;
                }
            }
            ++size; // Increase size outside the if-else block
        } catch (std::bad_alloc &e) {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            throw; // Re-throw the exception
        }
    }

    const char* peek() const {
        if (size == 0) {
            throw std::invalid_argument("Heap is empty");
        }
        return minNode->data;
    }

    FibonacciHeap* merge(FibonacciHeap& other) {
        if (other.size == 0) {
            return this;
        }

        if (size == 0) {
            minNode = other.minNode;
            size = other.size;
        } else {
            unionList(minNode, other.minNode);
            size += other.size;
        }
        if ((minNode != nullptr) || ((other.minNode != nullptr) && other.minNode < minNode)) {
            minNode = other.minNode;
        }

        other.minNode = nullptr;
        other.size = 0;

        return this;
    }

    void remove() {
        if (size == 0) {
            throw std::invalid_argument("Heap is empty");
        }

        Node *prev_min = minNode;

        if (minNode->child != nullptr) {
            Node *child = minNode->child;
            do {
                Node *next_child = child->right;
                unionList(minNode, child);
                child->parent = nullptr;
                child = next_child;
            } while (child != minNode->child);
        }

        Node *L = minNode->left;
        Node *R = minNode->right;
        L->right = R;
        R->left = L;

        if (minNode->right == minNode) {
            delete minNode;
            minNode = nullptr;
            size = 0;
            return;
        }
        delete minNode;
        minNode = R;
        consolidate();
        --size;
    }

    void consolidate() {
        const auto array_size = static_cast<size_t>(std::ceil(std::log2(size + 1)));
        Node **A = new Node *[array_size]();

        Node *current = minNode;
        do {
            Node *next = current->right;
            Node *degree_Node = A[current->degree];

            while (degree_Node != nullptr) {
                Node *add_to;
                Node *adding;
                if (degree_Node->key < current->key) {
                    add_to = degree_Node;
                    adding = current;
                } else {
                    add_to = current;
                    adding = degree_Node;
                }

                unionList(add_to->child, adding);
                adding->parent = add_to;
                A[add_to->degree] = nullptr;
                ++(add_to->degree);
                current = add_to;
                degree_Node = A[current->degree];
            }

            A[current->degree] = current;
            current = next;
        } while (current != minNode);

        minNode = nullptr;
        for (int i = 0; i < array_size; ++i) {
            if (A[i] != nullptr) {
                if ((minNode == nullptr) || A[i]->key < minNode->key) {
                    minNode = A[i];
                }
            }
        }

        delete[] A;
    }

    void unionList(Node* Node1, Node* Node2) {
        if (Node1 == nullptr || Node2 == nullptr) {
            return;
        }
        Node *newLeft = Node1->left;
        Node *newRight = Node2->right;
        Node2->right = Node1;
        Node1->left = Node2;
        newLeft->right = newRight;
        newRight->left = newLeft;
    }
};


void testFibonacciHeap() {
    // Test 1: Basic insert and peek
    FibonacciHeap heap1;
    heap1.insert("A", 5);
    heap1.insert("B", 3);
    heap1.insert("C", 7);
    assert(strcmp(heap1.peek(), "B") == 0);
    assert(!heap1.isEmpty());

    // Test 2: Merge with empty heap
    FibonacciHeap heap2;
    heap1.merge(heap2);
    assert(strcmp(heap1.peek(), "B") == 0);
    assert(heap2.isEmpty());

    // Test 3: Merge two non-empty heaps
    FibonacciHeap heap3;
    heap3.insert("D", 2);
    heap3.insert("E", 4);
    heap1.merge(heap3);
    assert(strcmp(heap1.peek(), "D") == 0); // Minimum should be D (priority 2)

    // Test 4: Check sizes after merge
    assert(!heap1.isEmpty());
    FibonacciHeap heap4;
    assert(heap4.isEmpty());
    heap4.insert("F", 10);
    heap4.insert("G", 1);
    assert(strcmp(heap4.peek(), "G") == 0);
    heap1.merge(heap4);
    assert(strcmp(heap1.peek(), "G") == 0);
    assert(!heap1.isEmpty());
    assert(heap4.isEmpty());

    // Test 5: Check that other heap is empty after merge

    // Test 6: Test merge with a heap that becomes empty
    FibonacciHeap heap5;
    heap5.insert("H", 8);
    heap1.merge(heap5);
    assert(strcmp(heap1.peek(), "H") == 0);
    assert(heap5.isEmpty());

    //Test 7: Check clear heap

    std::cout << "All merge tests passed!" << std::endl;
}

void testFibonacciHeapBasic() {
    FibonacciHeap heap;

    // Test 1: Basic insert and peek
    heap.insert("A", 5);
    assert(strcmp(heap.peek(), "A") == 0);

    heap.insert("B", 3);
    assert(strcmp(heap.peek(), "B") == 0);

    heap.insert("C", 7);
    assert(strcmp(heap.peek(), "B") == 0);

    // Test 2: isEmpty
    assert(!heap.isEmpty());
    FibonacciHeap emptyHeap;
    assert(emptyHeap.isEmpty());

    // Test 3: Copy constructor
    FibonacciHeap heap2 = heap;
    assert(strcmp(heap2.peek(), "B") == 0);

    // Test 4: Check original heap is not affected by copy
    heap2.insert("D", 1);
    assert(strcmp(heap2.peek(), "D") == 0);
    assert(strcmp(heap.peek(), "B") == 0);

    // Test 5: Large number of inserts
    FibonacciHeap largeHeap;
    const int numNodes = 100;
    const char* nodeData[numNodes];

    for (int i = 0; i < numNodes; ++i) {
        char* data = new char[10]; // Allocate memory for the string
        snprintf(data, 10, "Node %d", i); // Fill the string with "Node i"
        nodeData[i] = data; // Store the pointer
        largeHeap.insert(nodeData[i], rand() % 1000);
    }
    assert(!largeHeap.isEmpty());

    std::cout << "All base tests passed!" << std::endl;
}

int main() {
    testFibonacciHeap();
    testFibonacciHeapBasic();
    return 0;
}