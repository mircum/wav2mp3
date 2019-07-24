//
// Created by Mircea Gita on 2019-07-22.
//

#include <string>
#include <vector>


#include "lame.h"
#include "dir_iterator.h"
#include "encoder.h"

// get a list with all *.wav files from specified folder
// only the files with extension *.wav will be selected
// verifying if a file is really wav file or not, can be done in a future version
int getWavFilesList (std::string &dirPath, std::vector<std::string> &wavFilesList) {
    return 0;
}

void convertToMp3 (std::string &dirPath, std::string &file) {


}

int main () {
    std::string dirPath ("/Users/mircea.gita/work/testWav/");
    dir_iterator dirIterator(dirPath);
    encoder wavEncoder;
    dirIterator.iterate (wavEncoder);

//
//
//    std::vector<std::string> wavFilesList;
//    getWavFilesList (dirPath, wavFilesList);
//
//    std::vector<std::string>::iterator it = wavFilesList.begin ();
//    //for_each (wavFilesList.begin(), wavFilesList.end(), convertToMp3);
//    for (auto c : wavFilesList) {
//        convertToMp3 (dirPath, c);
//    }



    return 0;
}