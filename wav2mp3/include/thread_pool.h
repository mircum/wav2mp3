//
// Created by Mircea Gita on 2019-07-24.
//
#include <thread>
#include <mutex>
#include <queue>
#include <vector>



class thread_pool {
public:
    thread_pool (unsigned int nbThreads);
    ~thread_pool ();
    void addJob (std::function <void (void)> func);

private:
    void threadEntry (int i);

private:
    std::mutex lock_;
    std::condition_variable condVar_;
    bool shutdown_;
    std::vector <std::thread> threads_;
    std::queue <std::function <void (void)>> jobs_;
};
