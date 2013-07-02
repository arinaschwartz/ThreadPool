/*
 * CMSC 12300 - Computer Science with Applications 3
 * Borja Sotomayor, 2013
 *
 * Implementation of the USAGovClickReader abstract class
 * that connects to the 1.usa.gov live stream over HTTP.
 *
 */

#ifndef USAGOVCLICKHTTPREADER_H_
#define USAGOVCLICKHTTPREADER_H_

#include "USAGovClickReader.h"
#include <curl/curl.h>
#include <thread>
#include <string>
#include <queue>

void sigint_handler(int signum);

class USAGovClickHTTPReader: public USAGovClickReader {
	// URL of the live stream
	std::string url;
	// cURL "object"
	CURL *curl;
	// Should we stop?
	bool stop;
	// Click retrieval thread
	std::thread curlThread;
	// Queue of clicks retrieved so far
	std::queue<USAGovClick> q;
	// Mutex and condition variable to access queue
	std::mutex m;
	std::condition_variable cvQueueNonEmpty;

	// Has a SIGINT signal been received?
	static bool sigint;

	// We make the signal handler our friend. Otherwise,
	// it can't modify the sigint variable
	friend void sigint_handler(int signum);

public:
	// Constructor
	USAGovClickHTTPReader(std::string url);

	// Destructor
	virtual ~USAGovClickHTTPReader();

	// CURL handlers
    static size_t handle(char * data, size_t size, size_t nmemb, void * p);
    size_t handle_impl(char * data, size_t size, size_t nmemb);

	// Implementation of USAGovClickReader abstract methods.
	// See USAGovClickReader.h for details.
	bool done();
	USAGovClick next();
private:
	// Thread function
	void run();
};

#endif /* USAGOVCLICKHTTPREADER_H_ */
