//
// Created by Mircea Gita on 2019-07-24.
//

//#include <functional>
//#include <cstdio>

#include <sstream>
#include <thread>
#include <cstring>

#include "logger.h"
#include "encoder.h"


const char path_separator =
#ifdef _WIN32
        '\\';
#else
        '/';
#endif

#define PCM_BUF_SIZE 8192
#define MP3_BUF_SIZE 8192


using namespace std;

encoder::encoder (const std::string &dir_path, const std::string &file_name) :
can_encode_ (false),
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
    if (nullptr == in_) {
        throw system_error (ENOENT, generic_category (), "Input FILE could not be opened");
    }

    // read wave header from input file
    read_wave_header ();
    if (!is_riff ()) {
        logger::log (th_id_ + file_name + " skipped: Not a RIFF WAVE file format");
        return;
    }
    if (!is_pcm ()) {
        logger::log (th_id_ + file_name + " skipped: Not a PCM audio format");
        return;
    }
    if (2 < header_.channels) {
        logger::log (th_id_ + file_name + " skipped: Max 2 channels supported");
        return;
    }

    // initialize lame library
    init_lame ();

    // create output file with the same name + mp3 extension
    string out = resolve_out_file_name (file_name);
    logger::log (th_id_ + "Output file " + out);
    file_path = dir_path + path_separator + out;
    out_ = fopen (file_path.c_str(), "wb");
    if (in_ == nullptr) {
        throw runtime_error ("Output FILE could not be created");
    }

    can_encode_ = true;
}

encoder::~encoder () {
    if (lame_)
        lame_close (lame_);
    if (out_)
        fclose (out_);
    if (in_)
       fclose (in_);
}

void encoder::read_wave_header () {

    unsigned char buffer4[4];
    unsigned char buffer2[2];

    // read header_ parts
    fread (header_.riff, sizeof (header_.riff), 1, in_);
    fread (buffer4, sizeof (buffer4), 1, in_);

    // convert little endian to big endian 4 byte int
    header_.overall_size =   buffer4[0]|
                             (buffer4[1]<<8u)|
                             (buffer4[2]<<16u)|
                             (buffer4[3]<<24u);

    fread (header_.wave, sizeof (header_.wave), 1, in_);

    fread (header_.fmt_chunk_marker, sizeof (header_.fmt_chunk_marker), 1, in_);

    fread (buffer4, sizeof (buffer4), 1, in_);
    // convert little endian to big endian 4 byte integer
    header_.length_of_fmt =  buffer4[0]|
                             (buffer4[1]<<8u)|
                             (buffer4[2]<<16u)|
                             (buffer4[3]<<24u);

    fread (buffer2, sizeof (buffer2), 1, in_);
    header_.format_type = buffer2[0]|(buffer2[1]<<8u);

    fread (buffer2, sizeof (buffer2), 1, in_);
    header_.channels = buffer2[0]|(buffer2[1]<<8u);

    fread (buffer4, sizeof (buffer4), 1, in_);
    header_.sample_rate = buffer4[0]|
                          (buffer4[1]<<8u)|
                          (buffer4[2]<<16u)|
                          (buffer4[3]<<24u);

    fread (buffer4, sizeof (buffer4), 1, in_);
    header_.byterate =   buffer4[0]|
                         (buffer4[1]<<8u)|
                         (buffer4[2]<<16u)|
                         (buffer4[3]<<24u);

    fread (buffer2, sizeof (buffer2), 1, in_);
    header_.block_align = buffer2[0]|(buffer2[1]<<8u);

    fread (buffer2, sizeof (buffer2), 1, in_);
    header_.bits_per_sample = buffer2[0]|(buffer2[1]<<8u);

    fread (header_.data_chunk_header, sizeof (header_.data_chunk_header), 1, in_);

    fread (buffer4, sizeof (buffer4), 1, in_);
    header_.data_size =  buffer4[0]|
                         (buffer4[1]<<8u)|
                         (buffer4[2]<<16u)|
                         (buffer4[3]<<24u);
}

bool encoder::is_riff () {
    // the canonical WAVE format starts with the RIFF header
    return strstr ((char*)header_.riff, "RIFF") != nullptr;
}

bool encoder::is_pcm () {
    // this version only handles PCM format
    return 1 == header_.format_type;
}

void encoder::init_lame () {
    lame_ = lame_init ();
    if (nullptr == lame_) {
        throw runtime_error ("lame_init() failed");
    }

    lame_set_quality (lame_, 5);
    lame_set_in_samplerate (lame_, header_.sample_rate);
    lame_set_num_channels (lame_, header_.channels);
    MPEG_mode mode = MONO;
    if (2 == header_.channels)
        mode = STEREO;
    lame_set_mode (lame_, mode);
    if (-1 == lame_init_params (lame_)) {
        throw runtime_error("lame_init_params() failed");
    }
}

// with current implementation, a race might happen on mp3 files
// a thread might try to read it while another one might want to write into it
// TODO: to fix the possible race condition
void encoder::do_encode () {
    if (!can_encode_)
        return;

    int n_bytes_read;
    int n_bytes_write = 0;

    short pcm_buffer_s[PCM_BUF_SIZE];
    unsigned char mp3_buffer[MP3_BUF_SIZE];

    do {
        n_bytes_read = fread (pcm_buffer_s, sizeof (short), PCM_BUF_SIZE, in_);
        if (0 == n_bytes_read) {
            n_bytes_write = lame_encode_flush (lame_, mp3_buffer, MP3_BUF_SIZE);
        }
        else {
            if (2 == header_.channels) {
                n_bytes_write = lame_encode_buffer_interleaved (lame_, pcm_buffer_s,
                                                                n_bytes_read * 0.5, mp3_buffer, MP3_BUF_SIZE);
            }
            else if (1 == header_.channels) {
                n_bytes_write = lame_encode_buffer (lame_, pcm_buffer_s, nullptr,
                                                n_bytes_read, mp3_buffer, MP3_BUF_SIZE);
            }
        }
        if (0 > n_bytes_write) {
            logger::error (th_id_ + " ERROR: encoding failed. Skipping the rest of the file");
            break;
        }
        fwrite (mp3_buffer, sizeof (char), n_bytes_write, out_);
    } while (n_bytes_read > 0);
}

std::string encoder::resolve_out_file_name (const std::string &file_name) {
    size_t pos = file_name.rfind ('.', file_name.length ());
    if (pos != string::npos) {
        return(file_name.substr ( 0, pos) + ".mp3");
    }

    return std::string ();
}


