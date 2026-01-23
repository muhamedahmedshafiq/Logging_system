#pragma once
#include "string"

class SafeFile{
    private:
        int filefd;
    public:
    SafeFile()=delete;
    explicit SafeFile(const std::string &RefFilePath);

    // Prevent copying the socket
    SafeFile(const SafeFile& other)=delete;
    SafeFile(SafeFile&& other)noexcept;
    
    // Prevent copying the socket
    SafeFile &operator=(const SafeFile& other)=delete;
    SafeFile &operator=(SafeFile && other)noexcept;

    bool IsOpen();

    std::string Read();

    ~SafeFile ();
};