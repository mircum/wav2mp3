//
// Created by Mircea Gita on 2019-07-24.
//

#include <functional>
#include <cstdio>

#include "encoder.h"
#include "lame.h"
#include "thread_pool.h"

using namespace std;

encoder::encoder (thread_pool &tPool) : tPool_ (tPool) {

}


int encoder::encode (const string &filePath) {
    if (!validate (filePath))
        return 0;

    tPool_.add_job (bind (&encoder::doEncode, this, filePath));

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

    lame_close(lame);
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


