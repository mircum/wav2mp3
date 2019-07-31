//
// Created by Mircea Gita on 2019-07-24.
//

#ifndef WAV2MP3_ENCODER_H
#define WAV2MP3_ENCODER_H

#include <string>
#include <fstream>
#include "lame.h"

class wave_header;
class encoder final {
public:
    explicit encoder (const std::string &dir_path, const std::string &file_name);
    ~encoder();
    encoder () = delete;
    encoder (const encoder &) = delete;
    encoder (encoder &&) = delete;
    encoder & operator= (const encoder &) = delete;
    encoder & operator= (encoder &&) = delete;

    int encode (const std::string &filePath);

protected:
    int doEncode(const std::string &filePath);
    std::string getFileExt(const std::string& filePath);
    std::string get_out_file_name (const std::string &file_name);
    bool validate (const std::string &filePath);

private:
//    wave_header &wave_file_;
    std::string th_id_;
    lame_t lame_;
    FILE *in_;
    FILE *out_;
};


#endif //WAV2MP3_ENCODER_H
