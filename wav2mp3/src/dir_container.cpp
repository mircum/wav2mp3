//
// Created by Mircea Gita on 2019-07-24.
//

#include <iostream>

#include "dir_container.h"
#include "encoder.h"

using namespace std;

dir_container::dir_container (const string &dir_path) {

    dir_ = opendir (dir_path.c_str ());
    if (dir_ == nullptr) {
        throw system_error(ENOTDIR, generic_category (), "Dir could not be opened");
    }
}

dir_container::~dir_container () {
    closedir (dir_);
}

dir_container::const_iterator dir_container::begin () const {
    rewinddir (dir_);
    return dir_container::const_iterator (dir_);
}

dir_container::const_iterator dir_container::end () const{
    return {};// dir_container::const_iterator ();
}

dir_container::const_iterator::const_iterator () :
dir_ (nullptr),
entry_ (nullptr) {

}

dir_container::const_iterator::const_iterator (const dir_container::const_iterator &other) :
dir_ (nullptr),
entry_ (nullptr) {
    dir_ = other.dir_;
    entry_ = other.entry_;
}

dir_container::const_iterator::const_iterator (dir_container::const_iterator &&other) noexcept :
dir_ (nullptr),
entry_ (nullptr) {
    dir_ = other.dir_;
    entry_ = other.entry_;
    other.dir_ = nullptr;
    other.entry_ = nullptr;
}

dir_container::const_iterator &dir_container::const_iterator::operator= (const dir_container::const_iterator &other) {
    if (this != &other) {
        dir_ = other.dir_;
        entry_ = other.entry_;
    }
    return *this;
}

dir_container::const_iterator &dir_container::const_iterator::operator= (dir_container::const_iterator &&other) noexcept {
    if (this != &other) {
        dir_ = other.dir_;
        entry_ = other.entry_;
        other.dir_ = nullptr;
        other.entry_ = nullptr;

    }
    return *this;
}

dir_container::const_iterator::const_iterator (DIR *dir) :
dir_ (dir),
entry_ (nullptr) {

    if (dir_ == nullptr) {
        throw invalid_argument("DIR cannot be nullptr");
    }

    entry_ = readdir (dir_);
    if (entry_ == nullptr && errno != 0) {
        throw runtime_error("readdir() failed");
    }
}

const struct dirent * dir_container::const_iterator::operator* () const {
    return entry_;
}

dir_container::const_iterator &dir_container::const_iterator::operator++ () {
    entry_ = readdir (dir_);
    if (entry_ == nullptr && errno != 0) {
        throw runtime_error("readdir() failed");
    }
    return *this;
}

bool dir_container::const_iterator::operator!= (const dir_container::const_iterator &other) const {
    return entry_ != other.entry_;
}

