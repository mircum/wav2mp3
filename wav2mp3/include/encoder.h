//
// Created by Mircea Gita on 2019-07-24.
//

#ifndef WAV2MP3_ENCODER_H
#define WAV2MP3_ENCODER_H

#include <string>


class encoder final {
public:
    encoder();
    int encode (const std::string &filePath);

protected:
    std::string getFileExt(const std::string& filePath);
    bool validate (const std::string &filePath);


private:
//    std::string filePath;
};


#endif //WAV2MP3_ENCODER_H
