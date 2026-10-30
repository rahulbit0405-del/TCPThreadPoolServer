#include"ThreadPool.h"
#include"TCPserver.h"


void Threadpool::worker(){
    while(true)
    {
        std::function<void()>task;
        {
            std::unique_lock<std::mutex>lock(queueMutex);
            cv.wait(lock,[this]{
                return !tasks.empty() || stop;
            });
            if (tasks.empty() && stop){
                break;
            }
            task = tasks.front();
            tasks.pop();
        }
        task();
    }

}
Threadpool::Threadpool(size_t numThreads){

    {
        tasks.push([](){
            std::cout<<"Task 1 running ..."<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout<<"Task 1 finished ..."<<std::endl;
        });
        tasks.push([](){
            std::cout<<"Task 2 running ..."<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout<<"Task 2 finished ..."<<std::endl;
        });
        tasks.push([](){
            std::cout<<"Task 3 running ..."<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout<<"Task 3 finished ..."<<std::endl;
        });
        tasks.push([](){
            std::cout<<"Task 4 running ..."<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout<<"Task 4 finished ..."<<std::endl;
        });
    }
    for(size_t i = 0; i< numThreads ;++i){
        workers.emplace_back([this]()
        {
            worker();
    });
  }
}
Threadpool::~Threadpool(){
    {
        std::unique_lock<std::mutex>lock(queueMutex);
        stop = true;
    }
    cv.notify_all();
    for(auto &worker:workers){
    if (worker.joinable()){
        worker.join();

    }
 }
}
