/*
 * CMSC 12300 - Computer Science with Applications 3
 * Borja Sotomayor, 2013
 *
 * USAGovClickData is a simple object that aggregates data
 * about multiple 1.usa.gov clicks.
 *
 * See USAGovClickData.h for details
 *
 */

#include "USAGovClickData.h"
#include <thread>
using namespace std;

USAGovClickData::USAGovClickData():numClicks(0),numNew(0) {

}

USAGovClickData::~USAGovClickData() {

}

void USAGovClickData::updateData(USAGovClick &click)
{
    clickdata_mutex.lock();
	numClicks++;
	if(!click.isKnown())
		numNew++;
	countries[click.getCountryCode()] += 1;
    clickdata_mutex.unlock();
}

stringCountMap& USAGovClickData::getCountries() {
	return countries;
}

int USAGovClickData::getNumClicks() {
	return numClicks;
}

int USAGovClickData::getNumNew() {
	return numNew;
}


