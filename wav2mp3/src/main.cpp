//
// Created by Mircea Gita on 2019-07-22.
//

#include <iostream>

#include "dir_container.h"
#include "encoder.h"
#include "thread_pool.h"


using namespace std;

int main (int argc, char *argv[]) {

    if (argc<2) {
        cerr << "Insufficient parameters. Usage: wav2mp3 path/to/dir" << endl;
        return 1;
    }

    // get number of concurrent thread that can be executed
    unsigned int nt = thread::hardware_concurrency ();
    cout << nt << " concurrent threads possible on this system." << endl;
    // create a thread_pool with c_threads - 1 number of threads
    // main execution thread should be considered a concurrent activity
    thread_pool tp (nt-1);

    string dir_path (argv[1]);
    cout << "Encoding wav files in " << dir_path << " directory." << endl;
    dir_container dc(dir_path);
    for (auto cit : dc) {
        if ((*cit).d_type != DT_REG)
            continue;
        string name ((*cit).d_name);
        cout << name << endl;
    }
    return 0;
}
