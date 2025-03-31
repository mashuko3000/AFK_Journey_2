#include <iostream>
#include <cassert>
#include <cstring>
#include "inc/BinomialHeap.hpp"

void testInsertAndPeek() {
    BinomialPriorityQueue queue;
    queue.insert("Task1", 4);
    queue.insert("Task2", 3);
    queue.insert("Task3", 2);
    queue.insert("Task4", 1);

    char* min = queue.peek();
    assert(strcmp(min, "Task4") == 0);
    delete[] min;
    std::cout << "testInsertAndPeek passed!" << std::endl;
}

void testRemove() {
    BinomialPriorityQueue queue;
    queue.insert("Task1", 5);
    queue.insert("Task2", 3);
    queue.insert("Task3", 4);

    char* min = queue.remove();
    assert(strcmp(min, "Task2") == 0);
    delete[] min;

    min = queue.remove();
    assert(strcmp(min, "Task3") == 0);
    delete[] min;

    min = queue.remove();
    assert(strcmp(min, "Task1") == 0);
    delete[] min;

    assert(queue.isEmpty());
    std::cout << "testRemove passed!" << std::endl;
}

void testMerge() {
    BinomialPriorityQueue queue1;
    queue1.insert("Task1", 5);
    queue1.insert("Task2", 3);

    BinomialPriorityQueue queue2;
    queue2.insert("Task3", 4);
    queue2.insert("Task4", 2);

    queue1.merge(queue2);

    char* min = queue1.remove();
    assert(strcmp(min, "Task4") == 0);
    delete[] min;

    min = queue1.remove();
    assert(strcmp(min, "Task2") == 0);
    delete[] min;

    min = queue1.remove();
    assert(strcmp(min, "Task3") == 0);
    delete[] min;

    min = queue1.remove();
    assert(strcmp(min, "Task1") == 0);
    delete[] min;

    assert(queue1.isEmpty());
    std::cout << "testMerge passed!" << std::endl;
}

int main() {
    try {
        testInsertAndPeek();
        testRemove();
        testMerge();
        std::cout << "All tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
    }
    return 0;
}

