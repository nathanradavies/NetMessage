cmake_minimum_required(VERSION 3.5)

project(NetMessage LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(NetMessage main.cpp netmessenger.h netmessenger.cpp
    netmessage.h netmessage.cpp)

target_link_libraries(NetMessage PUBLIC Boost::boost)
