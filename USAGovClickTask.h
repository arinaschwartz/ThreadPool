/*
 * CMSC 12300 - Computer Science with Applications 3
 * Borja Sotomayor, 2013
 *
 * Implementation of the Task abstract class which processes
 * a single 1.usa.gov click, collecting some basic information
 * from the click and aggregating it using the USAGovClickData class.
 *
 */

#ifndef USAGOVCLICKTASK_H_
#define USAGOVCLICKTASK_H_

#include "USAGovClick.h"
#include "USAGovClickData.h"
#include "Task.h"

class USAGovClickTask: public Task {
	// The click to be processed in this task
	USAGovClick click;

	// A reference to the aggregate data (which will be
	// modified using the contents of the click)
	USAGovClickData &data;

	// Artificial duration of the task
	// (the task will sleep for this many milliseconds before
	// it completes its work)
	int duration;

public:
	// Constructor
	USAGovClickTask(USAGovClick click, USAGovClickData &data, int duration);

	// Destructor
	virtual ~USAGovClickTask();

	// Implementation of the abstract run() method.
	// Processes the data in the click.
	void run();
};

#endif /* USAGOVCLICKTASK_H_ */
