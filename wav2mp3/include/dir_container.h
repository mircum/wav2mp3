//
// Created by Mircea Gita on 2019-07-24.
//

#ifndef WAV2MP3_DIRCONTAINER_H
#define WAV2MP3_DIRCONTAINER_H

#include <string>
#include <dirent.h>

class dir_container final {

public:
    struct entry {
        std::string name_;
        bool is_file_;
    };

    class const_iterator {
    public:
        const_iterator ();
        const_iterator (const const_iterator &other);
        const_iterator (const_iterator &&other) noexcept;
        const_iterator & operator= (const const_iterator &other);
        const_iterator & operator= (const_iterator &&other) noexcept;
        explicit const_iterator (DIR *dir);
        struct entry operator*() const;
        const_iterator &operator++();
        bool operator!=(const const_iterator &other) const;

    private:
        DIR *dir_;
        struct dirent *entry_;
    };

public:

    explicit dir_container (const std::string &dir_path);
    ~dir_container ();
    const_iterator begin() const;
    const_iterator end() const;

    dir_container () = delete;
    dir_container (const dir_container &) = delete;
    dir_container (dir_container &&) = delete;
    dir_container & operator= (const dir_container &) = delete;
    dir_container & operator= (dir_container &&) = delete;

private:
    DIR *dir_;
};

#endif //WAV2MP3_DIRCONTAINER_H
