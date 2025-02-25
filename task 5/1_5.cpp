#include <iostream>
#include <execution>
#include <cstring>

class PriorityQueue{
private:
    struct Node{
        int priority;
        char* data;
        Node* next;

        Node(int priority, const char* str){
            data = new char[strlen(str)+1];
            strcpy(data, str);
        }

        ~Node() {
            delete[] data;
        }
    };
    Node* head;
public:
    PriorityQueue() : head(nullptr) {};

    PriorityQueue(const PriorityQueue& other){
        Node* current = other.head;
        while (current){
            enqueue(current->priority, current->data);
            current = current->next;
        }
    }

    PriorityQueue& operator=(const PriorityQueue& other){
        if (this != &other){
            Node* current = other.head;
            while (current) {
                enqueue(current->priority, current->data);
                current = current->next;
            }
        }
        return *this;
    }

    ~PriorityQueue(){
        clear();
    }
    void clear(){
        while(head){
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void error() const{
        if (!head){
            throw std::runtime_error("Queue is empty Y_Y");
        }
    }

    PriorityQueue& enqueue(int priority, const char* data){
        Node* new_node = new Node(priority, data);
        if (!head || priority < head->priority){
            new_node->next = head;
            head = new_node;
        }
        else{
            Node* current = head;
            while(current->next && current->next->priority <= priority){
                current = current->next;
            }
            new_node->next = current->next;
            current->next = new_node;
        }
        return *this;
    }

    const char* peek() const{
        error();
        return head->data;
    }

    PriorityQueue& dequeue(){
        error();
        Node* temp = head;
        head = head->next;
        delete temp;
        return *this;
    }

    PriorityQueue& merge(PriorityQueue& other){
        while(other.head){
            enqueue(other.head->priority, other.head->data);
            other.dequeue();
        }
        return *this;
    }
};