#include <iostream>
#include "inc/SkewHeap.hpp"

int main() {
    SkewPriorityQueue queue;

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
