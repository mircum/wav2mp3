//
// Created by Mircea Gita on 2019-07-24.
//

#include <functional>
#include <iostream>

#include "thread_pool.h"


using namespace std;

thread_pool::thread_pool (unsigned int nbThreads) : shutdown_ (false) {
    threads_.reserve (nbThreads);
    for (int i = 0; i<nbThreads; ++i)
        threads_.emplace_back (bind (&thread_pool::threadEntry, this, i));
}

thread_pool::~thread_pool () {
    {
        // Unblock any threads and tell them to stop
        std::unique_lock <std::mutex> l (lock_);

        shutdown_ = true;
        condVar_.notify_all ();
    }

    // Wait for all threads to stop
    std::cerr<<"Joining threads"<<std::endl;
    for (auto &thread:threads_)
        thread.join ();
}

void thread_pool::addJob (std::function <void (void)> func) {
    // Place a job on the queu and unblock a thread
    std::unique_lock <std::mutex> l (lock_);

    jobs_.emplace (std::move (func));
    condVar_.notify_one ();
}

void thread_pool::threadEntry (int i) {
    std::function <void (void)> job;

    while (1) {
        {
            std::unique_lock <std::mutex> l (lock_);

            while (!shutdown_&&jobs_.empty ())
                condVar_.wait (l);

            if (jobs_.empty ())
            {
                // No jobs to do and we are shutting down
                std::cerr<<"Thread "<<i<<" terminates"<<std::endl;
                return;
            }

            std::cerr<<"Thread "<<i<<" does a job"<<std::endl;
            job = std::move (jobs_.front ());
            jobs_.pop ();
        }
        // Do the job without holding any locks
        job ();
    }
}

