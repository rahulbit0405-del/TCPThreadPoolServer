#include "TCPserver.h"
#include<cstring>

TcpServer::TcpServer():serverSocket(-1),running(false){}

void TcpServer::start(){
    std::cout<<"Starting a socket program ..."<<std::endl;

    std::cout<<"The socket is serverSocket "<<std::endl;

    serverSocket =socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if (serverSocket < 0){
        std::cout<<"Socket not found ..."<<std::endl;
    }
    else{
        std::cout<<"Socket found ..."<< std::endl;
    }
    setupSocket();
    
    running = true;

    acceptClients();
}
void TcpServer::setupSocket(){

    int opt = 1;
    setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    setsockopt(serverSocket,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt));


    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(
        serverSocket,
        (struct sockaddr*)&serverAddr,
        sizeof(serverAddr)
    );

    listen(serverSocket,10);

}
void TcpServer::acceptClients()
{
    std::cout << "Waiting for clients ..." << std::endl;

    while (running)
    {
        int client_fd = accept(
            serverSocket,
            nullptr,
            nullptr
        );

        if (client_fd < 0)
        {
            std::cout << "Accept failed ..\n";
            continue;
        }

        std::cout << "Client Connected : "
                  << client_fd << std::endl;

        handleClient(client_fd);
    }
}
void TcpServer::handleClient(int clientSocket){
    char buffer[1024];
    int bytes = recv(clientSocket , buffer , sizeof(buffer) - 1,0);
    
    if(bytes < 0){
        std::cerr<< "Receive failed \n";
        close(clientSocket);
        return;
    }
    buffer[bytes] ='\0';

    std::cout<<"Client sent ... :" << buffer <<std::endl;

    const char* msg = "HELLO WORLD";
    write(clientSocket,msg,strlen(msg));
    close(clientSocket);

}
void TcpServer::stop(){
    std::cout<<"To close the serverSocket ..."<<std::endl;
    close(serverSocket);
}


