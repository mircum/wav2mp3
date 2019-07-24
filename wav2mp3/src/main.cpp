//
// Created by Mircea Gita on 2019-07-22.
//

#include <string>
#include <vector>

#include <dirent.h>

#include "lame.h"

// get a list with all *.wav files from specified folder
// only the files with extension *.wav will be selected
// verifying if a file is really wav file or not, can be done in a future version
int getWavFilesList (std::string &dirPath, std::vector<std::string> &wavFilesList) {
    DIR *dir = opendir (dirPath.c_str ());
    struct dirent *entry;
    if ( dir != nullptr) {

        while ((entry = readdir (dir)) != NULL) {
            // skip it if folder
            if (entry->d_type == DT_DIR)
                continue;
            // skip files which are not wave
            if (strstr (entry->d_name, ".wav")) {
                wavFilesList.insert (wavFilesList.end (), std::string (entry->d_name));
                printf ("%s added to list\n", entry->d_name);
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return 1;
    }
    return 0;
}

void convertToMp3 (std::string &dirPath, std::string &file) {

    int read, write;

    std::string input(dirPath + file );
    std::string output(dirPath + file + ".mp3");

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

}

int main () {
    std::string dirPath ("/Users/mircea.gita/work/testWav/");
    std::vector<std::string> wavFilesList;
    getWavFilesList (dirPath, wavFilesList);

    std::vector<std::string>::iterator it = wavFilesList.begin ();
    //for_each (wavFilesList.begin(), wavFilesList.end(), convertToMp3);
    for (auto c : wavFilesList) {
        convertToMp3 (dirPath, c);
    }



    return 0;
}