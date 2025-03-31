#include <iostream>
#include "include/LeftistHeap.h"

int main() {
    LeftistPriorityQueue queue;

    queue.insert(10, "low priority");
    queue.insert(30, "medium priority");
    queue.insert(40, "high priority");
    queue.insert(50, "very high priority");

    std::cout << "Priority queue after insertions:\n";
    queue.print();

    std::cout << "\nTop element: " << queue.peek() << "\n";

    queue.remove();
    std::cout << "\nPriority queue after removing top element:\n";
    queue.print();

    return 0;
}