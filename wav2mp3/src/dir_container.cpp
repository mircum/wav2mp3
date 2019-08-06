//
// Created by Mircea Gita on 2019-07-24.
//

#include <system_error>

#include "dir_container.h"

using namespace std;

dir_container::dir_container (const string &dir_path) :
dir_ (nullptr) {

    dir_ = opendir (dir_path.c_str ());
    if (dir_ == nullptr) {
        throw system_error(ENOTDIR, generic_category (), "DIR could not be opened");
    }
}

dir_container::~dir_container () {
    if (dir_)
        closedir (dir_);
}

dir_container::const_iterator dir_container::begin () const {
    rewinddir (dir_);
    return dir_container::const_iterator (dir_);
}

dir_container::const_iterator dir_container::end () const{
    return {};
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
    if (entry_ == nullptr) {
        throw runtime_error("readdir() failed in dir_container::const_iterator::const_iterator");
    }
}

struct dir_container::entry dir_container::const_iterator::operator* () const {
    entry ent;
    ent.name_ = entry_->d_name;
    ent.is_file_ = (entry_->d_type == DT_REG);
    return ent;
}

dir_container::const_iterator &dir_container::const_iterator::operator++ () {
    errno = 0;
    entry_ = readdir (dir_);
    if (!entry_ && errno != 0) {
        throw runtime_error("readdir() failed in dir_container::const_iterator::operator++");
    }
    return *this;
}

bool dir_container::const_iterator::operator!= (const dir_container::const_iterator &other) const {
    return entry_ != other.entry_;
}

