//
// Created by mvideo on 29.03.2025.
//

#ifndef INC_7_TASK_BINOMIALHEAP_HPP
#define INC_7_TASK_BINOMIALHEAP_HPP

#include "../inc/PriorityQueue.h"

class BinomialPriorityQueue final : public PriorityQueue {
private:
    // Node structure for the binomial heap
    struct Node {
        char* data;           // Data stored in the node
        int priority;         // Priority of the node
        Node* sibling;        // Pointer to the next binomial tree
        Node* child;          // Pointer to the first child
        int degree;           // Number of children

        // Constructor
        Node(const char* value, int prio);

        // Destructor
        ~Node();
    };

    Node* head;  // Pointer to the head of the binomial heap

    // Helper methods
    Node* mergeHeaps(Node* h1, Node* h2);
    void linkNodes(Node* y, Node* z);
    Node* unionHeaps(Node* h1, Node* h2);
    Node* findMinNode() const;
    Node* removeMinNode();
    void deleteAllNodes(Node* node);
    Node* copyNodes(const Node* node);
    void makeCircularChildList(Node* parent);
    bool isCircularChildList(Node* parent) const;

public:
    // Constructor
    BinomialPriorityQueue();

    // Rule of three
    BinomialPriorityQueue(const BinomialPriorityQueue& other);
    BinomialPriorityQueue& operator=(const BinomialPriorityQueue& other);
    ~BinomialPriorityQueue() noexcept override;

    // Main operations
    void insert(const char* data, int priority) override;
    char* peek() const override;
    char* remove() override;
    void merge(PriorityQueue& other) override;

    // Utility methods
    bool isEmpty() const;
    void clear();
};

#endif //INC_7_TASK_BINOMIALHEAP_HPP
