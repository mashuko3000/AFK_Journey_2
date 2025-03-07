#ifndef INC_7_TASK_BASE_H
#define INC_7_TASK_BASE_H

class PriorityQueue {
public:
    virtual ~PriorityQueue() noexcept {};
    virtual void insert(int k, const char* value) = 0;
    virtual const char* findMin() const = 0;
    virtual void remove() = 0;
    virtual PriorityQueue& merge(PriorityQueue& other) = 0;
};

#endif //INC_7_TASK_BASE_H