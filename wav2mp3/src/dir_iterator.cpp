//
// Created by Mircea Gita on 2019-07-24.
//

#include <dirent.h>

#include "dir_iterator.h"
#include "encoder.h"

using namespace std;

file_iterator::file_iterator (const string &_dirPath) :
_dirPath (_dirPath) {
}

file_iterator::~file_iterator () {
}

int file_iterator::iterate (encoder &encoder) {
    DIR *dir = opendir (_dirPath.c_str ());
    struct dirent *entry;
    if (dir != nullptr) {

        while ((entry = readdir (dir)) != nullptr) {
            // skip if folder
            if (entry->d_type == DT_DIR)
                continue;

            string filePath = _dirPath + entry->d_name;
            encoder.encode (filePath.c_str());
        }
        closedir (dir);
    } else {
        perror ("could not open directory\n");
        return 1;
    }
    return 0;
}
