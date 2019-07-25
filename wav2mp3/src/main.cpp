//
// Created by Mircea Gita on 2019-07-22.
//

#include "dir_iterator.h"
#include "encoder.h"
#include "thread_pool.h"

int main () {
    // Create two threads
    thread_pool tp (1);

    std::string dirPath ("/Users/mircea.gita/work/testWav/");
    dir_iterator dirIterator(dirPath);
    encoder wavEncoder(tp);
    dirIterator.iterate (wavEncoder);

    return 0;
}