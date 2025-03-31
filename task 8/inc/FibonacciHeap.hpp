//
// Created by mvideo on 30.03.2025.
//

#ifndef INC_8_TASK_FIBONACCIHEAP_HPP
#define INC_8_TASK_FIBONACCIHEAP_HPP

#include "../inc/PriorityQueue.h"

class FibonacciHeap : public PriorityQueue {
private:
    struct Node{
        char* data;
        int priority;
        Node* sibling;
        Node* child;
        Node* parent;
        int degree;
        bool marked;
        Node(const char* value, int prio);
        ~Node();
    };
    Node* minNode;
    int size;

    void deleteAllNodes(Node* node);
    Node* deepCopyNodes(const Node* src);
    Node* copyNode(const Node* src);
    void addToRootList(Node* node);
    void consolidate();
    void link(Node* a, Node* b);
    void cut(Node* x, Node* y);
    void cascadingCut(Node* y);
    void decreaseKey(Node* x, int newPriority);
    void deleteNode(Node* x);

public:
    FibonacciHeap();
    FibonacciHeap(const FibonacciHeap& other);
    FibonacciHeap& operator=(const FibonacciHeap& other);
    ~FibonacciHeap() noexcept override;

    void insert(const char* data, int priority) override;
    const char* peek() const override;
    void remove() override;
    void merge(PriorityQueue& other) override;

    bool isEmpty() const;
};

#endif //INC_8_TASK_FIBONACCIHEAP_HPP
