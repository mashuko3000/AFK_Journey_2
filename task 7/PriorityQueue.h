//
// Created by mvideo on 28.03.2025.
//

#ifndef UNTITLED1_PRIORITYQUEUE_H
#define UNTITLED1_PRIORITYQUEUE_H

class PriorityQueue {
public:
    virtual ~PriorityQueue() noexcept = default;
    virtual void insert(const char* data, int priority) = 0;
    virtual char* peek() const = 0;
    virtual char* remove() = 0;
    virtual void merge(PriorityQueue& other) = 0;
};

#endif //UNTITLED1_PRIORITYQUEUE_H
