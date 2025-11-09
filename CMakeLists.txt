cmake_minimum_required(VERSION 4.0)
project(Chat)

set(CMAKE_CXX_STANDARD 20)

# Use vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/aliak/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "")

# Find the Boost libraries
find_package(Boost REQUIRED COMPONENTS system)

# --- Build Executables ---

add_executable(my_server CN_Assignment_01_Server.cpp)
add_executable(my_client CN_Assignment_01_Client.cpp)
add_executable(client2 client2.cpp)
add_executable(client3 client3.cpp)

# --- Include Boost headers for header-only libraries (like asio) ---
target_include_directories(my_server PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(my_client PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(client2 PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(client3 PRIVATE ${Boost_INCLUDE_DIRS})

# --- Link libraries ---
# Only link Boost::system (asio is header-only) + Windows sockets
target_link_libraries(my_server PRIVATE Boost::system ws2_32)
target_link_libraries(my_client PRIVATE Boost::system ws2_32)
target_link_libraries(client2 PRIVATE Boost::system ws2_32)
target_link_libraries(client3 PRIVATE Boost::system ws2_32)
