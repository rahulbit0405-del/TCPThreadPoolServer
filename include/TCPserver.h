#ifndef TCPSERVER_H
#define TCPSERVER_H

#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include"ThreadPool.h"

class TcpServer{
    private:
    int serverSocket;
    bool running;


    public:
    TcpServer();
    void start();
    void stop();
    
    private:
    struct sockaddr_in serverAddr{};
    void setupSocket();
    void acceptClients();
    void handleClient(int clientSocket);
};

#endif
