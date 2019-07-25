//
// Created by Mircea Gita on 2019-07-22.
//

#include <string>
#include <vector>
#include <iostream>
#include <functional>


#include "dir_iterator.h"
#include "encoder.h"

#include "thread_pool.h"


void silly (int n)
{
    // A silly job for demonstration purposes
    std::cerr<<"Sleeping for "<<n<<" seconds"<<std::endl;
    std::this_thread::sleep_for (std::chrono::seconds (n));
}


int main () {
    std::string dirPath ("/Users/mircea.gita/work/testWav/");
   // dir_iterator dirIterator(dirPath);
   // encoder wavEncoder;
   // dirIterator.iterate (wavEncoder);


        // Create two threads
    thread_pool tp (2);

    // Assign them 4 jobs
    tp.addJob (std::bind (silly, 1));
    tp.addJob (std::bind (silly, 2));
    tp.addJob (std::bind (silly, 3));
    tp.addJob (std::bind (silly, 4));


    return 0;
}