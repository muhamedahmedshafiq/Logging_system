#pragma once

#include <string>

class SafeSocket{
    private:
        int socketfd;
    public:
    SafeSocket()=delete;
    explicit SafeSocket(const std::string &RefFilePath);

    // Prevent copying the socket
    SafeSocket(const SafeSocket& other )=delete;
    SafeSocket(SafeSocket&& other)noexcept;
    
    // Prevent copying the socket
    SafeSocket &operator=(const SafeSocket&)=delete;
    SafeSocket &operator=(SafeSocket&& other)noexcept;
    
    bool IsOpen();
    std::string Read();

    ~SafeSocket();

};