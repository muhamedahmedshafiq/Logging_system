#include "raii/SafeSocket.hpp"
#include <iostream>
#include <sys/types.h>          
#include <sys/socket.h>
#include <cstring>
#include <sys/un.h>
#include <unistd.h>

constexpr int FAILED_TO_OPEN = -1;
constexpr int FAILED_TO_CONNECT = -1;


SafeSocket::SafeSocket(SafeSocket&& other) noexcept : socketfd{other.socketfd} { 
    other.socketfd = -1;        
}


SafeSocket& SafeSocket::operator=(SafeSocket&& other) noexcept {
    if (this != &other) {
        if (socketfd != -1){
            close(socketfd); // Close our own current resource first
        }
        socketfd = other.socketfd;           // Steal the new one
        other.socketfd = -1;                 // Nullify the old one
    }
    return *this;
}


SafeSocket::SafeSocket(const std::string &RefFilePath){
    socketfd = socket(AF_UNIX,SOCK_STREAM,0);
    if (socketfd != FAILED_TO_OPEN) {
        
        // Setup address structure
        struct sockaddr_un addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        std::strncpy(addr.sun_path, RefFilePath.c_str(), sizeof(addr.sun_path) - 1);
        
        int result = connect(socketfd,reinterpret_cast<struct sockaddr*>(&addr),sizeof(addr));
        if(result == FAILED_TO_CONNECT){
            close(socketfd);
            socketfd = FAILED_TO_OPEN;
        }
    }
    else{
        socketfd = FAILED_TO_OPEN;
    }
}


bool SafeSocket::IsOpen(){
    return(socketfd != FAILED_TO_OPEN);
}

std::string SafeSocket::Read(){
    std::string returnString="";

    if(socketfd != FAILED_TO_OPEN){
        char ch;
        while (read(socketfd,&ch,1)==1){
            if(ch == '\n'){
                break;
            }
            returnString += ch;
        }
    }
    return(returnString);
}

SafeSocket::~SafeSocket(){
    if(socketfd != FAILED_TO_OPEN){
        close(socketfd);
    }
}
