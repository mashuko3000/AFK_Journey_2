//
// Created by mvideo on 28.03.2025.
//

#ifndef INC_10_TASK_SKEWHEAP_HPP
#define INC_10_TASK_SKEWHEAP_HPP

#include "../inc/PriorityQueue.h"

struct SkewNode{
    char* data;
    int priority;
    SkewNode* right;
    SkewNode* left;
    
    SkewNode(const char* d, int p);
    ~SkewNode() noexcept;
    SkewNode(const SkewNode& other);
    SkewNode& operator=(const SkewNode& other);
};

class SkewPriorityQueue final : public PriorityQueue{
private:
    SkewNode* root;
    int size;
    SkewNode* copy_tree(const SkewNode* node) const;
    SkewNode* merge_nodes(SkewNode* a, SkewNode* b);
    void clear(SkewNode* node);
public:
    SkewPriorityQueue();
    ~SkewPriorityQueue() noexcept override;
    SkewPriorityQueue(const SkewPriorityQueue& other);
    SkewPriorityQueue& operator=(const SkewPriorityQueue& other);

    void insert(int priority, const char* data)override;
    const char* peek() const override;
    void remove()override;
    PriorityQueue& merge(PriorityQueue& other)override;

    void print(SkewNode* node, int depth) const;
    void print() const;
};

#endif //INC_10_TASK_SKEWHEAP_HPP
