/*
 * CMSC 12300 - Computer Science with Applications 3
 * Borja Sotomayor, 2013
 *
 * Process the 1.usa.gov click data (http://www.usa.gov/About/developer-resources/1usagov.shtml)
 * using a thread pool.
 *
 * This file is the entry point into the program. When run, the program
 * either reads a file with click data, or connects to the live stream.
 *
 * It accepts the following command-line parameters:
 *
 *  -f FILE: File with JSON representation of clicks, as returned by
 *           the live stream.
 *
 *  -w: Connect to the live stream instead of using a file.
 *
 *  -t THREADS: Number of threads to use in the thread pool.
 *
 *  -d DURATION: Artificially make the tasks in the thread pool run
 *               for this long (in milliseconds)
 *
 *  -i INTERVAL: Artificially introduce an interval (in milliseconds)
 *               between reading each new click. This only makes sense
 *               when reading from a file (when reading from the live
 *               stream, there is already a natural interval between clicks)
 *
 *  -v: Verbose mode. Prints out more messages to the console.
 *
 */

#include "USAGovClickReader.h"
#include "USAGovClickFileReader.h"
#include "USAGovClickHTTPReader.h"
#include "USAGovClickData.h"
#include "USAGovClick.h"
#include "USAGovClickTask.h"
#include "ThreadPool.h"
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
using namespace std;

const string USAGOVSTREAMURL = "http://developer.usa.gov/1usagov";
const string USAGE = "(-f FILE | -w ) [-t THREADS] [-d DURATION] [-i INTERVAL] [-v]";

int main(int argc, char* argv[])
{
	// For parsing command-line options
	int opt;
	// Should we be verbose?
	bool verbose = false;
	// Are we connecting to the live stream?
	bool live = false;
	// File with click data
	string file = "";
	fstream f;
	// Number of threads
	int nthreads = 1;
	// Duration of each task (milliseconds)
	int duration = 0;
	// Interval between tasks (milleconds)
	int interval = 0;
	// Number of tasks dropped by the thread pool
	int dropped = 0;
	// Click reader
	USAGovClickReader *reader;
	// Data object
	USAGovClickData data;

	// Parse command-line options
	while ((opt = getopt(argc, argv, "f:wt:d:i:vh")) != -1)
		switch (opt)
		{
			case 'f':
				file = optarg;
				live = false;
				break;
			case 'w':
				live = true;
				break;
			case 't':
				nthreads = atoi(optarg);
				break;
			case 'd':
				duration = atoi(optarg);
				break;
			case 'i':
				interval = atoi(optarg);
				break;
			case 'v':
				verbose = true;
				break;
			default:
				cerr << "Usage: " << argv[0] << " " << USAGE << endl;
				cerr << "ERROR: Unknown option -" << char(opt) << endl;
				exit(-1);
		}

	if(live && file != "")
	{
		cerr << "Usage: " << argv[0] << " " << USAGE << endl;
		cerr << "ERROR: Cannot specify both the -f and -w options." << endl;
		exit(-1);
	}

	if(!live && file == "")
	{
		cerr << "Usage: " << argv[0] << " " << USAGE << endl;
		cerr << "ERROR: Must specify either the -f or -w option." << endl;
		exit(-1);
	}

	// Create the reader
	if(file != "")
	{
		f.open(file);
		reader = new USAGovClickFileReader(f);
	}
	else
	{
		reader = new USAGovClickHTTPReader(USAGOVSTREAMURL);
		signal(SIGINT, sigint_handler);
	}

	struct timespec start, finish;
	double elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);

	// Create thread pool
	ThreadPool pool(nthreads);

	int n=0;
	while(!reader->done())
	{
		USAGovClick click;
		try
		{
			click = reader->next();
		} catch (USAGovClickReaderException &e)
		{
			cerr << "Error reading click: " << e.what();
			continue;
		}

		USAGovClickTask *task = new USAGovClickTask(click, data, duration);

		if(pool.schedule(task))
		{
			if(verbose)
				cout << "Task " << n+1 << " scheduled." << endl;
		}
		else
		{
			dropped++;
			if(verbose)
				cout << "Task " << n+1 << " NOT scheduled." << endl;
		}

		n++;

		if(interval>0)
			std::this_thread::sleep_for( std::chrono::milliseconds(interval) );
	}
	pool.stop();

	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	if (verbose)
	{
		cout << endl << endl;
		cout << "Running time: " << elapsed << endl;
		cout << "Received " << n << " clicks." << endl;
		cout << "Dropped " << dropped << " clicks." << endl;
		cout << "Processed " << data.getNumClicks() << " clicks." << endl << endl;

		cout << data.getNumNew() << " of the clicks were from new users" << endl << endl;

		cout << "Breakdown by country:" << endl;
		stringCountMap countries = data.getCountries();
		for(auto country : countries)
		{
			string countryName = country.first==""? "??": country.first;
			cout << countryName << " " << country.second << endl;
		}
	}
	else
	{
		cout << file << "," << nthreads << "," << elapsed << "," << n << "," << dropped << endl;
	}

}
