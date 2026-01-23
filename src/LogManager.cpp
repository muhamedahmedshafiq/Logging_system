#include "LogManager.hpp"
#include "ILogSink.hpp"

void LogManager::addLog(const LogMessage& log){
    logs.push_back(log);
}


//transfers ownership of sink to manager
void LogManager::addSink(std::unique_ptr<ILogSink> sink){
    sinks.push_back(std::move(sink));
}

//sends each log to all sinks
void LogManager::routeLogsForAllSinks(){
    for(auto& log : logs){
        for(auto& sink: sinks){
            sink->write(log);
        }
    }
}