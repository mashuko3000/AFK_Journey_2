//
// Created by mvideo on 28.03.2025.
//
#include "../inc/SkewHeap.hpp"
#include <cstring>
#include <stdexcept>
#include <iostream>

SkewNode::SkewNode(const char* d, int p) : priority(p), right(nullptr), left(nullptr){
    try{
        data = new char[strlen(d)+1];
        strcpy(data, d);
    } catch(const std::bad_alloc& e){
        throw std::runtime_error("Memory allocation error 0_-");
    }
}
SkewNode::~SkewNode() noexcept{
    delete[] data;
}

SkewNode::SkewNode(const SkewNode& other) : priority(other.priority), right(nullptr), left(nullptr) {
    try {
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);

        if (other.right) {
            right = new SkewNode(*other.right);
        }
        if (other.left) {
            left = new SkewNode(*other.left);
        }
    } catch (...) {
        delete[] data;
        delete right;
        delete left;
        throw std::runtime_error("Failed to copy SkewNode");
    }
}

SkewNode& SkewNode::operator=(const SkewNode& other) {
    if (this != &other) {
        char* new_data = nullptr;
        SkewNode* new_right = nullptr;
        SkewNode* new_left = nullptr;

        try {
            new_data = new char[strlen(other.data) + 1];
            strcpy(new_data, other.data);

            if (other.right) {
                new_right = new SkewNode(*other.right);
            }
            if (other.left) {
                new_left = new SkewNode(*other.left);
            }

            delete[] data;
            delete right;
            delete left;

            data = new_data;
            right = new_right;
            left = new_left;
            priority = other.priority;

        } catch (...) {
            delete[] new_data;
            delete new_right;
            delete new_left;
            throw std::runtime_error("Failed to assign SkewNode");
        }
    }
    return *this;
}

SkewNode* SkewPriorityQueue::merge_nodes(SkewNode* a, SkewNode* b){
    if (!a) return b;
    if (!b) return a;

    if (a->priority < b->priority) std::swap(a, b);

    a->right = merge_nodes(a->right, b);
    std::swap(a->left, a->right);

    return a;
}
void SkewPriorityQueue::clear(SkewNode* node){
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}
SkewPriorityQueue::SkewPriorityQueue():root(nullptr), size(0){}
SkewPriorityQueue::~SkewPriorityQueue() noexcept{
    clear(root);
}

SkewNode* SkewPriorityQueue::copy_tree(const SkewNode* node) const {
    if (!node) return nullptr;
    try {
        SkewNode* new_node = new SkewNode(*node);
        return new_node;
    } catch (...) {
        throw std::runtime_error("Failed to copy SkewNode tree");
    }
}

SkewPriorityQueue::SkewPriorityQueue(const SkewPriorityQueue& other) : root(nullptr), size(other.size) {
    try {
        root = copy_tree(other.root);
    } catch (...) {
        clear(root);
        throw std::runtime_error("Failed to copy SkewPriorityQueue");
    }
}

SkewPriorityQueue& SkewPriorityQueue::operator=(const SkewPriorityQueue& other) {
    if (this != &other) {
        SkewNode* new_root = nullptr;

        try {
            new_root = copy_tree(other.root);

            // Если копирование прошло успешно
            clear(root);
            root = new_root;
            size = other.size;

        } catch (...) {
            clear(new_root);
            throw std::runtime_error("Failed to assign SkewPriorityQueue");
        }
    }
    return *this;
}

void SkewPriorityQueue::insert(int priority, const char* data) {
    try{
        SkewNode* newNode = new SkewNode(data, priority);
        root = merge_nodes(root, newNode);
        ++size;
    }catch(const std::bad_alloc& e) {
        throw std::runtime_error("Memory allocation error 0_-");
    }
}

const char* SkewPriorityQueue::peek() const {
    if (!root) throw std::runtime_error("Queue is empty");
    return root->data;
}
void SkewPriorityQueue::remove() {
    if (!root) throw std::runtime_error("Queue is empty");
    SkewNode* oldRoot = root;
    root = merge_nodes(root->left, root->right);
    delete oldRoot;
    --size;
}
PriorityQueue& SkewPriorityQueue::merge(PriorityQueue& other) {
    SkewPriorityQueue& otherQueue = dynamic_cast<SkewPriorityQueue&>(other);
    if (!otherQueue.root) {
        return *this;
    }
    root = merge_nodes(root, otherQueue.root);
    size += otherQueue.size;
    otherQueue.size = 0;
    otherQueue.root = nullptr;

    return *this;
}
void SkewPriorityQueue::print(SkewNode* node, int depth=0) const{
    if (!node) return;

    print(node->right, depth + 1);
    std::cout << node->priority << ": " << node->data << "\n";
    print(node->left, depth + 1);
}
void SkewPriorityQueue::print() const{
    print(root);
}
