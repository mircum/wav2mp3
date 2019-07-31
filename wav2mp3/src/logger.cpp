//
// Created by Mircea Gita on 2019-07-31.
//
#include <iostream>
#include "logger.h"

using namespace std;

logger &logger::get_instance () {
    static logger instance;
    return instance;
}

void logger::log (const std::string &msg) {
    logger::get_instance().log_message (msg, false);
}

void logger::error (const std::string &err) {
    logger::get_instance ().log_message (err, true);
}

void logger::log_message (const std::string &msg, bool is_error) {
    std::lock_guard<std::mutex> lock (mutex_);
    if (is_error)
        cerr << msg << endl;
    else
        cout << msg << endl;
}
