//
// Created by Mircea Gita on 2019-07-22.
//
#include "dir_container.h"
#include "encoder.h"
#include "thread_pool.h"
#include "logger.h"

using namespace std;

void wav2mp3 (const string &dir_path, const string &file_name) {
    encoder enc (dir_path, file_name);
    enc.encode ();
}

int main (int argc, char *argv[]) {
    //argc = 2;

    if (argc<2) {
        logger::error ("ERROR: Insufficient parameters. Usage: wav2mp3 path/to/dir");
        return 1;
    }

    try {
        string dir_path (argv[1]);
        string msg = "Scanning " + dir_path + " directory.";
        logger::log (msg);

        // get number of concurrent thread that can be executed
        unsigned int nt = thread::hardware_concurrency ();
        msg = to_string(nt) + " concurrent threads possible on this system.";
        logger::log (msg);


        dir_container dc(dir_path);
        // create a thread_pool with c_threads - 1 number of threads
        // main execution thread should be considered a concurrent activity
        thread_pool tp (nt-1);

        for (auto cit : dc) {
            dir_container::entry ent = cit;
            if (!ent.is_file_)
                continue;
            //tp.add_job (bind (wav2mp3, dir_path, ent.name_));
            wav2mp3 (dir_path, ent.name_);
        }
    }
    catch (exception e) {
        logger::log ("exception");
    }

    return 0;
}

