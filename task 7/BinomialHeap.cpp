//
// Created by mvideo on 29.03.2025.
//
#include "../inc/BinomialHeap.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <cassert>
#include <cstring>

BinomialPriorityQueue::Node::Node(const char* value, int prio) : priority(prio), sibling(nullptr), child(nullptr), degree(0) {
    try {
        // Allocate memory for data and copy the string
        size_t len = strlen(value) + 1;
        data = new char[len];
        strcpy(data, value);
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("Memory allocation error: ");
    }
}

BinomialPriorityQueue::Node::~Node() {
    delete[] data;
}

// Constructor
BinomialPriorityQueue::BinomialPriorityQueue() : head(nullptr) {}

// Copy constructor
BinomialPriorityQueue::BinomialPriorityQueue(const BinomialPriorityQueue& other) : head(nullptr) {
    try {
        if (other.head) {
            head = copyNodes(other.head);
            //Make circular lists
            Node* current = head;
            while(current) {
                makeCircularChildList(current);
                current = current->sibling;
            }
        }
    } catch (const std::exception& e) {
        clear();
        throw std::runtime_error(std::string("Copy constructor failed: ") + e.what());
    }
}

// Assignment operator
BinomialPriorityQueue& BinomialPriorityQueue::operator=(const BinomialPriorityQueue& other) {
    if (this != &other) {
        try {
            BinomialPriorityQueue temp(other);
            std::swap(head, temp.head);
            //Make circular lists
            Node* current = head;
            while(current){
                makeCircularChildList(current);
                current = current->sibling;
            }
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("Assignment operator failed: ") + e.what());
        }
    }
    return *this;
}

// Destructor
BinomialPriorityQueue::~BinomialPriorityQueue() {
    clear();
}

BinomialPriorityQueue::Node* BinomialPriorityQueue::copyNodes(const Node* node) {
    if (!node) return nullptr;

    try {
        Node* newNode = new Node(node->data, node->priority);
        newNode->degree = node->degree;

        // Copy child
        if (node->child) {
            newNode->child = copyNodes(node->child);
            makeCircularChildList(newNode);
        }

        // Copy sibling
        if (node->sibling) {
            newNode->sibling = copyNodes(node->sibling);
        }

        return newNode;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Node copy failed: ") + e.what());
    }
}

// Make child list circular
void BinomialPriorityQueue::makeCircularChildList(Node* parent) {
    if (!parent || !parent->child) return;

    if (isCircularChildList(parent)) {
        std::cout << "Already circular, skipping" << std::endl;
        return;
    }

    Node* current = parent->child;
    while (current->sibling && current->sibling != parent->child) {
        current = current->sibling;
    }

    current->sibling = parent->child; // Make it circular
}

bool BinomialPriorityQueue::isCircularChildList(Node* parent) const {
    if (!parent || !parent->child) return true;

    Node* slow = parent->child;
    Node* fast = parent->child;

    while (fast && fast->sibling) {
        slow = slow->sibling;
        fast = fast->sibling->sibling;

        if (slow == fast) return true;
    }

    return false;
}

// Clear the queue
void BinomialPriorityQueue::clear() {
    if (head) {
        deleteAllNodes(head);
        head = nullptr;
    }
}

// Delete all nodes recursively
void BinomialPriorityQueue::deleteAllNodes(Node* node) {
    if (!node) return;

    // Break circular list if needed
    if (node->child) {
        Node* current = node->child;
        Node* start = current;
        Node* next = nullptr;

        do {
            next = current->sibling;
            if (next == start) {
                current->sibling = nullptr;
                break;
            }
            current = next;
        } while (current != start);
    }

    // Delete child nodes
    if (node->child) {
        deleteAllNodes(node->child);
    }

    // Delete sibling nodes
    if (node->sibling) {
        deleteAllNodes(node->sibling);
    }

    delete node; // Delete node itself
}
// Check if queue is empty
bool BinomialPriorityQueue::isEmpty() const {
    return head == nullptr;
}

// Link two binomial trees of the same degree
void BinomialPriorityQueue::linkNodes(Node* y, Node* z) {
    if (!z->child) {
        // Если у z нет детей, создаем кольцо из y
        y->sibling = y;  // y указывает сам на себя
    } else {
        // Встраиваем y в кольцевой список детей z
        y->sibling = z->child->sibling;
        z->child->sibling = y;
    }
    z->child = y;  // Теперь y — первый ребенок z
    z->degree++;
}

// Merge two heaps
BinomialPriorityQueue::Node* BinomialPriorityQueue::mergeHeaps(Node* h1, Node* h2) {
    if (!h1) return h2;
    if (!h2) return h1;

    Node* result = nullptr;
    Node** lastPtr = &result;

    while (h1 && h2) {
        if (h1->degree <= h2->degree) {
            *lastPtr = h1;
            h1 = h1->sibling;
        } else {
            *lastPtr = h2;
            h2 = h2->sibling;
        }
        lastPtr = &((*lastPtr)->sibling);
    }

    if (h1) *lastPtr = h1;
    else *lastPtr = h2;

    return result;
}

// Union two heaps
BinomialPriorityQueue::Node* BinomialPriorityQueue::unionHeaps(Node* h1, Node* h2) {
    Node* heap = mergeHeaps(h1, h2);
    if (!heap) return nullptr;

    Node* prev = nullptr;
    Node* current = heap;
    Node* next = current->sibling;

    while (next) {
        if (current->degree != next->degree ||
            (next->sibling && next->sibling->degree == current->degree)) {
            prev = current;
            current = next;
        } else {
            if (current->priority <= next->priority) {
                current->sibling = next->sibling;
                linkNodes(next, current);
            } else {
                if (!prev) {
                    heap = next;
                } else {
                    prev->sibling = next;
                }
                linkNodes(current, next);
                current = next;
            }
        }
        next = current->sibling;
    }
    return heap;
}

// Find the node with minimum priority
BinomialPriorityQueue::Node* BinomialPriorityQueue::findMinNode() const {
    if (!head) return nullptr;

    Node* min = head;
    Node* current = head->sibling;

    while (current) {
        if (current->priority < min->priority) {
            min = current;
        }
        current = current->sibling;
    }

    return min;
}

BinomialPriorityQueue::Node* BinomialPriorityQueue::removeMinNode() {
    if (!head) return nullptr;

    // Находим минимальный узел
    Node* min = head;
    Node* minPrev = nullptr;
    Node* current = head;
    while (current->sibling) {
        if (current->sibling->priority < min->priority) {
            min = current->sibling;
            minPrev = current;
        }
        current = current->sibling;
    }

    // Удаляем минимальный узел из корневого списка
    if (minPrev) {
        minPrev->sibling = min->sibling;
    } else {
        head = min->sibling;
    }

    // Обрабатываем детей минимального узла (разрываем кольцо)
    Node* newHead = nullptr;
    if (min->child) {
        Node* firstChild = min->child;
        Node* child = firstChild;
        do {
            Node* nextChild = child->sibling;
            child->sibling = newHead;
            newHead = child;
            child = nextChild;
        } while (child != firstChild);  // Проходим всё кольцо
    }

    // Объединяем основную кучу с кучей из детей
    head = unionHeaps(head, newHead);
    return min;
}

// Insert a new element
void BinomialPriorityQueue::insert(const char* data, int priority) {
    try {
        Node* newNode = new Node(data, priority);

        // Create a new heap with just the new node
        BinomialPriorityQueue tempHeap;
        tempHeap.head = newNode;

        // Merge the new heap with the current heap
        merge(tempHeap);

        // Prevent double deletion
        tempHeap.head = nullptr;
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("Memory allocation failed during insert");
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Insert failed: ") + e.what());
    }
}

// Get the minimum element without removing it
char* BinomialPriorityQueue::peek() const {
    if (isEmpty()) {
        throw std::runtime_error("Cannot peek from an empty queue");
    }

    Node* minNode = findMinNode();
    if (!minNode) {
        throw std::runtime_error("Error finding minimum node");
    }

    // Return a copy of the data
    size_t len = strlen(minNode->data) + 1;
    char* result = new char[len];
    strcpy(result, minNode->data);

    return result;
}

// Remove and return the minimum element
char* BinomialPriorityQueue::remove() {
    if (isEmpty()) {
        throw std::runtime_error("Cannot remove from an empty queue");
    }

    try {
        Node* minNode = removeMinNode();
        if (!minNode) {
            throw std::runtime_error("Error removing minimum node");
        }

        // Get the data
        size_t len = strlen(minNode->data) + 1;
        char* result = new char[len];
        strcpy(result, minNode->data);

        // Delete the node but keep its data
        char* temp = minNode->data;
        minNode->data = nullptr;  // Prevent data deletion in the destructor
        delete minNode;

        return result;
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("Memory allocation failed during remove");
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Remove failed: ") + e.what());
    }
}

// Merge with another priority queue
void BinomialPriorityQueue::merge(PriorityQueue& otherQueue) {
    BinomialPriorityQueue& other = dynamic_cast<BinomialPriorityQueue&>(otherQueue);
    if (!other.head){
        throw std::invalid_argument("Failed while dinamic_cast J_J");
    }

    try {
        head = unionHeaps(head, other.head);
        other.head = nullptr;  // Prevent double deletion
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Merge failed: ") + e.what());
    }
}


