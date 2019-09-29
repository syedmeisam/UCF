#include <iostream>
#include <pthread.h>
#include <bits/stdc++.h> 
#include <chrono> 
#include <queue> 
#include <atomic>
#include <thread>

using namespace std;

class Stack {
public:
	class Node {
	public:
		int val;
		Node* next;

		Node(int _t) {
			val = _t;
		}
	};

	// member variables
	std::atomic<Node*> head;
	std::atomic<int> numOps;
	std::atomin<int> size;
	
	// constructor
	Stack() {
		head= NULL;
		numOps = 0;
	}

	void sequentialPush(int x) {
		Node* n = new Node(x);
		n->next = head;
		head = n;
	}

	bool push(int x) {
		Node* oldHead = head;
  		Node* newNode = new Node(x);
  		newNode->next = oldHead;

  		while (!head.compare_exchange_weak(oldHead,newNode)) {
    		newNode->next = oldHead;
  		}
  		atomic_fetch_add(&numOps, 1);
    	return true;
	}

	int pop() {
		Node* oldHead = head.load(memory_order_acquire);
		Node* newHead;
		do {
	        if(oldHead == NULL) {
	        	return NULL;
	        }
        	newHead = oldHead->next;
    	} while(!head.compare_exchange_weak(oldHead, newHead, memory_order_acquire));
    	atomic_fetch_add(&numOps, 1);
    	return oldHead->val;
	}

	void print() {
		Node* n = head;
		for(;n!=NULL;n=n->next) {
			cout << n->val << ' ';
		}
		cout << endl;
	}
};

void *runJob(Stack* stack) {
	for (int i=0; i<150000; i++) {
		double num = rand();

		if (num > 0.5) {
			// call push
			stack->push(i);
		} else {
			// call pop
			stack->pop();
		}
	}
	pthread_exit(NULL);
}

int main() {
	Stack* stack = new Stack();
	srand (time(NULL));
	// use sequential push to prepopuulate the list
	// this does not change the value of numOps
	for (int i = 0; i < 50; i++) {
		stack->sequentialPush(i);
	}

	pthread_t threads[4];

	// spawn 4 threads
	thread t1(runJob, stack);
	thread t2(runJob, stack);
	thread t3(runJob, stack);
	thread t4(runJob, stack);

	t1.join();
	t2.join();
	t3.join();
	t4.join();	

	cout << stack->numOps << endl;
}