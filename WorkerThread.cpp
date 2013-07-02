#include "ThreadPool.h"
#include "WorkerThread.h"
#include<thread>
#include<iostream>
#include<mutex>
using namespace std;

WorkerThread::WorkerThread(ThreadPool &pool):pool(pool) {

}

WorkerThread::~WorkerThread() {

}

void WorkerThread::start()
{
	// Create the thread and make it run the run() function
	t = thread(&WorkerThread::run, this);
}

void WorkerThread::run()
{
    Task *task;
    while(true){
        {
            unique_lock<mutex>
                lock(pool.m);
            while(pool.q.empty()){
                pool.cvQueueNonEmpty.wait(lock);
            }
            task = pool.q.front();
            pool.q.pop();
        }
        (*task).run();
    }
}
