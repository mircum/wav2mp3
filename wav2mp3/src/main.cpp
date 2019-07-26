//
// Created by Mircea Gita on 2019-07-22.
//

#include <iostream>

#include "dir_iterator.h"
#include "encoder.h"
#include "thread_pool.h"

using namespace std;

int main (int argc, char *argv[]) {

    if (argc<2) {
        cerr<<"Insuficient parameters. Usage: wav2mp3 path/to/dir"<<endl;
        return 1;
    }

    string dir_path (argv[1]);
    file_iterator dirIterator (dir_path);
    // get number of concurent thread that can be executed
    unsigned int c_threads = thread::hardware_concurrency ();   
    // create a thread_pool with c_threads - 1 number of threads
    // main execution thread should be considered a concurent activity
    thread_pool tp (c_threads - 1);


    encoder wavEncoder(tp);
    dirIterator.iterate (wavEncoder);

    return 0;
}