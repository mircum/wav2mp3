//
// Created by Mircea Gita on 2019-07-24.
//

#ifndef WAV2MP3_ENCODER_H
#define WAV2MP3_ENCODER_H

#include <string>
#include <fstream>
#include "lame.h"

class wave_file;
class encoder final {
public:
    explicit encoder(const std::string &file_path);
    ~encoder();
    encoder () = delete;
    encoder (const encoder &) = delete;
    encoder (encoder &&) = delete;
    encoder & operator= (const encoder &) = delete;
    encoder & operator= (encoder &&) = delete;

    static int encode (const std::string &filePath);

protected:
    static int doEncode(const std::string &filePath);
    static std::string getFileExt(const std::string& filePath);
    static bool validate (const std::string &filePath);

private:
    wave_file &wave_file_;
    std::string file_path_;
    lame_t lame_;
    std::ifstream in_;
    std::ofstream out_;
};


#endif //WAV2MP3_ENCODER_H
