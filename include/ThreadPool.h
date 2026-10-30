#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<iostream>
#include<mutex>
#include<thread>
#include<vector>
#include<condition_variable>
#include<chrono>
#include<algorithm>
#include<functional>
#include<queue>
using namespace std;
using namespace std::chrono;

class Threadpool{
    private:
    std::condition_variable cv;
    std::mutex queueMutex;
    void worker();
    bool stop = false;
    std::vector<std::thread>workers;
    std::queue<std::function<void()>>tasks;

    public:
    Threadpool(size_t numThreads);
    ~Threadpool();

};
#endif
