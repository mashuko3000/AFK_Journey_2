#include <iostream>
#include <cstring>
#include <stdexcept>
#include <new> 

class PriorityQueue final{
private:
    struct Node{
        int priority;
        char* data;
        Node* next;
        Node(int p, const char* str) : priority(p), next(nullptr){
            try{
                data = new char [strlen(str)+1];
                strcpy(data, str);
            } catch(const std::bad_alloc& e){
                std::cerr << "Memory allocation error: " << e.what()<<std::endl;
            }
        }
        ~Node(){
            delete[] data;
        }
    };

    Node* head;

    void clear(){
        while(head){
            Node* temp = head;
            head = temp->next;
            delete temp;
        }
    }

    void copy(const PriorityQueue& other){
        Node* otherCurrent = other.head;
        Node* prev = nullptr;

        while (otherCurrent) {
            try{
                Node* newNode = new Node(otherCurrent->priority, otherCurrent->data);
                if (!head) {
                    head = newNode;
                } else {
                    prev->next = newNode;
                }
                prev = newNode;
                otherCurrent = otherCurrent->next;
            }catch(const std::bad_alloc& e){
                std::cerr << "Memory allocation error: " << e.what()<<std::endl;
            }
        }
    }

public:
    PriorityQueue() : head(nullptr){}

    PriorityQueue(const PriorityQueue& other) : head(nullptr){
        try{
            copy(other);
        }catch(const std::bad_alloc&e){
            clear();
            throw;
        }
    }

    PriorityQueue& operator=(const PriorityQueue& other){
        if (this != &other){
            clear();
            try{
                copy(other);
            }catch(const std::bad_alloc& e){
                clear();
                throw;
            }
        }
        return *this;
    }

    ~PriorityQueue(){
        clear();
    }

    PriorityQueue& push(int priority, const char* data){
        if (!data) {
            throw std::invalid_argument("data cannot be null");
        }

        Node* newNode = nullptr;
        try {
            newNode = new Node(priority, data);
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("Memory allocation failed");
        }

        if (!head || head->priority < priority) {
            newNode->next = head;
            head = newNode;
        } else {
            Node* current = head;
            while (current->next && current->next->priority >= priority) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }

        return *this;
    }

    const char* top() const{
        if(head == nullptr){
            throw std::runtime_error("PriorityQueue is Empty I_I");
        }
        return head->data;
    }

    void pop(){
        if(head == nullptr){
            throw std::runtime_error("PriorityQueue is Empty I_I");
        }
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    PriorityQueue& merge(const PriorityQueue& other) {
        PriorityQueue tempQueue;
        Node* thisCurrent = head;
        Node* otherCurrent = other.head;

        try {
            while (thisCurrent && otherCurrent) {
                if (thisCurrent->priority >= otherCurrent->priority) {
                    tempQueue.push(thisCurrent->priority, thisCurrent->data);
                    thisCurrent = thisCurrent->next;
                } else {
                    tempQueue.push(otherCurrent->priority, otherCurrent->data);
                    otherCurrent = otherCurrent->next;
                }
            }

            while (thisCurrent) {
                tempQueue.push(thisCurrent->priority, thisCurrent->data);
                thisCurrent = thisCurrent->next;
            }

            while (otherCurrent) {
                tempQueue.push(otherCurrent->priority, otherCurrent->data);
                otherCurrent = otherCurrent->next;
            }
        } catch (const std::bad_alloc& e) {
            tempQueue.clear();
            throw std::runtime_error("Memory allocation failed during merge");
        }

        *this = tempQueue;
        return *this;
    }
    bool empty() const {
        return head == nullptr;
    }
};

int main() {
    try {
        PriorityQueue pq1;
        pq1.push(3, "High").push(1, "Low").push(2, "Middle");

        PriorityQueue pq2;
        pq2.push(4, "Hiest").push(0, "Lowest");

        pq1.merge(pq2);

        while (!pq1.empty()) {
            std::cout << pq1.top() << std::endl;
            pq1.pop();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}