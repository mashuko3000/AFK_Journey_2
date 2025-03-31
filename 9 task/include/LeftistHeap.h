//
// Created by mvideo on 28.03.2025.
//

#ifndef UNTITLED1_LEFTISTHEAP_H
#define UNTITLED1_LEFTISTHEAP_H

#include "../include/PriorityQueue.h"

struct LeftistNode{
    char* data;
    int priority;
    int rank;
    LeftistNode* left;
    LeftistNode* right;

    LeftistNode(const char* d, int p);
    LeftistNode(const LeftistNode& other)= delete;
    LeftistNode& operator=(const LeftistNode& other) = delete;
    ~LeftistNode();
};

class LeftistPriorityQueue final : public PriorityQueue{
private:
    LeftistNode* root;
    int size;

    int get_rank(LeftistNode* node);

    LeftistNode* merge_nodes(LeftistNode* a, LeftistNode* b);

    void clear(LeftistNode*);
public:
    LeftistPriorityQueue();
    ~LeftistPriorityQueue() noexcept override;

    void print(LeftistNode* node, int depth) const;
    void print() const;

    void insert(int priority, const char* data) override;
    const char* peek() const override;
    void remove() override;
    PriorityQueue& merge(PriorityQueue& other) override;
};

#endif //UNTITLED1_LEFTISTHEAP_H
