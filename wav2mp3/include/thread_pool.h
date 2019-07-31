//
// Created by Mircea Gita on 2019-07-24.
//
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <functional>

class thread_pool final {
public:
    explicit thread_pool (unsigned int nt);
    ~thread_pool ();
    void add_job (std::function<void (void)> func);

    thread_pool () = delete;
    thread_pool (const thread_pool &) = delete;
    thread_pool (thread_pool &&) = delete;
    thread_pool & operator= (const thread_pool &) = delete;
    thread_pool & operator= (thread_pool &&) = delete;

private:
    void thread_entry (int i);

private:
    std::mutex mutex_;
    std::condition_variable cond_var_;
    bool shutdown_;
    std::vector <std::thread> threads_;
    std::queue <std::function <void (void)>> jobs_;
};
