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

    void do_encode ();

protected:
    std::string resolve_out_file_name (const std::string &file_name);

private:
    bool encode_;
    std::unique_ptr<wave_header> wh_;
    std::string th_id_;
    lame_t lame_;
    FILE *in_;
    FILE *out_;
};


#endif //WAV2MP3_ENCODER_H
