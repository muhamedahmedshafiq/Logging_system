# Phase 2: Data Sources & Smart Resource Management

## 📋 Overview
Phase 2 extends the logging system with **telemetry data sources** that continuously receive data from multiple sources, implementing **RAII (Resource Acquisition Is Initialization)** principles for safe resource management.

---

## 🎯 Goals
1. Create actual data sources that continuously receive data from multiple source types
2. Implement RAII-compliant resource wrappers for safe file and socket management
3. Apply Move Semantics and Rule of 3/5/0 principles

---

## 🏗️ Architecture

### Components Hierarchy
```
ITelemetrySource (Interface)
    ├── FileTelemetrySourceImpl (uses SafeFile)
    └── SocketTelemetrySourceImpl (uses SafeSocket)

RAII Wrappers
    ├── SafeFile (wraps POSIX file operations)
    └── SafeSocket (wraps Unix Domain Socket operations)
```

### Data Flow
```
Data Sources → ITelemetrySource → LogManager → ILogSink → Output
     ↓              ↓                  ↓           ↓         ↓
/proc/loadavg   SafeFile          Buffers    ConsoleSink  stdout
Unix Socket     SafeSocket                   FileSink     files
```

---

## 📂 Project Structure

```
phase_2/
├── CMakeLists.txt
├── app/
│   ├── CMakeLists.txt
│   └── main.cpp
├── src/
│   ├── CMakeLists.txt
│   ├── logger/
│   │   ├── LogMessage.cpp
│   │   └── LogManager.cpp
│   ├── sink/
│   │   ├── ConsoleSinkImpl.cpp
│   │   └── FileSinkImpl.cpp
│   ├── raii/                          ⭐ NEW
│   │   ├── SafeFile.cpp
│   │   └── SafeSocket.cpp
│   └── sources/                       ⭐ NEW
│       ├── FileTelemetrySourceImpl.cpp
│       └── SocketTelemetrySourceImpl.cpp
├── inc/
│   ├── logger/
│   │   ├── LogMessage.hpp
│   │   └── LogManager.hpp
│   ├── sink/
│   │   ├── ILogSink.hpp
│   │   ├── ConsoleSinkImpl.hpp
│   │   └── FileSinkImpl.hpp
│   ├── raii/                          ⭐ NEW
│   │   ├── SafeFile.hpp
│   │   └── SafeSocket.hpp
│   └── sources/                       ⭐ NEW
│       ├── ITelemetrySource.hpp
│       ├── FileTelemetrySourceImpl.hpp
│       └── SocketTelemetrySourceImpl.hpp
└── simple_telemetry_server.sh         ⭐ NEW (for testing)
```

---

## 🔑 Key Concepts Implemented

### 1. **RAII (Resource Acquisition Is Initialization)**
Resources are acquired in constructors and released in destructors, guaranteeing proper cleanup even with exceptions.

```cpp
// Constructor acquires resource
SafeFile::SafeFile(const std::string& path) {
    filefd = open(path.c_str(), O_RDWR);
}

// Destructor releases resource (automatic cleanup)
SafeFile::~SafeFile() {
    if(filefd != -1) {
        close(filefd);
    }
}
```

### 2. **Move Semantics**
Transfer ownership of resources instead of copying (which is impossible for file descriptors).

```cpp
// Move constructor - transfers ownership
SafeFile::SafeFile(SafeFile&& other) noexcept 
    : filefd{other.filefd} {
    other.filefd = -1;  // Invalidate source
}

// Copy constructor - DELETED (cannot copy file descriptors)
SafeFile(const SafeFile&) = delete;
```

### 3. **Rule of 5**
If you define any special member function, define all 5:
- Destructor
- Copy Constructor (deleted)
- Copy Assignment (deleted)
- Move Constructor
- Move Assignment

### 4. **Rule of Zero**
Classes like `LogMessage` and `FileTelemetrySourceImpl` use only standard library types that manage resources automatically, so no special member functions are needed.

---

## 🔨 Building the Project

### Prerequisites
```bash
# Ensure you have:
- CMake 3.10+
- g++ with C++17 support
- netcat (for socket testing)
```

### Build Steps
```bash
# From project root
mkdir build
cd build
cmake ..
cmake --build .
```

### Run
```bash
./TeleLog
```

---

## 🧪 Testing

### Test 1: File Telemetry Source
**No setup needed** - reads from `/proc/loadavg`

```bash
./TeleLog
```

**Expected Output:**
```
📋 Testing File Telemetry Source...
✅ File source read successfully
2026-02-04 14:37:48 [INFO] [FileSource::CPU_Load] 1.67 1.41 1.22 1/2989 39416
```

### Test 2: Socket Telemetry Source

#### Step 1: Install netcat (if needed)
```bash
sudo apt-get install netcat
```

#### Step 2: Make server executable
```bash
chmod +x simple_telemetry_server.sh
```

#### Step 3: Start server (Terminal 1)
```bash
./simple_telemetry_server.sh
```

**Expected Output:**
```
✅ Starting simple telemetry server on /tmp/telemetry.sock
Sending float numbers every second...
```

#### Step 4: Run application (Terminal 2)
```bash
cd build
./TeleLog
```

**Expected Output:**
```
🔌 Testing Socket Telemetry Source...
✅ Socket connected successfully
📥 Received reading 1
📥 Received reading 2
...
2026-02-04 14:38:00 [INFO] [SocketSource::RemoteTelemetry] 42.7531
```

---

## 📊 Component Details

### ITelemetrySource Interface
```cpp
class ITelemetrySource {
public:
    virtual bool openSource() = 0;           // Establish connection
    virtual bool readSource(std::string& out) = 0;  // Read data
    virtual ~ITelemetrySource() = default;
};
```

### SafeFile (RAII Wrapper)
- **Purpose**: Safe management of POSIX file descriptors
- **Features**:
  - Automatic file closing on destruction
  - Move-only (cannot be copied)
  - Exception-safe
- **API**:
  - `SafeFile(const std::string& path)` - Opens file
  - `bool IsOpen()` - Check if file is open
  - `std::string Read()` - Read line from file
  - `~SafeFile()` - Automatically closes file

### SafeSocket (RAII Wrapper)
- **Purpose**: Safe management of Unix Domain Sockets
- **Features**:
  - Automatic socket closing on destruction
  - Move-only (cannot be copied)
  - Handles connection failures gracefully
- **API**:
  - `SafeSocket(const std::string& path)` - Creates and connects socket
  - `bool IsOpen()` - Check if socket is connected
  - `std::string Read()` - Read line from socket
  - `~SafeSocket()` - Automatically closes socket

### FileTelemetrySourceImpl
- **Purpose**: Read telemetry from files (e.g., `/proc/loadavg`)
- **Implementation**: Uses `SafeFile` internally
- **Features**: Rule of Zero compliant

### SocketTelemetrySourceImpl
- **Purpose**: Read telemetry from Unix Domain Sockets
- **Implementation**: Uses `SafeSocket` internally
- **Features**: Rule of Zero compliant

---

## 🎓 Learning Outcomes

### RAII Benefits Demonstrated
✅ **Automatic cleanup** - Resources freed even if exceptions occur  
✅ **No memory leaks** - Destructors guarantee cleanup  
✅ **Clear ownership** - `unique_ptr` makes ownership explicit  
✅ **Exception safety** - Resources always released properly  

### Move Semantics Benefits
✅ **Efficient** - No expensive copying  
✅ **Correct** - File descriptors cannot be copied  
✅ **Safe** - Original object invalidated after move  

### Design Patterns Applied
✅ **RAII Idiom** - Resource management tied to object lifetime  
✅ **Strategy Pattern** - Different telemetry sources (File, Socket)  
✅ **Rule of Zero** - Prefer standard library for automatic management  

---

## 🐛 Troubleshooting

### Problem: "Failed to open file source"
**Cause**: `/proc/loadavg` doesn't exist (not on Linux)  
**Solution**: Test on Linux system or use a regular file

### Problem: "Socket connection failed"
**Cause**: Server not running  
**Solution**: Start `simple_telemetry_server.sh` in another terminal

### Problem: "Address already in use"
**Cause**: Socket file left from previous run  
**Solution**: 
```bash
rm -f /tmp/telemetry.sock
```

### Problem: Build errors with includes
**Cause**: Wrong include paths  
**Solution**: All includes should be relative to `inc/` directory:
```cpp
#include "sources/ITelemetrySource.hpp"  // ✅ Correct
#include "ITelemetrySource.hpp"          // ❌ Wrong
```

---

## 📈 Phase 2 Requirements Checklist

| Requirement | Status | Notes |
|-------------|--------|-------|
| `ITelemetrySource` interface | ✅ | 3 methods: open, read, destructor |
| `SafeFile` RAII class | ✅ | Move-only, POSIX file wrapper |
| `SafeSocket` RAII class | ✅ | Move-only, Unix socket wrapper |
| `FileTelemetrySourceImpl` | ✅ | Uses SafeFile, Rule of Zero |
| `SocketTelemetrySourceImpl` | ✅ | Uses SafeSocket, Rule of Zero |
| LogMessage refactored | ✅ | All special members defaulted |
| Move semantics | ✅ | Implemented for both RAII classes |
| Copy prevention | ✅ | Copy operations deleted |
| Rule of 5 compliance | ✅ | SafeFile & SafeSocket |
| Rule of 0 compliance | ✅ | Source implementations |

---

## 🚀 Next Steps (Phase 3)

Phase 2 provides the foundation for:
- Asynchronous data collection
- Thread-safe logging
- Multiple concurrent data sources
- Real-time telemetry monitoring

---

## 📚 References

- **RAII**: [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-raii)
- **Move Semantics**: [cppreference.com](https://en.cppreference.com/w/cpp/language/move_constructor)
- **Unix Domain Sockets**: `man 7 unix`
- **POSIX File Operations**: `man 2 open`, `man 2 read`, `man 2 close`

---

## 👥 Contributing

Phase 2 demonstrates proper resource management in C++. Key principles:
1. Always use RAII for resource management
2. Prefer move semantics over copying for unique resources
3. Follow Rule of Zero when possible
4. Use Rule of 5 when managing raw resources

---

**Status**: ✅ Phase 2 Complete  
**Grade**: A  
**Next**: Phase 3 - Asynchronous Operations & Thread Safety
