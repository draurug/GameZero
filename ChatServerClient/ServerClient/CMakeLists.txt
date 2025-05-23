cmake_minimum_required(VERSION 3.12)

project(ServerClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ServerClient STATIC
    src/Session.cpp
    src/test.cpp
    
    include/Logs.h
    include/TcpServer.h
    include/TcpClient.h
    include/Session.h
)

target_include_directories(ServerClient PRIVATE include)

# Add compiler warnings (optional but recommended)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(ServerClient PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(ServerClient PRIVATE /W4 /WX)
endif()

find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(ServerClient PRIVATE Boost::system)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if (UNIX AND NOT APPLE)
    target_link_libraries(ServerClient PRIVATE pthread)
endif()
