/*
 * CMSC 12300 - Computer Science with Applications 3
 * Borja Sotomayor, 2013
 *
 * Abstract class to represent a task to be run by the thread pool.
 *
 */

#ifndef TASK_H_
#define TASK_H_

class Task {
public:
	// Constructor
	Task();

	// Destructor
	virtual ~Task();

	// Abstract method. Whatever the task does must be
	// implemented inside this method in the subclasses.
	virtual void run() = 0;
};

#endif /* TASK_H_ */
