cmake_minimum_required(VERSION 3.5)

project(NetMessage LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_THREAD_PREFER_PTHREAD ON)                                                                                                                                                                                                           
set(THREADS_PREFER_PTHREAD_FLAG ON)                                                                                                                                                                                                           
find_package(Threads REQUIRED)

add_executable(NetMessage main.cpp netmessenger.h netmessenger.cpp
    netmessage.h netmessage.cpp)
