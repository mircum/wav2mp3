//
// Created by Mircea Gita on 2019-07-24.
//

#include <dirent.h>

#include "file_iterator.h"
#include "encoder.h"

using namespace std;

file_iterator::file_iterator (const string &dir_path) :
dir_path_ (dir_path) {
}

file_iterator::~file_iterator () {
}

int file_iterator::iterate (encoder &encoder) {
    DIR *dir = opendir (dir_path_.c_str ());
    struct dirent *entry;
    if (dir != nullptr) {

        while ((entry = readdir (dir)) != nullptr) {
            // skip if folder
            if (entry->d_type == DT_DIR)
                continue;

            string filePath = dir_path_ + entry->d_name;
            encoder.encode (filePath.c_str());
        }
        closedir (dir);
    } else {
        perror ("could not open directory\n");
        return 1;
    }
    return 0;
}
