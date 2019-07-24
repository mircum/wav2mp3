//
// Created by Mircea Gita on 2019-07-24.
//

#ifndef WAV2MP3_DIRITERATOR_H
#define WAV2MP3_DIRITERATOR_H

#include <string>

class encoder;
class dir_iterator final {
public:
    //dir_iterator (std::string &dirPath);
    dir_iterator (const std::string &_dirPath);
    int iterate (encoder &encoder);

    ~dir_iterator ();

protected:
private:
    dir_iterator () = delete;

private:
    std::string _dirPath;

};


#endif //WAV2MP3_DIRITERATOR_H
