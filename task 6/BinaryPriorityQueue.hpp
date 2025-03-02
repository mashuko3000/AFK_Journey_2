#ifndef BINARYPRIORITYQUEUE_HPP
#define BINARYPRIORITYQUEUE_HPP

#include "base.hpp"
#include <stdexcept>
#include <cstring>

class BinaryPriorityQueue : public PriorityQueue {
private:
    struct Node{
        int priority;
        char* data;

        Node(int p, const char* d);
        ~Node();
    };

    Node** heap;
    size_t size;
    size_t capacity;

    void resize();
    void heapUp(int index);
    void heapDown(int index);
public:
    BinaryPriorityQueue();
    ~BinaryPriorityQueue() noexcept override;

    void insert(int priority, const char* value) override;
    const char* peek() const override;
    void remove() override;
    PriorityQueue& merge(PriorityQueue& other) override;
};

#endif
