/*
 * CMSC 12300 - Computer Science with Applications 3
 * Borja Sotomayor, 2013
 *
 * Implementation of the Task abstract class which processes
 * a single 1.usa.gov click, collecting some basic information
 * from the click and aggregating it using the USAGovClickData class.
 *
 * See USAGovClickTask.h for details.
 */

#include "USAGovClickTask.h"
#include <thread>

USAGovClickTask::USAGovClickTask(USAGovClick click, USAGovClickData &data, int duration):
		Task(),click(click),data(data),duration(duration) {

}

USAGovClickTask::~USAGovClickTask() {

}

void USAGovClickTask::run() {
	if (duration>0)
	    std::this_thread::sleep_for( std::chrono::milliseconds(duration) );
	data.updateData(click);
}
