//
// Created by mvideo on 28.03.2025.
//

#ifndef UNTITLED1_PRIORITYQUEUE_H
#define UNTITLED1_PRIORITYQUEUE_H

class PriorityQueue {
public:
    virtual ~PriorityQueue() noexcept = default;
    virtual void insert(int priority, const char* value) = 0;
    virtual const char* peek() const = 0;
    virtual void remove() = 0;
    virtual PriorityQueue& merge(PriorityQueue& other) = 0;
};

#endif //UNTITLED1_PRIORITYQUEUE_H
