
// PrintQueue.h
#ifndef PRINTQUEUE_H
#define PRINTQUEUE_H

#include <queue>
#include <mutex>
#include <string>
#include <ostream>

using namespace std;

struct PrintJobNode {
    int printJobID; // Each printJob object will have an ID, when new printJob node is added, the Id should be consecutive
    int numberOfPages;
    PrintJobNode* next;

    PrintJobNode() : printJobID(0), numberOfPages(0), next(nullptr) {}
    PrintJobNode(const int& id, int pages) : printJobID(id), numberOfPages(pages), next(nullptr) {}

};

class PrintQueue {
public:
    PrintJobNode* front = nullptr;
    PrintJobNode* rear = nullptr;

    PrintQueue(); // Default constructor
    PrintQueue(const PrintQueue& queue); // Copy constructor
    ~PrintQueue(); // Destructor for this class

    void enqueue(int  num, int pages); // Adding new printJob nodes to the rear of the queue
    void dequeue(int& num, int& pages); // Removing a printJob node from the front of the queue

    // Overloaded operators.
    PrintQueue& operator+(const PrintJobNode& job); // Operator to enqueue a new PrintJob node to the queue
    PrintQueue& operator-(PrintJobNode& pop);
    PrintQueue& operator=(const PrintQueue& other); // Operator to assign an instance of PrintQueue class to the one another
    friend ostream& operator<<(ostream& os, const PrintQueue& queue);  // Operator to output of this class

    int queueSize() const;
    bool isEmpty() const;

private:
    int currentSize;
};
#endif // PRINTQUEUE_H