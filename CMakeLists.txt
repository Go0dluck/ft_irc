cmake_minimum_required(VERSION 3.16)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv server.cpp client.cpp)