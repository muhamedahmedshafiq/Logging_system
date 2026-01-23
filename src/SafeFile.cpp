#include "SafeFile.hpp"
#include <fcntl.h>   
#include <unistd.h>  
#include <iostream>


constexpr int FAILED_TO_OPEN = -1;

SafeFile::SafeFile(SafeFile&& other) noexcept: filefd{other.filefd}{
    other.filefd = -1; //prevents the Double Close
}

SafeFile& SafeFile::operator=(SafeFile&& other)noexcept{
    if(this != &other){
        if(filefd !=-1){
            close(filefd);
        }
        filefd = other.filefd;
        other.filefd = -1;
    }
    return *this;
}


SafeFile::SafeFile(const std::string &RefFilePath){
    filefd = open(RefFilePath.c_str(),O_RDWR);

}

bool SafeFile::IsOpen(){
    return(filefd != FAILED_TO_OPEN);
}

std::string SafeFile::Read(){
    std::string returnString="";
    
    if(filefd != FAILED_TO_OPEN){
        char ch;
        // Read one character at a time until newline or EOF
        while (read(filefd, &ch, 1) == 1) {
            if (ch == '\n') {
                break;
            }
            returnString += ch;
        }
    }
    return(returnString);
}

SafeFile::~SafeFile(){
    if(filefd != FAILED_TO_OPEN){
        close(filefd);
    }
    
}