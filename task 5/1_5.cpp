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
};