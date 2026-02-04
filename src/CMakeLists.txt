# Create a library from all source files
add_library(TeleLogLib STATIC
    # Phase 1: Logger & Sinks
    logger/LogMessage.cpp
    logger/LogManager.cpp
    sink/ConsoleSinkImpl.cpp
    sink/FileSinkImpl.cpp
    
    # Phase 2: RAII Wrappers & Telemetry Sources
    raii/SafeFile.cpp
    raii/SafeSocket.cpp
    sources/FileTelemetrySourceImpl.cpp
    sources/SocketTelemetrySourceImpl.cpp
)

# Make inc/ available to this library
target_include_directories(TeleLogLib PUBLIC ${CMAKE_SOURCE_DIR}/inc)