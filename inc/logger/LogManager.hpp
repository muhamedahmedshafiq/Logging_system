#pragma once
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "RingBuffer.hpp"
#include "LogMessage.hpp"
#include "sink/ILogSink.hpp"

class LogManager {
private:
    RingBuffer<LogMessage> queue;
    std::vector<std::unique_ptr<ILogSink>> sinks;
    
    // Threading components
    std::thread workerThread;
    std::mutex cvMtx;              // Mutex specifically for the condition variable
    std::condition_variable cv;    // For notifying the worker thread
    std::atomic<bool> stopFlag;    // Thread-safe shutdown signal

    // The function executed by the background thread
    void processLoop();

public:
    explicit LogManager(size_t capacity = 100);
    ~LogManager();

    // Prevent copies to avoid thread ownership issues
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

    // Interface for the Producer (Telemetry Source)
    void addLog(LogMessage&& msg);
    
    // Interface for Configuration
    void addSink(std::unique_ptr<ILogSink> sink);
};