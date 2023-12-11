#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>


using namespace std;

struct WordFrequency {
    string word;
    int frequency;
    WordFrequency(const string& w, int f) : word(w), frequency(f) {}
};
class DynStringStack {

    private:
        StackNode* top;

    public:
        DynStringStack(void);
        void push(string);
        void pop(string&);
        bool isEmpty(void) const;
};

DynStringStack::DynStringStack(void) {
    top = NULL;
}

void DynStringStack::push(string word) {

    StackNode* newNode;
    // Allocate a new node & store word
    newNode = new StackNode; 
    newNode->value = word; 

    // If there are no nodes in the list make newNode the first node
    if (isEmpty()) { 
        top = newNode; 
        newNode->next = NULL; 
    }
    else {	// Otherwise, insert NewNode before top
        newNode->next = top; 
        top = newNode; 
    }
}

void DynStringStack::pop(string& word) {
    StackNode* temp;

    if (isEmpty()) {
        cout << "The stack is empty.\n";
    }
    else {	// pop value off top of stack
        word = top->value;
        temp = top->next;
        delete top;
        top = temp;
    }
}
bool DynStringStack::isEmpty(void) const {
    bool status;

    if (top == NULL)
        status = true;
    else
        status = false;

    return status;
}


