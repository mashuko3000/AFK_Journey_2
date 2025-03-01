#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

class PriorityQueue {
public:
    virtual ~PriorityQueue(){};
    virtual void insert(int priority, const char* data) = 0;
    virtual const char* peek() const = 0;
    virtual void remove() = 0;
    virtual PriorityQueue& merge(PriorityQueue& other) = 0;
};

#endif