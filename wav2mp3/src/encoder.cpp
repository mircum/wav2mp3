//
// Created by Mircea Gita on 2019-07-24.
//

//#include <functional>
//#include <cstdio>

#include <fstream>

#include "encoder.h"
#include "wave_file.h"

using namespace std;

encoder::encoder (const std::string &file_path) :
wave_file_ (wave_file(file_path)), 
file_path_ (file_path) {

    lame_ = lame_init ();
    lame_set_quality (lame_, 5);
    lame_set_in_samplerate (lame_, wave_file_.sample_rate ());
    unsigned int nc = wave_file_.channels ();
    lame_set_num_channels (lame_, nc);
    //lame_set_mode
    lame_set_VBR (lame_, vbr_default);
    lame_init_params (lame_);

    in_.open (file_path.c_str());
//    in.

}

encoder::~encoder () {
    lame_close (lame_);

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

