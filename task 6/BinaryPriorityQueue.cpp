#include "BinaryPriorityQueue.hpp"
#include <algorithm>
#include <cstring>

BinaryPriorityQueue::Node::Node(int p, const char* d) : priority(p){
    try{
        data = new char[strlen(d)+1];
        strcpy(data, d);
    } catch(const std::bad_alloc& e){
        throw std::runtime_error("Memory allocation error T_T");
    }
}

BinaryPriorityQueue::Node::~Node(){
    delete[] data;
}

BinaryPriorityQueue::BinaryPriorityQueue() : size(0), capacity(10){
    try{
        heap = new Node*[capacity];
    }catch(const std::bad_alloc& e){
        throw std::runtime_error("Error allocation memory T_T");
    }
}

BinaryPriorityQueue::~BinaryPriorityQueue() noexcept{
    for (size_t i = 0; i < size; i++){
        delete heap[i];
    }
    delete[] heap;
}

void BinaryPriorityQueue::resize(){
    try{
        capacity*=2;
        Node** newHeap = new Node*[capacity];
        for (size_t i = 0; i<size; ++i){
            newHeap[i] = heap[i];
        }

        delete [] heap;
        heap = newHeap;

    } catch(const std::bad_alloc& e){
        throw std::runtime_error("Memory allocation failed during resize");
    }
}

void BinaryPriorityQueue::heapUp(int index){
    while(index > 0){
        int parent = (index-1)/2;
        if (heap[parent]->priority >= heap[index]->priority){
            break;
        }else{
            std::swap(heap[parent], heap[index]);
            index = parent;
        }
    }
}

void BinaryPriorityQueue::heapDown(int index){
    while(true){
        int left = index * 2 + 1;
        int right = index * 2 + 2;
        int root = index;

        if (left < size && heap[left]->priority > heap[index]->priority){
            root = left;
        }

        if (right < size && heap[right]->priority > heap[index]->priority){
            root = right;
        }

        if (root == index) break;

        std::swap(heap[index], heap[root]);
        index = root;
    }
}

void BinaryPriorityQueue::insert(int priority, const char* data){
    try{
        if (size==capacity){
            resize();
        }
        heap[size] = new Node(priority, data);
        heapUp(size);
        ++size;
    }catch (const std::bad_alloc& e) {
        throw std::runtime_error("Memory allocation failed during insert U_U");
    }catch (const std::exception& e) {
        throw std::runtime_error("Failed to insert element L_L");
    }

}
const char* BinaryPriorityQueue::peek() const{
        if (size == 0){
            throw std::runtime_error("Priority queue is empty I_I");
        }
        return heap[0]->data;
}
void BinaryPriorityQueue::remove(){
        if (size == 0){
            throw std::runtime_error("Priority queue is empty I_I");
        }
        try {
            delete heap[0];
            heap[0] = heap[--size];
            heapDown(0);
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to delete max element V_V");
        }
}
PriorityQueue& BinaryPriorityQueue::merge(PriorityQueue& other){
    BinaryPriorityQueue* otherQueue = dynamic_cast<BinaryPriorityQueue*>(&other);
    if (!otherQueue){
        throw std::invalid_argument("Failed while dinamic_cast J_J");
    }

    try{
        for (int i = 0; i<otherQueue->size; ++i){
            insert(otherQueue->heap[i]->priority, otherQueue->heap[i]->data);
        }
    } catch(const std::exception& e){
        throw std::runtime_error("Error during merge N_N");
    }

    return *this;
}
