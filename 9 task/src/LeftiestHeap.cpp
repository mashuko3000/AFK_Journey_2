//
// Created by mvideo on 28.03.2025.
//

#include "../include/LeftistHeap.h"
#include <stdexcept>
#include <cstring>
#include <iostream>

LeftistNode::LeftistNode(const char* d, int p) : priority(p), rank(1), left(nullptr), right(nullptr){
    try{
        data = new char[strlen(d+1)];
        strcpy(data, d);
    } catch(const std::bad_alloc& e){
        throw std::runtime_error("Memory allocation error O_-");
    }
}
/*
LeftistNode::LeftistNode(const LeftistNode& other)
        : priority(other.priority), rank(other.rank),
          left(nullptr), right(nullptr) {
    data = new char[strlen(other.data) + 1];
    strcpy(data, other.data);

    if (other.left) {
        left = new LeftistNode(*other.left);
    }
    if (other.right) {
        right = new LeftistNode(*other.right);
    }
}

LeftistNode& LeftistNode::operator=(const LeftistNode& other) {
    if (this != &other) {
        delete[] data;
        delete left;
        delete right;

        priority = other.priority;
        rank = other.rank;

        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);

        left = other.left ? new LeftistNode(*other.left) : nullptr;
        right = other.right ? new LeftistNode(*other.right) : nullptr;
    }
    return *this;
}
*/
LeftistNode::~LeftistNode(){
    delete[] data;
}

LeftistPriorityQueue::LeftistPriorityQueue() : root(nullptr), size(0){}

void LeftistPriorityQueue::clear(LeftistNode* node){
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
};

LeftistPriorityQueue::~LeftistPriorityQueue() noexcept{
    clear(root);
}

int LeftistPriorityQueue::get_rank(LeftistNode* node){
    return (node!=nullptr) ? node->rank : 0;
}

LeftistNode* LeftistPriorityQueue::merge_nodes(LeftistNode* a, LeftistNode* b){
    if (!a) return b;
    if (!b) return a;

    if (a->priority < b->priority) std::swap(a, b);

    a->right = merge_nodes(a->right, b);

    if (get_rank(a->left) < get_rank(a->right)) {
        std::swap(a->left, a->right);
    }

    a->rank = get_rank(a->right) + 1;
    return a;
}

void LeftistPriorityQueue::insert(int priority, const char* data) {
    try {
        LeftistNode *newNode = new LeftistNode(data, priority);
        root = merge_nodes(root, newNode);
        ++size;
    } catch (const std:: bad_alloc& e){
        throw std::runtime_error("Memory allocation error 0_0");
    }catch(const std::exception& e){
        std::cerr<< "Error: " << e.what() << std::endl;
    }catch(...){
        std::cerr<< "Unknown error"<< std::endl;
    }
}
const char* LeftistPriorityQueue::peek() const{
    if (!root) throw std::runtime_error("Queue is empty");
    return root->data;
}
void LeftistPriorityQueue::remove(){
    if(!root) throw std::runtime_error("Queue is empty");

    LeftistNode* oldRoot = root;
    root = merge_nodes(root->left, root->right);
    oldRoot->left = nullptr;
    oldRoot->right = nullptr;
    delete oldRoot;
    --size;
}
PriorityQueue& LeftistPriorityQueue::merge(PriorityQueue& other){
    auto otherQueue = dynamic_cast<LeftistPriorityQueue*>(&other);
    if (!otherQueue){
        throw std::invalid_argument("Failed while dinamic_cast J_J");
    }
    root = merge_nodes(root, otherQueue->root);
    size+=otherQueue->size;
    otherQueue->size = 0;
    otherQueue->root = nullptr;

    return* this;
}

void LeftistPriorityQueue::print(LeftistNode* node, int depth = 0) const {
    if (!node) return;

    print(node->right, depth + 1);
    std::cout << std::string(depth * 4, ' ') << node->priority << ": " << node->data << "\n";
    print(node->left, depth + 1);
}

void LeftistPriorityQueue::print() const {
    print(root);
}
