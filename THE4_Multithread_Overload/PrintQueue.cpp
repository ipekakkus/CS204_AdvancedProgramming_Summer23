// PrintQueue.cpp
#include "PrintQueue.h"
#include <iostream>
using namespace std;

PrintQueue::PrintQueue() { // Default constructor implementation
    front = nullptr;
    rear = nullptr;
    currentSize = 0;
}

PrintQueue::PrintQueue(const PrintQueue& queue) { // Copy constructor implementation

    this->front = nullptr;
    this->rear = nullptr;
    this->currentSize = queue.currentSize;

    PrintJobNode* temp = nullptr;
    PrintJobNode* search_ptr = queue.front;
    bool boom = true;
    while (search_ptr != nullptr) {
        temp = new PrintJobNode();
        temp->numberOfPages = search_ptr->numberOfPages;
        temp->printJobID = search_ptr->printJobID;
        if (boom) {//if this is the first node mark as front
            this->front = temp;
            boom = false;
        }
        this->rear = temp;
        search_ptr = search_ptr->next;
    }
}

PrintQueue::~PrintQueue() { // Destructor implementation
    int id, page;
    while (!isEmpty()) { // till the queue is empty, it will delete the items
        dequeue(id, page);
    }
}

void PrintQueue::enqueue(int num, int pages) { // Implementation to add a new printJob node to the rear of the queue
    if (isEmpty()) {
        front = new PrintJobNode(num, pages);
        rear = front;
        currentSize++;
    }
    else {
        rear->next = new PrintJobNode(num, pages);
        rear = rear->next;
        currentSize++;

    }
}
void PrintQueue::dequeue(int& num, int& pages) { // Implementation to remove a printJob node from the front of the queue
    PrintJobNode* temp;
    if (isEmpty()) {
        cout << "Attempting to dequeue on empty queue, exiting program...\n";
        exit(1);
    }
    else {
        num = front->printJobID;
        pages = front->numberOfPages;
        temp = front;
        front = front->next;
        delete temp; 
        currentSize--;
    }
}

PrintQueue& PrintQueue::operator+(const PrintJobNode& job) {
    auto* newQueue = new PrintQueue(*this);
    newQueue->enqueue(job.printJobID, job.numberOfPages); // Subtraction operation uses enqueue member function
    return *newQueue;
}
PrintQueue& PrintQueue::operator-(PrintJobNode& job) {
    int id, page;
    this->dequeue(id, page); // Subtraction operation uses dequeue member function
    job.printJobID = id;
    job.numberOfPages = page;
    return *this;
}
PrintQueue& PrintQueue::operator=(const PrintQueue& other) { // Implementation to copy the contents of the other PrintQueue to this PrintQueue
    auto* thing = new PrintQueue(other);
    this->front = thing->front;
    this->rear = thing->rear;
    this->currentSize = thing->currentSize;
    return *this;
}
ostream& operator<<(ostream& os, const PrintQueue& queue) {  // Implementation to output the contents of the PrintQueue
    PrintJobNode* current = queue.front;
    while (current != nullptr) {
        os << "PrintJobID: " << current->printJobID << ", Pages: " << current->numberOfPages << endl;
        current = current->next;
    }
    return os;
}

int PrintQueue::queueSize() const { // Implementation to return the size of the queue
    return currentSize;
}
bool PrintQueue::isEmpty() const { // Implementation to check if the queue is empty
    return front == nullptr;
}

