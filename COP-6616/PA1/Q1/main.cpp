#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <bits/stdc++.h> 
#include <chrono> 
#include <queue> 
#include <atomic>

using namespace std;

#define NUM_THREADS 10
#define LIMIT 100000000
#define LARGEST_NUMBER 10

int counter = 2;
long long	 sum = 0;
int primesFound = 0;
list<int> sorted;

sem_t sumLock;
sem_t counterLock;
sem_t arrayLock;
sem_t primeLock;
sem_t sortedLock;

struct threadData {
	int thread_id;
};

int getNextJob() {
	int r;
	sem_wait(&counterLock);	// begin critical section
	r = counter;
	if (counter == 2) counter = 3;
	else counter += 2;
	sem_post(&counterLock);	// end critical section
	
	return r;
}

bool isSmallest(int n) {
	list<int>::iterator it = sorted.begin();
	for (; it!=sorted.end(); it++) {
		if (n > *it) {
			return false;
		}
	}
	return true;
}

void addToQueue(int prime) {
	sem_wait(&sortedLock);
	sorted.sort();
	if (sorted.size() == LARGEST_NUMBER) {
		sorted.pop_front();
	}
	sorted.push_back(prime);
	sem_post(&sortedLock);
}

bool isPrime(int n) {
	for (int i = 2; i <= n/2; i++) {
		if (n%i == 0) return false;
	}
	return true;
}

void addToSum(int prime) {
	sem_wait(&sumLock);
	sum += prime;
	sem_post(&sumLock);
}

void incrementPrimeCount() {
	sem_wait(&primeLock);
	primesFound++;
	sem_post(&primeLock);
}

void* runJob(void*) {
	while (1) {
		int toCheck = getNextJob();
		if (toCheck > LIMIT) break;
		if (isPrime(toCheck)) {
			addToQueue(toCheck);
			addToSum(toCheck);
			incrementPrimeCount();
		}
	}

	pthread_exit(NULL);
}

int main() {
	pthread_t threads[NUM_THREADS];
	struct threadData td[NUM_THREADS];

	sem_init(&counterLock, 0, 1);
	sem_init(&arrayLock, 0, 1);
	sem_init(&sumLock, 0, 1);
	sem_init(&primeLock, 0, 1);
	sem_init(&sortedLock, 0, 1);

	auto start = chrono::high_resolution_clock::now(); 
	for (int i = 0; i < NUM_THREADS; i++) {
		cout << "main(): creating thread: " << i << endl;
		td[i].thread_id = i;
		int rc = pthread_create(&threads[i], NULL, runJob, NULL);
	}

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL); 

	auto end = chrono::high_resolution_clock::now(); 
	double timeElapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count(); 
	timeElapsed *= 1e-9; 

	cout << fixed << timeElapsed << setprecision(5);
	cout << ' ' << primesFound; 
	cout << ' ' << sum << endl;

	sorted.sort();
	list<int>::iterator it = sorted.begin();
	for (; it!=sorted.end(); it++) {
		cout << *it << ' ';
	}
	cout << endl;
}