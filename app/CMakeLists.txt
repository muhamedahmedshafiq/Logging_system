# Define the executable
add_executable(TeleLog main.cpp)

# Link the Library and the Threads for final binary generation
target_link_libraries(TeleLog PRIVATE TeleLogLib Threads::Threads)

# Header paths for main.cpp
target_include_directories(TeleLog PRIVATE 
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/inc/formatter
)