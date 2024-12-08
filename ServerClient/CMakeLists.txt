# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.12)

# Define the project name
project(ServerClient LANGUAGES CXX)

# Set the C++ standard (e.g., C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the executable
add_executable(ServerClient
    src/test.cpp
    include/Logs.h
    include/TcpServer.h
    include/TcpClient.h
    include/ClientSession.h
)

# Include directories for header files
target_include_directories(ServerClient PRIVATE include)

# Add compiler warnings (optional but recommended)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(ServerClient PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(ServerClient PRIVATE /W4 /WX)
endif()

# Add libraries if needed (example: linking with pthread on Unix)
if (UNIX AND NOT APPLE)
    target_link_libraries(ServerClient PRIVATE pthread)
endif()
