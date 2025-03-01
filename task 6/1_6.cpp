#include "BinaryPriorityQueue.hpp"
#include <iostream>

int main(){
    try{
        BinaryPriorityQueue queue;

        queue.insert(0, "Zero");
        queue.insert(1, "One");
        queue.insert(2, "Two");
        queue.insert(3, "Three");
        queue.insert(4, "Four");

        std::cout << "Max Element: " << queue.peek() << std::endl;


        queue.remove();
        std::cout << "Max Element after deleting another max: " << queue.peek() << std::endl;

        BinaryPriorityQueue anotherQueue;
        anotherQueue.insert(5, "Five");
        anotherQueue.insert(40, "Forty");

        queue.merge(anotherQueue);

        std::cout << "Max Element after merging: " << queue.peek() << std::endl;

        queue.remove();
        std::cout << "Max Element after deleting another max: " << queue.peek() << std::endl;

    } catch (const std::exception& e){
        std::cerr<<"Error: "<<e.what()<<std::endl;
    }

    return 0;
}