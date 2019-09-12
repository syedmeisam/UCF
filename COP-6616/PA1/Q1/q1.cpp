#include <iostream>
#include <pthread.h>
#include <bits/stdc++.h> 
#include <chrono> 
#include <queue> 
#include <atomic>
#include <semaphore.h>

using namespace std;

#define NUM_THREADS 1
#define LIMIT 100000000
#define LARGEST_NUMBER 10

int N = 2;
std::atomic<int> counter(N);
long long sum = 0;
std::atomic<long long> sum_counter(sum);
int primesFound = 0;
std::atomic<int> prime_counter(primesFound);

list<int> sorted;
sem_t sortedLock;


struct threadData {
	int thread_id;
};

int getNextJob() {
	int r;
	r = counter;
	if (counter == 2) counter = 3;
	else {
		if (counter < LIMIT) atomic_fetch_add(&counter, 2);
	}
	return r;
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
	atomic_fetch_add(&sum_counter, (long long) prime);
}

void incrementPrimeCount() {
	atomic_fetch_add(&prime_counter, 1);
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

	auto start = chrono::high_resolution_clock::now(); 
	for (int i = 0; i < NUM_THREADS; i++) {
		td[i].thread_id = i;
		int rc = pthread_create(&threads[i], NULL, runJob, NULL);
	}

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL); 

	auto end = chrono::high_resolution_clock::now(); 
	double timeElapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count(); 
	timeElapsed *= 1e-9; 

	cout << fixed << timeElapsed << setprecision(5);
	cout << ' ' << prime_counter; 
	cout << ' ' << sum_counter << endl;

	sorted.sort();
	list<int>::iterator it = sorted.begin();
	for (; it!=sorted.end(); it++) {
		cout << *it << ' ';
	}
	cout << endl;
}