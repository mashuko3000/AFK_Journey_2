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

    void clear(){
        while(head){
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    PriorityQueue& enqueue(int priority, const char* data){
        Node* new_node = new Node(priority, data);
        if (!head || priority < head->priority){
            new_node->next = head;
            head = new_node;
        }
    }

    const char* peek() const{
        if (!head){
            throw std::runtime_error("Queue is empty Y_Y");
        }
        return head->data;
    }


};