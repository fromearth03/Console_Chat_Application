Chat Application (C++ / Boost.Asio)

This is a multi-threaded chat application built in C++ using Boost.Asio. It supports multiple clients connecting to a server simultaneously, public messages, private messages using @username, and colored console output for messages, join/leave notifications, and errors.

Features

Multi-threaded server handling multiple clients.

Public and private messaging (@username).

Real-time join/leave notifications.

Colored console output for better readability.

Supports Windows console colors.

Prerequisites

C++20 compatible compiler (e.g., MinGW-w64 for Windows)

CMake 4.0+

Boost C++ Libraries (only system is required; asio is header-only)

vcpkg (for package management on Windows)

CLion or VSCode

Installing Boost via vcpkg

Clone vcpkg (if not installed)

git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat


Install Boost libraries

.\vcpkg install boost-system:x64-mingw-static


Set vcpkg toolchain in CMake

Replace YOUR_VCPKG_PATH with the path to your local vcpkg folder:

# Example: C:/Users/YourName/vcpkg
set(CMAKE_TOOLCHAIN_FILE "YOUR_VCPKG_PATH/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "")

Project Setup (CMake)

Your CMakeLists.txt should include:

cmake_minimum_required(VERSION 4.0)
project(Chat)

set(CMAKE_CXX_STANDARD 20)

# Use vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "YOUR_VCPKG_PATH/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "")

# Find Boost libraries
find_package(Boost REQUIRED COMPONENTS system)

# Build Executables
add_executable(my_server CN_Assignment_01_Server.cpp)
add_executable(my_client CN_Assignment_01_Client.cpp)
add_executable(client2 client2.cpp)
add_executable(client3 client3.cpp)

# Include Boost headers
target_include_directories(my_server PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(my_client PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(client2 PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(client3 PRIVATE ${Boost_INCLUDE_DIRS})

# Link Boost system and Windows sockets
target_link_libraries(my_server PRIVATE Boost::system ws2_32)
target_link_libraries(my_client PRIVATE Boost::system ws2_32)
target_link_libraries(client2 PRIVATE Boost::system ws2_32)
target_link_libraries(client3 PRIVATE Boost::system ws2_32)

Running in CLion

Open CLion and import the project using CMakeLists.txt.

Make sure vcpkg toolchain file is correctly set in the CMake settings.

Build the project (CLion uses Ninja or MinGW automatically).

Run the server:

Select my_server target and click Run.

Run clients:

Select my_client (or client2/client3) target and click Run.

The server must run first before starting any clients.

Running in VSCode

Open the project folder in VSCode.

Install CMake Tools extension.

Make sure your CMake toolchain file is set in CMakeSettings.json or via cmake.configureArgs.

Configure the project with CMake:

cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=YOUR_VCPKG_PATH/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-static


Build the project:

cmake --build build --target my_server
cmake --build build --target my_client


Run server first, then clients. Use separate terminals for each client.

Usage

Enter a username when prompted.

Type messages and press Enter.

Use @username message to send private messages.

Type /quit to exit the client.

Notes

Only Boost.System is required; Boost.Asio is header-only.

Console colors work on Windows terminals.

Ensure all clients connect to the correct server IP and port (127.0.0.1:8085 by default).

This README allows anyone to install Boost, build, and run the project on Windows using CLion or VSCode by just replacing YOUR_VCPKG_PATH with their actual vcpkg path.
