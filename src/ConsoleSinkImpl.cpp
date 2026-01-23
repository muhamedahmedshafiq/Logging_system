#include "ConsoleSinkImpl.hpp"
#include<iostream>

void ConsoleSink::write(const LogMessage& log) {
    std::cout << log << std::endl; 
}