#include"TCPserver.h"
#include"ThreadPool.h"
#include<iostream>
#include<chrono>


int main(int argc, char* argv[]){
    
    Threadpool pool(4);
    TcpServer server;
    server.start();

    return 0;
    
}
