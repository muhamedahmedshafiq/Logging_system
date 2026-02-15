cmake_minimum_required(VERSION 3.10)
project(TeleLogSystem)

# 1. SET C++ STANDARD
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. FIND THREADS (Crucial: Must be in root)
find_package(Threads REQUIRED)

# 3. ADD SUBDIRECTORIES
add_subdirectory(src)
add_subdirectory(app)