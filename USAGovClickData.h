/*
 * CMSC 12300 - Computer Science with Applications 3
 * Borja Sotomayor, 2013
 *
 * USAGovClickData is a simple object that aggregates data
 * about multiple 1.usa.gov clicks.
 *
 */

#ifndef USAGOVCLICKDATA_H_
#define USAGOVCLICKDATA_H_

#include<string>
#include<map>
#include<thread>
#include"USAGovClick.h"

// Typedef maps of strings to ints.
// This is used to keep a count of how many times
// a country code has appeared.
typedef std::map<std::string, int> stringCountMap;

class USAGovClickData {
	// Total number of clicks
	int numClicks;

	// Number of clicks from new users
	int numNew;

	// Number of times each country code appears
	stringCountMap countries;

    std::mutex clickdata_mutex;
	
public:
	// Constructor
	USAGovClickData();

	// Destructor
	virtual ~USAGovClickData();

	// Take a single click and update the aggregate data accordingly
	void updateData(USAGovClick &click);

	// GETTERS
	std::map<std::string, int>& getCountries();
	int getNumClicks();
	int getNumNew();
};

#endif /* USAGOVCLICKDATA_H_ */
