#include <string>

#ifndef DYNSTACK_H
#define DYNSTACK_H

struct StackNode {
	string value;   
	StackNode* next;  
};

class DynStack {
private:
	StackNode* top;

public:
	DynStack(void); 
	void push(int); 
	void pop(int&); 
	bool isEmpty(void) const; 
};

#endif