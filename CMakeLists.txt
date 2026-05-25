cmake_minimum_required(VERSION 4.2)
project(System_Monitor)

set(CMAKE_CXX_STANDARD 26)

add_executable(System_Monitor main.cpp
    PCinfo.cpp)

target_link_libraries(System_Monitor Psapi.lib)