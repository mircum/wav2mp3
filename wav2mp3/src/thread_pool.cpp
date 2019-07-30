//
// Created by Mircea Gita on 2019-07-24.
//

#include <iostream>

#include "thread_pool.h"

using namespace std;

thread_pool::thread_pool (unsigned int nt) : shutdown_ (false) {
    threads_.reserve (nt);
    for (int i = 0; i<nt; ++i)
        threads_.emplace_back (bind (&thread_pool::thread_entry, this, i));
}

thread_pool::~thread_pool () {
    {
        // Unblock any threads and tell them to stop
        unique_lock<mutex> l (lock_);

        shutdown_ = true;
        cond_var_.notify_all ();
    }

    // Wait for all threads to stop
    cerr << "Joining threads" << endl;
    for (auto &thread:threads_)
        thread.join ();
}

void thread_pool::add_job (function<void (void)> func) {
    // Place a job on the queue and unblock a thread
    unique_lock<mutex> l (lock_);

    jobs_.emplace (move (func));
    cond_var_.notify_one ();
}

void thread_pool::thread_entry (int i) {
    function <void (void)> job;
    while (true) {
        {
            unique_lock<mutex> l (lock_);

            while (!shutdown_ && jobs_.empty ())
                cond_var_.wait (l);

            if (jobs_.empty ()) {
                // No jobs to do and we are shutting down
                cerr << "Thread " << i << " terminates" << endl;
                return;
            }

            cerr << "Thread " << i << " does a job" << endl;
            job = move (jobs_.front ());
            jobs_.pop ();
        }
        // Do the job without holding any locks
        job ();
    }
}

