/*
 * CMSC 12300 - Computer Science with Applications 3
 * Borja Sotomayor, 2013
 *
 * Implementation of the USAGovClickReader abstract class
 * that connects to the 1.usa.gov live stream over HTTP.
 *
 * See USAGovClickHTTPReader.h for details.
 */


#include "USAGovClickHTTPReader.h"
#include <curl/curl.h>
#include <string>
#include <iostream>
using namespace std;

bool USAGovClickHTTPReader::sigint = false;

void sigint_handler(int signum)
{
	USAGovClickHTTPReader::sigint = true;
}

USAGovClickHTTPReader::USAGovClickHTTPReader(string url):url(url),stop(false) {
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &USAGovClickHTTPReader::handle);

	curlThread = thread(&USAGovClickHTTPReader::run, this);
}

USAGovClickHTTPReader::~USAGovClickHTTPReader() {
	curlThread.join();
    curl_easy_cleanup(curl);
}

// This is mostly a workaround for cURL being a C-only library and
// not liking non-static methods.
size_t USAGovClickHTTPReader::handle(char * data, size_t size, size_t nmemb, void *p)
{
    return static_cast<USAGovClickHTTPReader*>(p)->handle_impl(data, size, nmemb);
}

size_t USAGovClickHTTPReader::handle_impl(char* data, size_t size, size_t nmemb)
{
	// If a sigint has been received, make cURL stop processing clicks
	if(sigint)
		return CURLE_WRITE_ERROR;

	// Read in one line, and parse it (using the USAGovClick class)
	string line(data, size * nmemb);
	USAGovClick click;
	try
	{
		click = USAGovClick(line);
		{
			// Put the click on the queue
			lock_guard<mutex> lock(m);
			q.push(click);
			if(q.size() == 1)
				cvQueueNonEmpty.notify_one();
		}
	} catch (USAGovJSONException &e)
	{
		return size * nmemb;
	}

	return size * nmemb;
}

void USAGovClickHTTPReader::run()
{
	CURLcode res;
	// The following function will keep calling USAGovClickHTTPReader::handle
	// for as long as data keeps on arriving through the HTTP connection.
    res = curl_easy_perform(curl);

    // Once curl_easy_perform returns, we wake up the click reading
    // thread. Otherwise, it might block indefinitely waiting for
    // the queue to be non-empty.
    stop = true;
    cvQueueNonEmpty.notify_one();
}

bool USAGovClickHTTPReader::done()
{
	return stop;
}


USAGovClick USAGovClickHTTPReader::next()
{
	unique_lock<mutex> lock(m);

	while(q.size()==0 && !stop)
		cvQueueNonEmpty.wait(lock);

	if(stop)
	{
		if(sigint)
			throw USAGovClickReaderException("SIGINT signal received.");
		else
			throw USAGovClickReaderException("Stopped for unknown reasons.");
	}

	// Grab next click from the queue.
	USAGovClick click = q.front();
	q.pop();
	lock.unlock();

	return click;
}
