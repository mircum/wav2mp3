//
// Created by Mircea Gita on 2019-07-31.
//

#ifndef WAV2MP3_LOGGER_H
#define WAV2MP3_LOGGER_H

#include <mutex>
#include <string>

class logger {
public:
    logger (const logger &) = delete;
    logger (logger &&) = delete;
    logger &operator= (const logger &) = delete;
    logger &operator= (logger &&) = delete;

    static logger &get_instance();
    static void log (const std::string &msg);
    static void error (const std::string &err);

private:
    logger () = default;
    void log_message (const std::string &msg, bool is_error);

private:
    std::mutex mutex_;
};


#endif //WAV2MP3_LOGGER_H
