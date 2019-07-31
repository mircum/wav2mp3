//
// Created by Mircea Gita on 2019-07-30.
//
#include <cstdio>
#include <string>

class wave_header final {
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
    explicit wave_header (FILE *file);
    ~wave_header () = default;
    unsigned int size ();
    bool is_wave ();
    bool is_pcm ();
    unsigned int channels ();
    unsigned int sample_rate ();
    unsigned int bits_per_sample ();
    void *buffer ();

    wave_header () = delete;
    wave_header (const wave_header &) = delete;
    wave_header (wave_header &&) = delete;
    wave_header &operator= (const wave_header &) = delete;
    wave_header &operator= (wave_header &&) = delete;

private:
    HEADER header_;
};