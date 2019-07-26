//
// Created by Mircea Gita on 2019-07-24.
//

#ifndef WAV2MP3_DIRITERATOR_H
#define WAV2MP3_DIRITERATOR_H

#include <string>

class encoder;
class file_iterator final {
public:
    //dir_iterator (std::string &dirPath);
    file_iterator (const std::string &_dirPath);
    int iterate (encoder &encoder);

    ~file_iterator ();

protected:
private:
    file_iterator () = delete;

private:
    std::string _dirPath;

};


#endif //WAV2MP3_DIRITERATOR_H
