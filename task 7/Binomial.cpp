#include <iostream>
#include <cstring>
#include <stdexcept>
#include "base.h"

class BinomialHeap : public PriorityQueue {
private:
    struct Node {
        int key;
        char* data;
        Node* next;
        Node* child;

        Node(int k, const char* d) : key(k), next(nullptr), child(nullptr) {
            try {
                data = new char[strlen(d) + 1];
            } catch (const std::bad_alloc& e) {
                std::cerr << "Error memory allocation: " << e.what() << std::endl;
                throw;
            }
            strcpy(data, d);
        }

        ~Node() {
            delete[] data;
            Node* currentChild = child;
            while (currentChild != nullptr) {
                Node* nextChild = currentChild->next;
                delete currentChild;
                currentChild = nextChild;
            }
        }
    };

    Node* head;

    static Node* mergeTrees(Node* tree1, Node* tree2) {
        if (tree1 == nullptr) return tree2;
        if (tree2 == nullptr) return tree1;

        if (tree1->key < tree2->key) {
            tree2->next = tree1->child;
            tree1->child = tree2;
            return tree1;
        } else {
            tree1->next = tree2->child;
            tree2->child = tree1;
            return tree2;
        }
    }

    static Node* mergeHeaps(Node* heap1, Node* heap2) {
        if (heap1 == nullptr) return heap2;
        if (heap2 == nullptr) return heap1;

        Node* result = nullptr;
        Node* current = nullptr;

        while (heap1 != nullptr && heap2 != nullptr) {
            Node* temp;
            if (heap1->key < heap2->key) {
                temp = heap1;
                heap1 = heap1->next;
            } else {
                temp = heap2;
                heap2 = heap2->next;
            }

            if (result == nullptr) {
                result = temp;
                current = temp;
            } else {
                current->next = temp;
                current = temp;
            }
        }

        if (heap1 != nullptr) {
            current->next = heap1;
        } else {
            current->next = heap2;
        }

        return result;
    }

    static Node* reverseHeap(Node* heap) {
        Node* prev = nullptr;
        Node* current = heap;
        Node* next = nullptr;

        while (current != nullptr) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        return prev;
    }

public:
    BinomialHeap() : head(nullptr) {}

    BinomialHeap(const BinomialHeap& other) = delete;
    BinomialHeap& operator=(const BinomialHeap& other) = delete;

    ~BinomialHeap() noexcept {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void print() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << "Key: " << current->key << ", Value: " << current->data << std::endl;
            current = current->next;
        }
    }

    void insert(int key, const char* value) override {
        if (value == nullptr) {
            throw std::invalid_argument("Value cannot be null in insert");
        }
        try {
            Node* newNode = new Node(key, value);
            head = mergeHeaps(head, newNode);
        } catch (const std::bad_alloc& e) {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            throw;
        }
    }

    const char* findMin() const override {
        if (head == nullptr) {
            throw std::runtime_error("Queue is empty in findMin");
        }
        Node* minNode = head;
        Node* current = head->next;

        while (current != nullptr) {
            if (current->key < minNode->key) {
                minNode = current;
            }
            current = current->next;
        }

        return minNode->data;
    }

    void remove() override {
        if (head == nullptr) {
            throw std::runtime_error("Queue is empty in deleteMin");
        }

        Node* minNode = head;
        Node* prevMinNode = nullptr;
        Node* current = head;
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->key < minNode->key) {
                minNode = current;
                prevMinNode = prev;
            }
            prev = current;
            current = current->next;
        }

        if (prevMinNode != nullptr) {
            prevMinNode->next = minNode->next;
        } else {
            head = minNode->next;
        }

        Node* reversedChildren = nullptr;
        if (minNode->child != nullptr) {
            reversedChildren = reverseHeap(minNode->child);
        }

        head = mergeHeaps(head, reversedChildren);

        minNode->child = nullptr;
        minNode->next = nullptr;
        delete minNode;
    }

    PriorityQueue& merge(PriorityQueue& other) override {
        BinomialHeap& otherQueue = dynamic_cast<BinomialHeap&>(other);
        head = mergeHeaps(head, otherQueue.head);
        otherQueue.head = nullptr;
        return *this;
    }
};

int main() {
    try {
        BinomialHeap queue1;
        queue1.insert(5, "Task 5");
        queue1.insert(3, "Task 3");
        queue1.insert(7, "Task 7");

        BinomialHeap queue2;
        queue2.insert(2, "Task 2");
        queue2.insert(4, "Task 4");

        std::cout << "Queue 1:" << std::endl;
        queue1.print();

        std::cout << "Queue 2:" << std::endl;
        queue2.print();

        queue1.merge(queue2);

        std::cout << "Merged Queue:" << std::endl;
        queue1.print();

        std::cout << "Min value: " << queue1.findMin() << std::endl;

        queue1.remove();
        std::cout << "After deleteMin:" << std::endl;
        queue1.print();

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    }

    return 0;
}