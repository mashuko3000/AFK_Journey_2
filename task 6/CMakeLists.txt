cmake_minimum_required(VERSION 3.20)
project(PriorityQueueProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
        1_6.cpp
        BinaryPriorityQueue.cpp
)

add_executable(PriorityQueueProject ${SOURCES})