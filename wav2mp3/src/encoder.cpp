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
in_ (nullptr),
out_ (nullptr),
lame_ (nullptr) {
    // save thread id in a string to identify messages
    stringstream ss;
    ss << this_thread::get_id();
    th_id_ = "\n"+ss.str()+"-> ";

    logger::log (th_id_+"Input file "+file_name);

    string file_path = dir_path+path_separator+file_name;

    // open input file and check if it is a wave file
    in_ = fopen(file_path.c_str(), "rb");
    if (in_ == nullptr) {
        throw system_error(ENOENT, generic_category (), "Input FILE could not be opened");
    }

    wave_header wh(in_);
    if (!wh.is_wave ()) {
        logger::error (th_id_+file_name+"\nERROR: Not a WAVE file format");
        return;
    }
    if (!wh.is_pcm ()) {
        logger::error (th_id_+file_name+"\nERROR: Not a PCM audio format");
        return;
    }

    string out = get_out_file_name (file_name);
    logger::log (th_id_+"Output file "+out);
    file_path = dir_path+path_separator+out;
    out_ = fopen (file_path.c_str(), "wb");

    // create and init lame;
    lame_ = lame_init ();
    if (lame_ == nullptr) {

    }
    lame_set_quality (lame_, 5);
    lame_set_in_samplerate (lame_, wh.sample_rate ());
    unsigned int nc = wh.channels ();
    lame_set_num_channels (lame_, nc);
    MPEG_mode mode = MONO;
    if (nc==2)
        mode = STEREO;
    lame_set_mode (lame_, mode);
//    lame_set_VBR (lame_, vbr_default);
    lame_init_params (lame_);//TODO: check return
//    lame_print_config (lame_);
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
#define MP3_SIZE 8480//8192

//TODO: all mp3 should be removed, otherwise there might be a situation when a thread is opening for read and the other for write

int encoder::encode () {

    int n_bytes_read;
    int n_bytes_write;
    int i;

    short pcm_buffer_s[PCM_BUF_SIZE];
    unsigned char pcm_buffer[PCM_BUF_SIZE];
    unsigned char mp3_buffer[MP3_SIZE];

    fseek (in_, 72, SEEK_SET);

    do {
        n_bytes_read = fread (pcm_buffer, sizeof (char), PCM_BUF_SIZE, in_);
        for (i = 0; i<n_bytes_read; i++) {
            pcm_buffer_s[i] = (short)(pcm_buffer[i]-0x80)<<8;
        }
        if (n_bytes_read==0) {
            n_bytes_write = lame_encode_flush (lame_, mp3_buffer, MP3_SIZE);
        }
        else {
//            n_bytes_write = lame_encode_buffer (lame_, pcm_buffer_s, NULL,
//                                                n_bytes_read, mp3_buffer, MP3_SIZE);
            n_bytes_write = lame_encode_buffer_interleaved (lame_, pcm_buffer_s,
                                                n_bytes_read, mp3_buffer, MP3_SIZE);
        }
        fwrite (mp3_buffer, sizeof (char), n_bytes_write, out_);
    } while (n_bytes_read>0);

    return 0;
}

std::string encoder::get_out_file_name (const std::string &file_name) {
    size_t pos = file_name.rfind ('.', file_name.length ());
    if (pos != string::npos) {
        return(file_name.substr ( 0, pos) + ".mp3");
    }

    return std::string ();
}

