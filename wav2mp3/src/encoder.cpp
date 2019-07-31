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
    //lame_set_mode
    lame_set_VBR (lame_, vbr_default);
    lame_init_params (lame_);
}

encoder::~encoder () {
    if (lame_)
        lame_close (lame_);
    if (out_)
        fclose (out_);
    if (in_)
       fclose (in_);

}

int encoder::encode (const string &filePath) {
    if (!validate (filePath))
        return 0;

    doEncode (filePath);

    return 0;
}

int encoder::doEncode (const std::string &filePath) {

    printf ("%s \n", filePath.c_str ());
    int read, write;

    std::string input (filePath);
    std::string output (filePath + ".mp3");

    FILE *pcm = fopen(input.c_str(), "rb");
    FILE *mp3 = fopen(output.c_str(), "wb");

    const int PCM_SIZE = 8192;
    const int MP3_SIZE = 8192;

    short int pcm_buffer[PCM_SIZE*2];
    unsigned char mp3_buffer[MP3_SIZE];

    lame_t lame = lame_init();
    lame_set_quality(lame, 5);
    lame_set_in_samplerate(lame, 44100);
    lame_set_VBR(lame, vbr_default);
    lame_init_params(lame);

    do {
        read = fread(pcm_buffer, 2*sizeof(short int), PCM_SIZE, pcm);
        if (read == 0)
            write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
        else
            write = lame_encode_buffer_interleaved(lame, pcm_buffer, read, mp3_buffer, MP3_SIZE);
        fwrite(mp3_buffer, write, 1, mp3);
    } while (read != 0);

    fclose(mp3);
    fclose(pcm);
    return 0;
}

string encoder::getFileExt(const string& filePath) {

    size_t pos = filePath.rfind('.', filePath.length());
    if (pos != string::npos) {
        return(filePath.substr (pos + 1, filePath.length() - pos));
    }

    return("");
}

// only the files with extension *.wav will be selected
// verifying the file header to check if a file is really wav or not, can be done in a future version
bool encoder::validate (const std::string &filePath) {
    string ext = getFileExt (filePath);
    if (ext == "wav")
        return true;
    return false;
}

std::string encoder::get_out_file_name (const std::string &file_name) {
    size_t pos = file_name.rfind ('.', file_name.length ());
    if (pos != string::npos) {
        return(file_name.substr ( 0, pos) + ".mp3");
    }

    return std::string ();
}

