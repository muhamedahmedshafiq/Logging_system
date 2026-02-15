# Telemetry Logger System

A modern, extensible C++ logging framework designed for real-time telemetry monitoring with policy-based severity classification and multiple output destinations.

## 🎯 Overview

This system provides a robust architecture for collecting, processing, and routing telemetry data (CPU, GPU, RAM metrics) with automatic severity classification based on configurable thresholds. It features a clean separation of concerns through policy-based design, RAII resource management, and the Factory pattern.

## ✨ Key Features

- **Policy-Based Design**: Extensible telemetry policies for CPU, GPU, and RAM monitoring
- **Multiple Output Sinks**: Console, file, and socket outputs with factory creation
- **Automatic Severity Classification**: Dynamic severity levels (INFO, WARNING, CRITICAL) based on thresholds
- **RAII Resource Management**: Safe file and socket handling with automatic cleanup
- **Template-Based Formatting**: Type-safe log message formatting with policy-driven behavior
- **Thread-Safe Architecture**: Designed for concurrent telemetry collection

## 📁 Project Structure

```
.
├── enums/
│   ├── LogSinkType.hpp         # Output destination types
│   ├── SeverityLevel.hpp       # Log severity levels
│   └── TelemetrySource.hpp     # Telemetry data types
│
├── formatter/
│   ├── LogFormatter.hpp        # Template-based log formatter
│   ├── LogFormatter.tpp        # Template implementation
│   └── policies/
│       ├── CpuPolicy.hpp       # CPU threshold policy (80%/95%)
│       ├── GpuPolicy.hpp       # GPU threshold policy (70%/85%)
│       └── RamPolicy.hpp       # RAM threshold policy (75%/95%)
│
├── logger/
│   ├── LogManager.hpp/cpp      # Central log routing manager
│   └── LogMessage.hpp/cpp      # Timestamped log message
│
├── sink/
│   ├── ILogSink.hpp            # Sink interface
│   ├── ConsoleSinkImpl.hpp/cpp # Console output
│   ├── FileSinkImpl.hpp/cpp    # File output
│   └── LogSinkFactory.hpp/cpp  # Sink factory
│
├── sources/
│   ├── ITelemetrySource.hpp    # Source interface
│   ├── FileTelemetrySourceImpl.hpp/cpp
│   └── SocketTelemetrySourceImpl.hpp/cpp
│
├── raii/
│   ├── SafeFile.cpp            # RAII file wrapper
│   └── SafeSocket.hpp/cpp      # RAII socket wrapper
│
└── main.cpp                    # Integration tests
```

## 🏗️ Architecture

### Core Components

#### 1. **Telemetry Policies**
Each policy defines resource-specific thresholds and severity inference:

```cpp
struct CpuPolicy {
    static constexpr float WARN_THRESHOLD = 80.0f;
    static constexpr float CRIT_THRESHOLD = 95.0f;
    static constexpr std::string_view unit = "%";
    
    static SeverityLvl_enum inferSeverity(float value);
};
```

#### 2. **Log Formatter (Template)**
Converts raw telemetry data to structured log messages using policy-driven severity:

```cpp
template <typename Policy>
class LogFormatter {
    std::optional<LogMessage> formatDataToLogMsg(const std::string& rawData);
};
```

#### 3. **Log Manager**
Routes logs to multiple sinks with ownership management:

```cpp
class LogManager {
    void addLog(const LogMessage& log);
    void addSink(std::unique_ptr<ILogSink> sink);
    void routeLogsForAllSinks();  // Broadcasts to all sinks
};
```

#### 4. **Sink System**
Factory-created output destinations with polymorphic interface:

```cpp
auto sink = LogSinkFactory::createSink(LogSinkType_enum::CONSOLE);
manager.addSink(std::move(sink));
```

## 🚀 Usage Examples

### Basic Usage

```cpp
#include "logger/LogManager.hpp"
#include "formatter/LogFormatter.hpp"
#include "formatter/policies/CpuPolicy.hpp"
#include "sink/LogSinkFactory.hpp"

int main() {
    // 1. Create log manager
    LogManager manager;
    
    // 2. Add output sinks
    manager.addSink(LogSinkFactory::createSink(LogSinkType_enum::CONSOLE));
    manager.addSink(LogSinkFactory::createSink(LogSinkType_enum::FILE, "telemetry.log"));
    
    // 3. Create formatter with policy
    LogFormatter<CpuPolicy> cpuFormatter("MyApp", "CPU_CORE_0");
    
    // 4. Process telemetry data
    std::vector<std::string> readings = {"45.0", "82.5", "96.3"};
    
    for (const auto& reading : readings) {
        auto logMsg = cpuFormatter.formatDataToLogMsg(reading);
        if (logMsg.has_value()) {
            manager.addLog(std::move(*logMsg));
        }
    }
    
    // 5. Flush logs to all sinks
    manager.routeLogsForAllSinks();
    
    return 0;
}
```

### Output Example

```
2026-02-12 14:32:10 [INFO   ] [MyApp::CPU_CORE_0] CPU_CORE_0 usage: 45.000000%
2026-02-12 14:32:10 [WARNING] [MyApp::CPU_CORE_0] CPU_CORE_0 usage: 82.500000%
2026-02-12 14:32:10 [ERROR  ] [MyApp::CPU_CORE_0] CPU_CORE_0 usage: 96.300000%
```

### Multi-Policy Monitoring

```cpp
LogFormatter<CpuPolicy> cpuFormatter("Server-01", "CPU");
LogFormatter<GpuPolicy> gpuFormatter("Server-01", "NVIDIA_RTX_4090");
LogFormatter<RamPolicy> ramFormatter("Server-01", "MEMORY");

// CPU reading: 85% → WARNING
auto cpuLog = cpuFormatter.formatDataToLogMsg("85.0");

// GPU reading: 72% → WARNING (GPU has lower threshold)
auto gpuLog = gpuFormatter.formatDataToLogMsg("72.0");

// RAM reading: 68% → INFO
auto ramLog = ramFormatter.formatDataToLogMsg("68.0");

manager.addLog(*cpuLog);
manager.addLog(*gpuLog);
manager.addLog(*ramLog);
manager.routeLogsForAllSinks();
```

### Telemetry Source Integration

```cpp
#include "sources/FileTelemetrySourceImpl.hpp"

// Read from file
FileTelemetrySourceImpl fileSource("/tmp/cpu_metrics.txt");
if (fileSource.openSource()) {
    std::string data;
    while (fileSource.readSource(data)) {
        auto logMsg = cpuFormatter.formatDataToLogMsg(data);
        if (logMsg) manager.addLog(*logMsg);
    }
}

// Read from socket
SocketTelemetrySourceImpl socketSource("/tmp/telemetry.sock");
if (socketSource.openSource()) {
    std::string data;
    while (socketSource.readSource(data)) {
        auto logMsg = gpuFormatter.formatDataToLogMsg(data);
        if (logMsg) manager.addLog(*logMsg);
    }
}
```

## 🔧 Configuration

### Severity Thresholds

| Policy | WARNING | CRITICAL | Unit |
|--------|---------|----------|------|
| **CPU** | 80.0% | 95.0% | % |
| **GPU** | 70.0% | 85.0% | % |
| **RAM** | 75.0% | 95.0% | % |

### Sink Types

```cpp
enum class LogSinkType_enum {
    CONSOLE,    // stdout output
    FILE,       // Persistent file (default: system.log)
    SOCKET      // Network socket (not yet implemented)
};
```

### Severity Levels

```cpp
enum class SeverityLvl_enum {
    CRITICAL,   // Immediate action required
    WARNING,    // Attention needed
    INFO        // Normal operation
};
```

## 🛠️ Building the Project

### Prerequisites
- C++17 or later
- POSIX-compliant system (for file/socket operations)
- Standard library support for `<chrono>`, `<optional>`, `<memory>`

### Compilation

```bash
# Compile all source files
g++ -std=c++17 -I. \
    logger/LogManager.cpp \
    logger/LogMessage.cpp \
    sink/ConsoleSinkImpl.cpp \
    sink/FileSinkImpl.cpp \
    sink/LogSinkFactory.cpp \
    sources/FileTelemetrySourceImpl.cpp \
    sources/SocketTelemetrySourceImpl.cpp \
    raii/SafeFile.cpp \
    raii/SafeSocket.cpp \
    main.cpp \
    -o telemetry_logger

# Run
./telemetry_logger
```

### CMake Example

```cmake
cmake_minimum_required(VERSION 3.15)
project(TelemetryLogger CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(telemetry_logger
    logger/LogManager.cpp
    logger/LogMessage.cpp
    sink/ConsoleSinkImpl.cpp
    sink/FileSinkImpl.cpp
    sink/LogSinkFactory.cpp
    sources/FileTelemetrySourceImpl.cpp
    sources/SocketTelemetrySourceImpl.cpp
    raii/SafeFile.cpp
    raii/SafeSocket.cpp
    main.cpp
)

target_include_directories(telemetry_logger PRIVATE ${CMAKE_SOURCE_DIR})
```

## 🧪 Testing

The `main.cpp` includes diagnostic tests:

1. **Policy Threshold Verification**: Confirms policy constants compile
2. **Formatter Logic**: Tests valid/invalid data handling
3. **Factory Integration**: Verifies sink creation via factory
4. **Full System Flow**: End-to-end telemetry processing

Run tests:
```bash
./telemetry_logger
```

Expected output:
```
************************************************************
      PHASE 3: TELEMETRY LOGGER - COMPLETE DIAGNOSTIC
************************************************************

==================================================
  Test 1: Policy Thresholds
==================================================
  [PASS] Threshold constants compiled successfully.

==================================================
  Test 2: Formatter & Severity Mapping
==================================================
  [PASS] Value 45.0 produced a valid LogMessage object.
  [PASS] Invalid data correctly returned nullopt.
  
...
```

## 🎨 Design Patterns

### 1. **Policy-Based Design**
Templates enable compile-time customization without runtime overhead:
```cpp
LogFormatter<CpuPolicy>  // CPU-specific thresholds
LogFormatter<GpuPolicy>  // GPU-specific thresholds
```

### 2. **Factory Pattern**
Centralized sink creation with consistent interface:
```cpp
auto sink = LogSinkFactory::createSink(LogSinkType_enum::FILE, "output.log");
```

### 3. **RAII (Resource Acquisition Is Initialization)**
Automatic resource cleanup for files and sockets:
```cpp
SafeFile file(path);   // Opens in constructor
// ... use file ...
// Automatically closes in destructor
```

### 4. **Strategy Pattern**
Interchangeable telemetry sources via interface:
```cpp
ITelemetrySource* source = new FileTelemetrySourceImpl(path);
source->openSource();
source->readSource(data);
```

## 🔍 Key Implementation Details

### Template Implementation Visibility
The `LogFormatter.tpp` file is included in the header to ensure template instantiation works correctly:
```cpp
// LogFormatter.hpp
#include "LogFormatter.tpp"  // Required for templates
```

### Move Semantics
The system uses move semantics extensively to avoid copying:
```cpp
manager.addSink(std::move(sink));  // Transfers ownership
manager.addLog(std::move(*logMsg)); // Efficient log transfer
```

### Optional Return Values
Formatters return `std::optional<LogMessage>` for safe error handling:
```cpp
auto logMsg = formatter.formatDataToLogMsg("invalid");
if (logMsg.has_value()) {
    // Safe to use logMsg
}
```

### Severity Mapping
Policy severities are mapped to log types:
```cpp
CRITICAL → ERROR
WARNING  → WARNING
INFO     → INFO
```

## 📊 Performance Considerations

- **Zero-Cost Abstractions**: Template policies incur no runtime overhead
- **Move Semantics**: Reduces unnecessary copying of log objects
- **RAII**: Eliminates resource leaks without manual management
- **Factory Pattern**: Centralizes object creation for better optimization

## 🚧 Future Enhancements

- [ ] Socket sink implementation for network logging
- [ ] Async logging with thread pool
- [ ] Configurable threshold loading from file
- [ ] Log rotation and archival
- [ ] Structured logging (JSON output format)
- [ ] Performance metrics and benchmarking
- [ ] Custom policy creation via configuration

## 📝 License

This project is provided as-is for educational and commercial use.

## 🤝 Contributing

When extending the system:

1. **Adding New Policies**: Create a struct with `WARN_THRESHOLD`, `CRIT_THRESHOLD`, `unit`, and `inferSeverity()`
2. **Adding New Sinks**: Inherit from `ILogSink` and implement `write()`
3. **Adding New Sources**: Inherit from `ITelemetrySource` and implement `openSource()` and `readSource()`

### Example: Custom Policy

```cpp
struct NetworkPolicy {
    static constexpr std::string_view unit = "Mbps";
    static constexpr float WARN_THRESHOLD = 800.0f;
    static constexpr float CRIT_THRESHOLD = 950.0f;
    
    static SeverityLvl_enum inferSeverity(float value) {
        if (value >= CRIT_THRESHOLD) return SeverityLvl_enum::CRITICAL;
        if (value >= WARN_THRESHOLD) return SeverityLvl_enum::WARNING;
        return SeverityLvl_enum::INFO;
    }
};

// Usage
LogFormatter<NetworkPolicy> netFormatter("Router-01", "ETH0");
```

## 📧 Contact

For questions or suggestions, please open an issue or submit a pull request.

---

**Built with modern C++ best practices** | **RAII · Templates · Factory Pattern · Policy-Based Design**
