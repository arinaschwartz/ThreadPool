/*
 * CMSC 12300 - Computer Science with Applications 3
 * Borja Sotomayor, 2013
 *
 * An implementation of the thread pool pattern.
 *
 * A thread pool is created with a fixed number of threads
 * and a queue with a size limit. Initially, the queue
 * will be empty and the threads will be idle (waiting
 * for tasks to arrive). While waiting, they should not
 * busy-wait; they should sleep until the queue becomes
 * non-empty.
 *
 * As tasks arrive, they are placed on the queue, and
 * claimed by the next idle thread.
 *
 * If all the threads are busy, this just means the task
 * will have to wait in the queue until it is run. However.
 * if the queue reaches its limit, any new tasks will be
 * rejected.
 *
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include<thread>
#include<queue>
#include<vector>
#include<Task.h>

// Forward declaration of the WorkerThread class
class WorkerThread;

// The thread pool
class ThreadPool {

	// The default maximum number of tasks (used if one is not specified
	// when creating the object
	const static int DEFAULT_MAX_TASKS = 10;

	// The actual maximum number of tasks
	unsigned int maxTasks;

	// The number of worker threads
	unsigned int numWorkers;

	// The worker threads
	std::vector<WorkerThread*> workers;

	// The task queue
	std::queue<Task*> q;

	// A mutex to control access to the queue
	std::mutex m;

	// Condition variable to notify idling threads when the
	// queue goes from empty to non-empy
	std::condition_variable cvQueueNonEmpty;

	// Worker threads are our friends. They need to be able to
	// access the queue and the condition variable.
	friend class WorkerThread;

public:
	// Constructor
	ThreadPool(int numThreads, int maxTasks = DEFAULT_MAX_TASKS);

	// Destructor
	virtual ~ThreadPool();

	// Schedules a single task. Returns true if the task was scheduled,
	// and false otherwise.
	bool schedule(Task *t);

	// Stop the thread pool. This joins all the threads.
	void stop();
};

#endif /* THREADPOOL_H_ */
