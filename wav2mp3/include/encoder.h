//
// Created by Mircea Gita on 2019-07-24.
//

#ifndef WAV2MP3_ENCODER_H
#define WAV2MP3_ENCODER_H

#include <string>

#include "lame.h"

class encoder final {
    // WAVE file header format
    struct HEADER {
        unsigned char riff[4];                      // RIFF string
        unsigned int overall_size;                  // overall size of file in bytes
        unsigned char wave[4];                      // WAVE string
        unsigned char fmt_chunk_marker[4];          // fmt string with trailing null char
        unsigned int length_of_fmt;                 // length of the format data
        unsigned int format_type;                   // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
        unsigned int channels;                      // no.of channels
        unsigned int sample_rate;                   // sampling rate (blocks per second)
        unsigned int byterate;                      // SampleRate * NumChannels * BitsPerSample/8
        unsigned int block_align;                   // NumChannels * BitsPerSample/8
        unsigned int bits_per_sample;               // bits per sample, 8- 8bits, 16- 16 bits etc
        unsigned char data_chunk_header[4];         // DATA string or FLLR string
        unsigned int data_size;                     // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
    };

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
    size_t read_wave_header ();
    bool is_riff ();
    bool is_pcm ();
    void init_lame ();
    std::string resolve_out_file_name (const std::string &file_name);

private:
    bool encode_;
    HEADER header_;
    std::string th_id_;
    lame_t lame_;
    FILE *in_;
    FILE *out_;
};


#endif //WAV2MP3_ENCODER_H
