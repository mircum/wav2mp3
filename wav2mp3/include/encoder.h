//
// Created by Mircea Gita on 2019-07-24.
//

#ifndef WAV2MP3_ENCODER_H
#define WAV2MP3_ENCODER_H

#include <string>

class thread_pool;
class encoder final {
public:
    encoder(thread_pool &tPool);
    int encode (const std::string &filePath);

protected:
    //void createInstances
    int doEncode(const std::string &filePath);
    std::string getFileExt(const std::string& filePath);
    bool validate (const std::string &filePath);


private:
    thread_pool &tPool_;
};


#endif //WAV2MP3_ENCODER_H
