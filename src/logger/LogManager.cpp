#include "logger/LogManager.hpp"
#include "sink/ILogSink.hpp"


LogManager::LogManager(size_t capacity) 
    : queue(capacity), stopFlag(false) {
    // Start the worker thread immediately upon construction
    workerThread = std::thread(&LogManager::processLoop, this);
}


void LogManager::addLog(LogMessage&& msg) {
    // Try to push to the RingBuffer
    if (queue.tryPush(std::move(msg))) {
        // Only notify the thread if we successfully added a message
        cv.notify_one(); 
    }
}

void LogManager::addSink(std::unique_ptr<ILogSink> sink) {
    sinks.push_back(std::move(sink));
}

void LogManager::processLoop() {
    while (true) {
        std::unique_lock<std::mutex> lock(cvMtx);
        
        // Block here until data is available OR we are shutting down
        cv.wait(lock, [this] { 
            return stopFlag.load() || !queue.isEmpty(); 
        });

        // Shutdown condition: flag is set AND no more logs are left to process
        if (stopFlag.load() && queue.isEmpty()) {
            break; 
        }

        lock.unlock(); // Release lock while writing to sinks (expensive I/O)

        // Consume all available messages in the buffer
        while (auto msg = queue.tryPop()) {
            for (auto& sink : sinks) {
                if (sink) {
                    sink->write(*msg);
                }
            }
        }
    }
}

LogManager::~LogManager() {
    // 1. Signal the thread to stop
    stopFlag = true;
    
    // 2. Wake it up one last time in case it is sleeping on cv.wait()
    cv.notify_all();
    
    // 3. Wait for it to finish flushing the remaining logs
    if (workerThread.joinable()) {
        workerThread.join();
    }
}
