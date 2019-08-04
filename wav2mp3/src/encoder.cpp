//
// Created by Mircea Gita on 2019-07-24.
//

//#include <functional>
//#include <cstdio>

#include <sstream>
#include <thread>


#include "logger.h"
#include "encoder.h"


const char path_separator =
#ifdef _WIN32
        '\\';
#else
        '/';
#endif

using namespace std;

encoder::encoder (const std::string &dir_path, const std::string &file_name) :
encode_ (false),
in_ (nullptr),
out_ (nullptr),
lame_ (nullptr) {

    // save thread id in a string to use it when logging
    stringstream ss;
    ss << this_thread::get_id();
    th_id_ = ss.str() + "-> ";
    logger::log (th_id_ + "Input file " + file_name);

    // open input file
    string file_path = dir_path + path_separator + file_name;
    in_ = fopen (file_path.c_str(), "rb");
    if (in_ == nullptr) {
        throw system_error (ENOENT, generic_category (), "Input FILE could not be opened");
    }

    // read wave header from input file
    read_wave_header ();
    if (!is_riff ()) {
        logger::log (th_id_ + file_name + " skipped: Not a RIFF WAVE file format");
        return;
    }
    if (!is_pcm ()) {
        logger::error (th_id_ + file_name + " skipped: Not a PCM audio format");
        return;
    }

    // initialize lame library
    init_lame ();

    // create output file with the same name + mp3 extension
    string out = resolve_out_file_name (file_name);
    logger::log (th_id_+"Output file "+out);
    file_path = dir_path+path_separator+out;
    out_ = fopen (file_path.c_str(), "wb");
    if (in_ == nullptr) {
        throw runtime_error ("Output FILE could not be created");
    }

    encode_ = true;
}

encoder::~encoder () {
    if (lame_)
        lame_close (lame_);
    if (out_)
        fclose (out_);
    if (in_)
       fclose (in_);
}

size_t encoder::read_wave_header () {
    if (in_ == nullptr) {
        throw invalid_argument("FILE cannot be nullptr");
    }

    size_t read = 0;
    unsigned char buffer4[4];
    unsigned char buffer2[2];

    // read header_ parts
    read = fread (header_.riff, sizeof (header_.riff), 1, in_);
    read = fread (buffer4, sizeof (buffer4), 1, in_);

    // convert little endian to big endian 4 byte int
    header_.overall_size =   buffer4[0]|
                             (buffer4[1]<<8u)|
                             (buffer4[2]<<16u)|
                             (buffer4[3]<<24u);

    read = fread (header_.wave, sizeof (header_.wave), 1, in_);

    read = fread (header_.fmt_chunk_marker, sizeof (header_.fmt_chunk_marker), 1, in_);

    read = fread (buffer4, sizeof (buffer4), 1, in_);
    // convert little endian to big endian 4 byte integer
    header_.length_of_fmt =  buffer4[0]|
                             (buffer4[1]<<8u)|
                             (buffer4[2]<<16u)|
                             (buffer4[3]<<24u);

    read = fread (buffer2, sizeof (buffer2), 1, in_);
    header_.format_type = buffer2[0]|(buffer2[1]<<8u);

    read = fread (buffer2, sizeof (buffer2), 1, in_);
    header_.channels = buffer2[0]|(buffer2[1]<<8u);

    read = fread (buffer4, sizeof (buffer4), 1, in_);
    header_.sample_rate = buffer4[0]|
                          (buffer4[1]<<8u)|
                          (buffer4[2]<<16u)|
                          (buffer4[3]<<24u);

    read = fread (buffer4, sizeof (buffer4), 1, in_);
    header_.byterate =   buffer4[0]|
                         (buffer4[1]<<8u)|
                         (buffer4[2]<<16u)|
                         (buffer4[3]<<24u);

    read = fread (buffer2, sizeof (buffer2), 1, in_);
    header_.block_align = buffer2[0]|(buffer2[1]<<8u);

    read = fread (buffer2, sizeof (buffer2), 1, in_);
    header_.bits_per_sample = buffer2[0]|(buffer2[1]<<8u);

    read = fread (header_.data_chunk_header, sizeof (header_.data_chunk_header), 1, in_);

    read = fread (buffer4, sizeof (buffer4), 1, in_);
    header_.data_size =  buffer4[0]|
                         (buffer4[1]<<8u)|
                         (buffer4[2]<<16u)|
                         (buffer4[3]<<24u);
    return read;
}

bool encoder::is_riff ()
{
    // the canonical WAVE format starts with the RIFF header
    return strstr((char*)header_.riff, "RIFF") != nullptr;
}

bool encoder::is_pcm ()
{
    return header_.format_type == 1;
}

void encoder::init_lame () {
    lame_ = lame_init ();
    if (lame_ == nullptr) {
        throw runtime_error ("lame_init() failed");
    }

    lame_set_quality (lame_, 5);
    lame_set_in_samplerate (lame_, header_.sample_rate);
    lame_set_num_channels (lame_, header_.channels);
    MPEG_mode mode = MONO;
    if (header_.channels==2)
        mode = STEREO;
    lame_set_mode (lame_, mode);
//    lame_set_VBR (lame_, vbr_default);
    if (-1 == lame_init_params (lame_)) {
        throw runtime_error("lame_init_params() failed");

    }
//    lame_print_config (lame_);
}


#define PCM_BUF_SIZE 1024
#define MP3_SIZE 1024//8192

void encoder::do_encode () {
    if (!encode_)
        return;

    int n_bytes_read;
    int n_bytes_write = 0;
    int i;

    short pcm_buffer_s[PCM_BUF_SIZE];
//    unsigned char pcm_buffer[PCM_BUF_SIZE];
    unsigned char mp3_buffer[MP3_SIZE];

//    long pos = ftell (in_);
//    fseek (in_, pos, SEEK_SET);

    do {
        n_bytes_read = fread (pcm_buffer_s, sizeof (short), PCM_BUF_SIZE, in_);
//        for (i = 0; i<n_bytes_read; i++) {
//            pcm_buffer_s[i] = (short)(pcm_buffer[i]/*-0x80*/)<<8;
//        }
        if (n_bytes_read == 0) {
            n_bytes_write = lame_encode_flush (lame_, mp3_buffer, MP3_SIZE);
        }
        else {
//            n_bytes_write = lame_encode_buffer (lame_, pcm_buffer_s, NULL,
//                                                n_bytes_read, mp3_buffer, MP3_SIZE);
            int bytes = n_bytes_read;
            if (header_.channels == 2) {
                bytes *= 0.5;
                n_bytes_write = lame_encode_buffer_interleaved (lame_, pcm_buffer_s,
                                                                bytes, mp3_buffer, MP3_SIZE);

            }
            else if (header_.channels == 1) {
                n_bytes_write = lame_encode_buffer (lame_, pcm_buffer_s, nullptr,
                                                bytes, mp3_buffer, MP3_SIZE);

            }
        }
        fwrite (mp3_buffer, sizeof (char), n_bytes_write, out_);
    } while (n_bytes_read>0);
}

std::string encoder::resolve_out_file_name (const std::string &file_name) {
    size_t pos = file_name.rfind ('.', file_name.length ());
    if (pos != string::npos) {
        return(file_name.substr ( 0, pos) + ".mp3");
    }

    return std::string ();
}


