//
// Created by Mircea Gita on 2019-07-30.
//


#include <exception>

#include "wave_header.h"

using namespace std;

wave_header::wave_header (FILE *file) {
    if (file == nullptr) {
        throw invalid_argument("FILE cannot be nullptr");
    }
    
    int read = 0;
    unsigned char buffer4[4];
    unsigned char buffer2[2];

    // read header_ parts
    read = fread (header_.riff, sizeof (header_.riff), 1, file);
    read = fread (buffer4, sizeof (buffer4), 1, file);

    // convert little endian to big endian 4 byte int
    header_.overall_size =   buffer4[0]|
                            (buffer4[1]<<8)|
                            (buffer4[2]<<16)|
                            (buffer4[3]<<24);

    read = fread (header_.wave, sizeof (header_.wave), 1, file);

    read = fread (header_.fmt_chunk_marker, sizeof (header_.fmt_chunk_marker), 1, file);

    read = fread (buffer4, sizeof (buffer4), 1, file);
    // convert little endian to big endian 4 byte integer
    header_.length_of_fmt =  buffer4[0]|
                            (buffer4[1]<<8)|
                            (buffer4[2]<<16)|
                            (buffer4[3]<<24);

    read = fread (buffer2, sizeof (buffer2), 1, file);
    header_.format_type = buffer2[0]|(buffer2[1]<<8);

    read = fread (buffer2, sizeof (buffer2), 1, file);
    header_.channels = buffer2[0]|(buffer2[1]<<8);
 
    read = fread (buffer4, sizeof (buffer4), 1, file);
    header_.sample_rate = buffer4[0]|
                        (buffer4[1]<<8)|
                        (buffer4[2]<<16)|
                        (buffer4[3]<<24);

    read = fread (buffer4, sizeof (buffer4), 1, file);
    header_.byterate =   buffer4[0]|
                        (buffer4[1]<<8)|
                        (buffer4[2]<<16)|
                        (buffer4[3]<<24);

    read = fread (buffer2, sizeof (buffer2), 1, file);
    header_.block_align = buffer2[0]|(buffer2[1]<<8);

    read = fread (buffer2, sizeof (buffer2), 1, file);
    header_.bits_per_sample = buffer2[0]|(buffer2[1]<<8);

    read = fread (header_.data_chunk_header, sizeof (header_.data_chunk_header), 1, file);

    read = fread (buffer4, sizeof (buffer4), 1, file);
    header_.data_size =  buffer4[0]|
                        (buffer4[1]<<8)|
                        (buffer4[2]<<16)|
                        (buffer4[3]<<24);

    /*
    // calculate no.of samples
    long num_samples = (8*header_.data_size)/(header_.channels*header_.bits_per_sample);
    printf ("Number of samples:%lu \n", num_samples);

    long size_of_each_sample = (header_.channels*header_.bits_per_sample)/8;
    printf ("Size of each sample:%ld bytes\n", size_of_each_sample);

    // calculate duration of file
    float duration_in_seconds = (float)header_.overall_size/header_.byterate;
    printf ("Approx.Duration in seconds=%f\n", duration_in_seconds);
    printf ("Approx.Duration in h:m:s=%s\n", seconds_to_time (duration_in_seconds));
    */
}

wave_header::~wave_header () {
}

unsigned int wave_header::size ()
{
    return header_.overall_size;
}

bool wave_header::is_wave ()
{
    // the canonical WAVE format starts with the RIFF header
    return strstr((char*)header_.riff, "RIFF") != nullptr ? true : false;
}

bool wave_header::is_pcm ()
{
    return (header_.format_type == 1) ? true : false;
}

unsigned int wave_header::channels ()
{
    return header_.channels;
}

unsigned int wave_header::sample_rate ()
{
    return header_.sample_rate;
}

unsigned int wave_header::bits_per_sample ()
{
    return header_.bits_per_sample;
}

void *wave_header::buffer ()
{
    return nullptr;
}
