/*
 * CMSC 12300 - Computer Science with Applications 3
 * Borja Sotomayor, 2013
 *
 * An implementation of the thread pool pattern.
 * This file contains the implementation of the worker threads
 *
 */

#ifndef WORKERTHREAD_H_
#define WORKERTHREAD_H_

#include<thread>
#include"Task.h"

// Forward declaration of the ThreadPool class
class ThreadPool;

class WorkerThread {
	// The worker thread includes a reference to the
	// thread pool to be able to access its queue
	// and check whether there are any tasks to work on.
	ThreadPool &pool;

	// The actual thread
	std::thread t;

	// Make the ThreadPool our friend. It needs to
	// modify the "done" variable.
	friend class ThreadPool;

public:
	// Constructor
	WorkerThread(ThreadPool &pool);

	// Destructor
	virtual ~WorkerThread();

	// Start the thread
	void start();
private:
	// The function that will be run by the thread.
	void run();
};

#endif /* WORKERTHREAD_H_ */
