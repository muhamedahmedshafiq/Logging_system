#include <iostream>
#include <string>
#include <vector>
#include <chrono>

// --- System Includes ---
#include "logger/LogManager.hpp"
#include "formatter/LogFormatter.hpp"
#include "formatter/policies/CpuPolicy.hpp"
#include "formatter/policies/RamPolicy.hpp"
#include "sink/LogSinkFactory.hpp"

namespace UI {
    void info(const std::string& msg) { std::cout << "\033[1;34m[INFO]\033[0m " << msg << "\n"; }
    void alert(const std::string& msg) { std::cout << "\033[1;33m[ALERT]\033[0m " << msg << "\n"; }
}

int main() {
    std::cout << "\033[1;32m" << "====================================================\n";
    std::cout << "      PHASE 4: ASYNCHRONOUS TELEMETRY DAEMON\n";
    std::cout << "====================================================\033[0m\n";

    // 1. Initialize LogManager (Internal thread starts here)
    // We give it a buffer size of 50 messages
    LogManager manager(50);
    
    // 2. Setup Sinks
    manager.addSink(LogSinkFactory::createSink(LogSinkType_enum::CONSOLE));
    // manager.addSink(LogSinkFactory::createSink(LogSinkType_enum::FILE)); // Optional

    // 3. Setup Formatter
    LogFormatter<CpuPolicy> cpuFormatter("Desktop-Linux", "CPU_LOAD");
    
    UI::info("System active. Enter CPU percentages (e.g., 45.5) or type 'exit' to quit.");
    UI::alert("Notice: Logging is now happening on a separate background thread!");

    std::string input;
    while (true) {
        std::cout << "Telemetry Input > ";
        if (!(std::cin >> input) || input == "exit") {
            break;
        }

        // 4. Producer Logic: Format and push to RingBuffer
        auto logMsg = cpuFormatter.formatDataToLogMsg(input);
        
        if (logMsg) {
            // manager.addLog now notifies the background thread immediately
            manager.addLog(std::move(*logMsg));
        } else {
            UI::alert("Invalid data input. Format logic rejected value: " + input);
        }
    }

    UI::info("Shutting down... flushing remaining logs in the RingBuffer.");
    
    // The Manager's destructor will automatically signal the thread to stop,
    // wait for the buffer to empty, and join the thread safely.
    
    return 0;
}