# Create a library from all source files
add_library(TeleLogLib STATIC
    logger/LogMessage.cpp
    logger/LogManager.cpp
    sink/ConsoleSinkImpl.cpp
    sink/FileSinkImpl.cpp
    raii/SafeFile.cpp
    raii/SafeSocket.cpp
    sources/FileTelemetrySourceImpl.cpp
    sources/SocketTelemetrySourceImpl.cpp
    sink/LogSinkFactory.cpp
)

# Include paths
target_include_directories(TeleLogLib PUBLIC 
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/inc/formatter
    ${CMAKE_SOURCE_DIR}/inc/sink
)

# Link threads to the library so LogManager can use std::thread
target_link_libraries(TeleLogLib PUBLIC Threads::Threads)