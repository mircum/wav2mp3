//
// Created by Mircea Gita on 2019-07-24.
//

//#include <functional>
//#include <cstdio>

#include <sstream>
#include <thread>
#include <encoder.h>


#include "logger.h"
#include "encoder.h"
#include "wave_header.h"


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
    th_id_ = ss.str()+"-> ";

    logger::log (th_id_+"Input file "+file_name);

    string file_path = dir_path+path_separator+file_name;
    // open input file
    in_ = fopen(file_path.c_str(), "rb");
    if (in_ == nullptr) {
        throw system_error (ENOENT, generic_category (), "Input FILE could not be opened");
    }

    // read wave header from input file
    std::unique_ptr<wave_header> wh1( new wave_header(in_));
    wh_ = std::move(wh1);
    if (!wh_->is_riff ()) {
        logger::error (th_id_+file_name+"\nERROR: Not a RIFF WAVE file format");
        return;
    }
    if (!wh_->is_pcm ()) {
        logger::error (th_id_+file_name+"\nERROR: Not a PCM audio format");
        return;
    }


    // initialize lame library
    lame_ = lame_init ();
    if (lame_ == nullptr) {
        throw runtime_error ("lame_init() failed");
    }

    lame_set_quality (lame_, 5);
    lame_set_in_samplerate (lame_, wh_->sample_rate ());
    unsigned int nc = wh_->channels ();
    lame_set_num_channels (lame_, nc);
    MPEG_mode mode = MONO;
    if (nc==2)
        mode = STEREO;
    lame_set_mode (lame_, mode);
//    lame_set_VBR (lame_, vbr_default);
    if (-1 == lame_init_params (lame_)) {
        throw runtime_error("lame_init_params() failed");

    }
//    lame_print_config (lame_);

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

#define PCM_BUF_SIZE 1024
#define MP3_SIZE 1024//8192

//TODO: all mp3 should be removed, otherwise there might be a situation when a thread is opening for read and the other for write

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
        if (n_bytes_read==0) {
            n_bytes_write = lame_encode_flush (lame_, mp3_buffer, MP3_SIZE);
        }
        else {
//            n_bytes_write = lame_encode_buffer (lame_, pcm_buffer_s, NULL,
//                                                n_bytes_read, mp3_buffer, MP3_SIZE);
            int bytes = n_bytes_read;
            if (wh_->channels () == 2) {
                bytes *= 0.5;
                n_bytes_write = lame_encode_buffer_interleaved (lame_, pcm_buffer_s,
                                                                bytes, mp3_buffer, MP3_SIZE);

            }
            else if (wh_->channels () == 1) {
                n_bytes_write = lame_encode_buffer (lame_, pcm_buffer_s, NULL,
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

